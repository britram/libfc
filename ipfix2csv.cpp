/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * The name of ETH Zürich nor the names of other contributors 
 *      may be used to endorse or promote products derived from this software 
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH 
 * ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

/** Turn IPFIX stream into CSV.
 *
 * From Brian's ipfix2csv Python utility.
 *
 * Syntax: ipfix2csv [--version={5|9|10}|-v {5|9|10} [-s iespec-file] \
 *     [ienames...] [message-stream]
 *
 * E.g. ./ipfix2csv -s qof.iespec \
 *     sourceIPv4Address destinationIPv4Address 
 *   < ../qof/test/qof-test.ipfix \
 *   > qof-test-rtt-nocmin.csv
 *
 * Or:
 *
 * ./ipfix2csv --message-version=9 \
 *     --input=/zp0/statdat/test/19991_00098798_1398816000.dat.bz2 \
 *     sourceIPv4Address destinationIPv4Address             \
 *     meanTcpRttMilliseconds reverseMeanTcpRttMilliseconds
 *
 * Or:
 *
 * ./ipfix2csv \
 *     packetDeltaCount \
 *     reversePacketDeltaCount \
 *     octetDeltaCount \
 *     reverseOctetDeltaCount \
 *     tcpSequenceCount \
 *     reverseTcpSequenceCount \
 *     tcpSequenceLossCount \
 *     reverseTcpSequenceLossCount \
 *     tcpRetransmitCount \
 *     reverseTcpRetransmitCount \
 *     tcpRtxBurstCount \
 *     reverseTcpRtxBurstCount \
 *     tcpOutOfOrderCount \
 *     reverseTcpOutOfOrderCount \
 *     meanTcpRttMilliseconds \
 *     reverseMeanTcpRttMilliseconds \
 *     maxTcpFlightSize \
 *     reverseMaxTcpFlightSize \
 *     maxTcpReorderSize \
 *     flowStartMilliseconds \
 *     flowEndMilliseconds \
 *
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */
#include <cassert>
#include <cstddef>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>

#include <getopt.h>

#include "FileInputSource.h"
#include "InfoElement.h"
#include "InfoModel.h"
#include "PlacementCollector.h"
#include "PlacementTemplate.h"
#include "WandioInputSource.h"

#include "exceptions/FormatError.h"

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/configurator.h>
#  include <log4cplus/loggingmacros.h>
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

using namespace libfc;

static const char* spec_file_name = 0;
static int verbose_flag = false;
static int help_flag = false;
static int message_version = 10;
static int full_type_flag = 0;
static std::list<const char*> ie_names;
static std::string filename;

/* Code patterned after http://www.gnu.org/software/libc/
 * manual/html_node/Getopt-Long-Option-Example.html
 * #Getopt-Long-Option-Example */
static void parse_options(int argc, char* const* argv) {

  while (1) {
    static struct option options[] = {
      { "help", no_argument, &help_flag, 1 },
      { "input", required_argument, 0, 'i' },
      { "verbose", no_argument, &verbose_flag, 1 },
      { "message-version", required_argument, 0, 'm' },
      { "specfile", required_argument, 0, 's' },
      { "full-types", no_argument, &full_type_flag, 't' },
      { 0, 0, 0, 0 },
    };

    int option_index = 0;

    int c = getopt_long(argc, argv, "hi:m:s:tv", options, &option_index);

    if (c == -1)
      break;

    switch(c) {
    case 0:
      if (options[option_index].flag != 0)
        break;
      std::cerr << "option \"" << options[option_index].name << "\"";
      if (optarg)
        std::cerr << " with arg \"" << optarg << "\"";
      std::cerr << std::endl;
      break;
    case 'i':
      filename = optarg;
      break;
    case 'm':
      message_version = atoi(optarg);
      if (message_version != 9 && message_version != 10) {
        std::cerr << "Message version " << optarg 
                  << " is either unsupported or has a syntax error"
                  << " (only 9 and 10 are allowed)" << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case 's':
      spec_file_name = optarg;
      break;
    default:
      std::cerr << "Unrecognised option character '" << c 
                << "', aborting" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  if (optind < argc) {
    while (optind < argc)
      ie_names.push_back(argv[optind++]);
  }
}

static void help() {
  std::cerr << "usage: ./ipfix2csv [options] ie-names..." << std::endl
            << "Options:" << std::endl
            << "  -s file|--specfile=file" << std::endl
            << "\tuse FILE as IE spec filename" << std::endl
            << "  -h|--help\tprint this help text" << std::endl
            << "  -t|--full-types print full type info in columns" << std::endl
            << "  -v|--verbose\tprint verbose output" << std::endl;
}


static void
add_ies_from_spec_file() {
  if (spec_file_name != 0) {
    std::ifstream iespecs(spec_file_name);
    if (!iespecs)
      /* Silently ignore open error */
      return;

    std::string line;
    while (std::getline(iespecs, line))
      InfoModel::instance().add(line);

    /* Silently ignore close error */
    iespecs.close();
  }
}


static void
print_unsigned(std::ostream& os, const IEType* type, void* v) {
  switch (type->number()) {
  case IEType::kUnsigned8: os << static_cast<int>(*static_cast<uint8_t*>(v)); break;
  case IEType::kUnsigned16: os << *static_cast<uint16_t*>(v); break;
  case IEType::kUnsigned32: os << *static_cast<uint32_t*>(v); break;
  case IEType::kUnsigned64: os << *static_cast<uint64_t*>(v); break;
  default: /* Can't happen, ignore silently */ break;
  }
  
  if (full_type_flag) {
    switch (type->number()) {
    case IEType::kUnsigned8: os << "U"; break;
    case IEType::kUnsigned16: os << "U"; break;
    case IEType::kUnsigned32: os << "UL"; break;
    case IEType::kUnsigned64: os << "ULL"; break;
    default: /* Can't happen, ignore silently */ break;
    }
  }
}

static void
print_signed(std::ostream& os, const IEType* type, void* v) {
  switch (type->number()) {
  case IEType::kSigned8: os << static_cast<int>(*static_cast<int8_t*>(v)); break;
  case IEType::kSigned16: os << *static_cast<int16_t*>(v); break;
  case IEType::kSigned32: os << *static_cast<int32_t*>(v); break;
  case IEType::kSigned64: os << *static_cast<int64_t*>(v); break;
  default: /* Can't happen, ignore silently */ break;
  }

  if (full_type_flag) {
    switch (type->number()) {
    case IEType::kSigned8: break;
    case IEType::kSigned16: break;
    case IEType::kSigned32: os << "L"; break;
    case IEType::kSigned64: os << "LL"; break;
    default: /* Can't happen, ignore silently */ break;
    }
  }
}

static void
print_ipv4address(std::ostream& os, const IEType* type, void* v) {
  uint32_t val = *static_cast<uint32_t*>(v);
  os << ((val >> 24) & 0xff) 
     << '.' << ((val >> 16) & 0xff) 
     << '.' << ((val >>  8) & 0xff) 
     << '.' << ((val >>  0) & 0xff);
}

static void
print_ipv6address(std::ostream& os, const IEType* type, void* v) {
  uint8_t* val = static_cast<uint8_t*>(v);
  unsigned int i = 0;
  std::ios_base::fmtflags f = os.flags();

  os << std::hex;

  while (i < 16) {
    if (val[i] == 0) {
      os << ':';
      ++i;
      while (i < 16 && val[i] == 0)
        ++i;
    } else {
      if (i > 0)
        os << ':';
      os << val[i];
    }
    ++i;
  }

  os.flags(f);
}

static void
print_macaddress(std::ostream& os, const IEType* type, void* v) {
  uint8_t* val = static_cast<uint8_t*>(v);
  
  for (unsigned int i = 0; i < 6; i++) {
    unsigned char un = (val[i] >> 4) & 0x0f;
    unsigned char ln = (val[i] >> 0) & 0x0f;

    if (i > 0)
      os << ':';
    os << un << ln;
  }
}

static char
to_digit(unsigned int d) {
  assert(0 <= d && d < 10);
  switch(d) {
  case 0: return '0';
  case 1: return '1';
  case 2: return '2';
  case 3: return '3';
  case 4: return '4';
  case 5: return '5';
  case 6: return '6';
  case 7: return '7';
  case 8: return '8';
  case 9: return '9';
  }
  assert(d == 0);               /* Will evaluate to false. */
  return '\0';                  /* Shut up compiler. */
}

static void
print_iso_datetime(std::ostream& os, struct tm* tm, uint64_t fraction) {
  char buf[21];
  char* p = buf;

  assert(p + 5 < buf + sizeof(buf));
  tm->tm_year += 1900;
  assert(tm->tm_year < 10000);
  *(p + 3) = to_digit(tm->tm_year % 10); tm->tm_year /= 10;
  *(p + 2) = to_digit(tm->tm_year % 10); tm->tm_year /= 10;
  *(p + 1) = to_digit(tm->tm_year % 10); tm->tm_year /= 10;
  *(p + 0) = to_digit(tm->tm_year % 10);

  p += 4; *p++ = '-';

  assert(p + 3 < buf + sizeof(buf));
  tm->tm_mon++;
  assert(1 <= tm->tm_mon && tm->tm_mon <= 12);
  *(p + 1) = to_digit(tm->tm_mon % 10); tm->tm_mon /= 10;
  *(p + 0) = to_digit(tm->tm_mon % 10);

  p += 2; *p++ = '-';

  assert(p + 3 < buf + sizeof(buf));
  assert(1 <= tm->tm_mday && tm->tm_mday <= 31);
  *(p + 1) = to_digit(tm->tm_mday % 10); tm->tm_mday /= 10;
  *(p + 0) = to_digit(tm->tm_mday % 10);

  p += 2; *p++ = 'T';

  assert(p + 3 < buf + sizeof(buf));
  assert(0 <= tm->tm_hour && tm->tm_hour < 24);
  *(p + 1) = to_digit(tm->tm_hour % 10); tm->tm_hour /= 10;
  *(p + 0) = to_digit(tm->tm_hour % 10);

  p += 2; *p++ = ':';

  assert(p + 3 < buf + sizeof(buf));
  assert(0 <= tm->tm_min && tm->tm_min < 60);
  *(p + 1) = to_digit(tm->tm_min % 10); tm->tm_min /= 10;
  *(p + 0) = to_digit(tm->tm_min % 10);

  p += 2; *p++ = ':';

  assert(p + 4 == buf + sizeof(buf));
  assert(0 <= tm->tm_sec && tm->tm_sec <= 60); // Leap second
  *(p + 1) = to_digit(tm->tm_sec % 10); tm->tm_sec /= 10;
  *(p + 0) = to_digit(tm->tm_sec % 10);

  p += 2; *p++ = '.'; *p = '\0';

  os << buf;

  p = buf + 9;
  *p-- = '\0';

  while (fraction != 0 && fraction % 10 == 0)
    fraction /= 10;

  do {
    *p-- = to_digit(fraction % 10);
    fraction /= 10;
  } while (fraction != 0);

  os << (p + 1);
}

static void
print_datetime(std::ostream& os, const IEType* type, void* v) {
  /* Convert to dateTimeNanoseconds. */
  uint64_t nanos = 0;

  switch (type->number()) {
  case IEType::kDateTimeSeconds:
    nanos = *static_cast<uint32_t*>(v) * 1000000000ULL;
    break;
  case IEType::kDateTimeMilliseconds:
    nanos = *static_cast<uint64_t*>(v) * 1000000ULL;
    break;
  case IEType::kDateTimeMicroseconds:
    nanos = *static_cast<uint64_t*>(v) * 1000ULL;
    break;
  case IEType::kDateTimeNanoseconds:
    nanos = *static_cast<uint64_t*>(v) * 1ULL;
    break;
  default:
    /* Can't happen, ignore silently */
    break;
  }

  time_t seconds = nanos / 1000000000ULL;
  uint64_t fraction = nanos % 1000000000ULL;

  struct tm tm;
  gmtime_r(&seconds, &tm);
  print_iso_datetime(os, &tm, fraction);
}

static void
print_float(std::ostream& os, const IEType* type, void* v) {
  double d = 0.0;

  switch(type->number()) {
  case IEType::kFloat32: d = *static_cast<float*>(v); break;
  case IEType::kFloat64: d = *static_cast<double*>(v); break;
  default:
    /* Can't happen, ignore silently */
    break;
  }

  os << d;
}

static void
print_bool(std::ostream& os, const IEType* type, void* v) {
  bool b = *static_cast<uint8_t*>(v) == 0 ? false : true;

  os << (b ? "true" : "false");
}

static void
print_csv_header(std::ostream& os) {
  bool rest = false;
  for (const char* s : ie_names) {
    if (rest)
      os << ';';
    os << s;
    rest = true;
  }
  os << std::endl;
}

class CSVCollector : public PlacementCollector {
public:
  CSVCollector(PlacementCollector::Protocol protocol) 
    : PlacementCollector(protocol) {
    InfoModel& model = libfc::InfoModel::instance();

    csv_template = new PlacementTemplate();
    n_ies = ie_names.size();
    ie_values = new IEValue[n_ies];

    unsigned int i = 0;
    for (const char* s : ie_names) {
      const InfoElement* ie = model.lookupIE(s);
      if (ie == 0) {
        std::cerr << "Unknown IE " << s << std::endl;
        exit(EXIT_FAILURE);
      }

      ie_values[i].type = ie->ietype();
      ie_values[i].val = reinterpret_cast<void*>(0xdeadbeefdeadbeefULL);

      switch (ie_values[i].type->number()) {
      case IEType::kOctetArray: 
        std::cerr << "OctetArray not yet supported" << std::endl;
        exit(EXIT_FAILURE);
        break;

      case IEType::kUnsigned8:
        ie_values[i].val = new uint8_t[1];
        ie_values[i].renderer = print_unsigned;
        break;

      case IEType::kUnsigned16:
        ie_values[i].val = new uint8_t[2];
        ie_values[i].renderer = print_unsigned;
        break;

      case IEType::kUnsigned32:
        ie_values[i].val = new uint8_t[4];
        ie_values[i].renderer = print_unsigned;
        break;

      case IEType::kUnsigned64:
        ie_values[i].val = new uint8_t[8];
        ie_values[i].renderer = print_unsigned;
        break;

      case IEType::kSigned8:
        ie_values[i].val = new int8_t[1];
        ie_values[i].renderer = print_signed;
        break;

      case IEType::kSigned16:
        ie_values[i].val = new int8_t[2];
        ie_values[i].renderer = print_signed;
        break;

      case IEType::kSigned32:
        ie_values[i].val = new int8_t[4];
        ie_values[i].renderer = print_signed;
        break;

      case IEType::kSigned64:
        ie_values[i].val = new int8_t[8];
        ie_values[i].renderer = print_signed;
        break;

      case IEType::kFloat32:
        ie_values[i].val = new float[1];
        ie_values[i].renderer = print_float;
        break;

      case IEType::kFloat64:
        ie_values[i].val = new double[1];
        ie_values[i].renderer = print_float;
        break;

      case IEType::kBoolean:
        ie_values[i].val = new uint8_t[1];
        ie_values[i].renderer = print_bool;
        break;

      case IEType::kMacAddress:
        ie_values[i].val = new uint8_t[6];
        ie_values[i].renderer = print_macaddress;
        break;

      case IEType::kString:
        std::cerr << "String not yet supported" << std::endl;
        exit(EXIT_FAILURE);
        break;

      case IEType::kDateTimeSeconds:
        ie_values[i].val = new uint8_t[4];
        ie_values[i].renderer = print_datetime;
        break;

      case IEType::kDateTimeMilliseconds:
        ie_values[i].val = new uint8_t[8];
        ie_values[i].renderer = print_datetime;
        break;

      case IEType::kDateTimeMicroseconds:
        ie_values[i].val = new uint8_t[8];
        ie_values[i].renderer = print_datetime;
        break;

      case IEType::kDateTimeNanoseconds:
        ie_values[i].val = new uint8_t[8];
        ie_values[i].renderer = print_datetime;
        break;

      case IEType::kIpv4Address:
        ie_values[i].val = new uint8_t[4];
        ie_values[i].renderer = print_ipv4address;
        break;

      case IEType::kIpv6Address:
        ie_values[i].val = new uint8_t[16];
        ie_values[i].renderer = print_ipv6address;
        break;

      default:
        /* Can't happen */
        std::cerr << "Unknown IE type number " << ie->ietype()->number() 
                  << std::endl;
        exit(EXIT_FAILURE);
      }
      
      csv_template->register_placement(ie, ie_values[i].val, 0);

      ++i;
    }
    register_placement_template(csv_template);
  }
  
  std::shared_ptr<ErrorContext>
      start_placement(const PlacementTemplate* tmpl) {
    LIBFC_RETURN_OK();
  }

  std::shared_ptr<ErrorContext>
      end_placement(const PlacementTemplate* tmpl) {
    for (unsigned int i = 0; i < n_ies; ++i) {
      if (i > 0)
        std::cout << ';';
      ie_values[i].renderer(std::cout, ie_values[i].type, ie_values[i].val);
    }
    std::cout << std::endl;
    LIBFC_RETURN_OK();
  }

  ~CSVCollector() {
    delete csv_template;

    for (unsigned int i = 0; i < n_ies; ++i)
      delete[] static_cast<uint8_t*>(ie_values[i].val);

    delete[] ie_values;
  }

private:
  struct IEValue {
    void* val; /* Pointer to fixlen buffer or to varlen object */
    const IEType* type;
    void (*renderer)(std::ostream& os, const IEType* type, void* v);
  };

  size_t n_ies;
  IEValue* ie_values;
  PlacementTemplate* csv_template;
};

int main(int argc, char* const* argv) {
#ifdef _LIBFC_HAVE_LOG4CPLUS_
  log4cplus::PropertyConfigurator config("log4cplus.properties");
  config.configure();
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

  libfc::PlacementCollector::Protocol protocol;
  InputSource* is = 0;

  parse_options(argc, argv);

  if (message_version == 10) {
    InfoModel::instance().default5103();
    protocol = libfc::PlacementCollector::ipfix;
    is = new FileInputSource(0, "<stdin>"); // 0 == stdin
  } else if (message_version == 9) {
    InfoModel::instance().default5103();
    protocol = libfc::PlacementCollector::netflowv9;
    is = new WandioInputSource(filename);
  } else {
    std::cerr << "Unsupported message version " << message_version << std::endl;
    exit(EXIT_FAILURE);
  }

  add_ies_from_spec_file();

  if (help_flag) {
    help();
    return EXIT_SUCCESS;
  }

  std::cout.fill('0');

  CSVCollector cc{protocol};

  print_csv_header(std::cout);

  std::shared_ptr<ErrorContext> e = cc.collect(*is);
  if (e != 0) {
    std::cerr << e->to_string() << std::endl;
    return EXIT_FAILURE;
  }

  delete is;
  return EXIT_SUCCESS;
}
