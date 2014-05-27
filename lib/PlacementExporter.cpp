/* Hi Emacs, please use -*- mode: C++; -*- */
/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of ETH Zürich nor the names of other contributors 
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
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <algorithm>
#include <climits>
#include <ctime>
#include <cstdarg>
#include <sstream>

#include <unistd.h>

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include "ipfix_endian.h"

#include "BasicOctetArray.h"
#include "PlacementExporter.h"

#include "exceptions/ExportError.h"


/** Encode plans describe how a data record is to be encoded.
 *
 * Decoding a data record means determining, for each data field, 
 *
 *   - if the data's endianness must be converted;
 *   - if the data needs to be transformed in any other way (for
 *     example, boolean values are encoded with 1 meaning true and 2
 *     meaning false(!!), or reduced-length encoding of floating-point
 *     values means that doubles are really transferred as floats); and
 *   - for variable-length data, what the length of the encoded value
 *     is.
 *
 * See also the documentation for DecodePlan in DataSetDecoder.cpp
 */
class EncodePlan {
public:
  /** Creates an encoding plan from a placement template.
   *
   * @param placement_template a placement template from which we
   *   encode a data record.
   */
  EncodePlan(const LIBFC::PlacementTemplate* placementTemplate);

  /** Executes this plan.
   *
   * @param buf the buffer where to store the encoded values
   * @param offset the offset at which to store the values
   * @param length the total length of the buffer
   *
   * @return the number of encoded octets
   */
  uint16_t execute(uint8_t* buf, uint16_t offset, uint16_t length);
  
private:
  struct Decision {
    /** The decision type. */
    enum decision_type_t {
      /** Value for an uninitialised decision type. */
      encode_none,

      /** Encode a boolean.  I'm repeating here the comment I made in
       * the corresponding declaration for transfer_boolean in
       * DataSetDecoder.cpp, because it still gets my blood up:
       *
       * Someone found it amusing in RFC 2579 to encode the boolean
       * values true and false as 1 and 2, respectively [sic!].  And
       * someone else found it amusing to standardise this behaviour
       * in RFC 5101 too.  This is of course wrong, since it disallows
       * entirely sensible operations like `plus' for "or", `times'
       * for "and" and `less than' for implication (which is what you
       * get when you make false less than true).
       *
       * This is why we can't subsume the encoding of booleans (which
       * are fixlen-encoded values of length 1) under
       * encode_basic_no_endianness below. */
      encode_boolean,

      /** Encode a basic type (fixlen) with no endianness conversion. */
      encode_fixlen,

      /** Encode a basic type (fixlen) with endianness conversion. */
      encode_fixlen_endianness,

      /** Encode a BasicOctetArray as fixlen. */
      encode_fixlen_octets,

      /** Encode a BasicOctetArray as varlen. Varlen encoding is
       * supported only for BasicOctetArray and derived classes.  In
       * all other instances, I'll do what Brian Trammell recommended
       * I do and tell the user to eff off. */
      encode_varlen,

      /** Encode double as float with endianness conversion. */
      encode_double_as_float_endianness,

      /** Encode double as float, no endianness conversion. */
      encode_double_as_float,
    } type;

    /** Address where original value is to be found. */
    const void* address;

    /** Size of original (unencoded) data. 
     *
     * If type is encode_varlen or encode_double_as_float or
     * encode_fixlen_octets, this field is implied and may not contain
     * a valid value.
     */
    size_t unencoded_length;

    /** Requested size of encoded data. 
     *
     * If type is encode_varlen or encode_double_as_float, this field
     * is implied and may not contain a valid value.
     */
    size_t encoded_length;

    /** Creates a printable version of this encoding decision. 
     *
     * @return a printable version of this encoding decision
     */
    std::string to_string() const;
  };

  std::vector<Decision> plan;

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
  log4cplus::Logger logger;
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */
};


static void report_error(const char* message, ...) {
  static const size_t buf_size = 10240;
  static char buf[buf_size];
  va_list args;
  
  va_start(args, message);
  int nchars = vsnprintf(buf, buf_size, message, args);
  va_end(args);

  if (nchars < 0)
    strcpy(buf, "Error while formatting error message");
  else if (static_cast<unsigned int>(nchars) > buf_size - 1 - 3) {
    buf[buf_size - 4] = '.';
    buf[buf_size - 3] = '.';
    buf[buf_size - 2] = '.';
    buf[buf_size - 1] = '\0';   // Shouldn't be necessary
  }

  throw LIBFC::ExportError(buf);
}

/* See DataSetDecoder::DecodePlan::DecodePlan. */
EncodePlan::EncodePlan(const LIBFC::PlacementTemplate* placement_template)
#ifdef _LIBFC_HAVE_LOG4CPLUS_
  : logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("EncodePlan")))
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
 {
#if defined(IPFIX_BIG_ENDIAN)
  Decision::decision_type_t encode_fixlen_maybe_endianness
    = Decision::encode_fixlen;
  Decision::decision_type_t encode_double_as_float_maybe_endianness
    = Decision::encode_double_as_float;
#elif defined(IPFIX_LITTLE_ENDIAN)
  Decision::decision_type_t encode_fixlen_maybe_endianness
    = Decision::encode_fixlen_endianness;
  Decision::decision_type_t encode_float_into_double_maybe_endianness
    = Decision::encode_double_as_float_endianness;
#else
#  error libfc does not compile on weird-endian machines.
#endif

  LOG4CPLUS_TRACE(logger, "Yay EncodePlan");

  for (auto ie = placement_template->begin();
       ie != placement_template->end();
       ++ie) {
    assert(*ie != 0);
    assert((*ie)->ietype() != 0);

    Decision d;
    void* location;
    size_t size;

    /* Either g++ is too stupid to figure out that the relevant fields
     * will all be set in the various cases below (not even with -O3),
     * or I really have forgotten to set them.  Unfortunately, all the
     * error message says is that "warning:
     * ‘d.EncodePlan::Decision::xxx’ may be used uninitialized in this
     * function", and then pointing to the *declaration* of d, and not
     * at the places where it thinks that the variable might be used
     * uninitialised.  I'm therefore forced to initialise (possibly
     * redundantly) the members of the struct, just to shut the
     * compiler up. Not helpful. */
    d.type = Decision::encode_none;
    d.unencoded_length = 0;
    d.encoded_length = 0;

    /* The IE *must* be present in the placement template. If not,
     * there is something very wrong in the PlacementTemplate
     * implementation.  Weird construction is to avoid call to
     * lookup_placement() to be thrown out when compiling with
     * -DNDEBUG. */
    bool ie_present 
      = placement_template->lookup_placement(*ie, &location, &size);
    assert(ie_present);

    d.address = location;

    switch ((*ie)->ietype()->number()) {
    case LIBFC::IEType::kOctetArray: 
      if (size == LIBFC::kIpfixVarlen) {
        d.type = Decision::encode_varlen;
      } else {
        d.type = Decision::encode_fixlen_octets;
        d.encoded_length = size;
      }
      break;

    case LIBFC::IEType::kUnsigned8:
      assert(size <= sizeof(uint8_t));

      d.type = Decision::encode_fixlen;
      d.unencoded_length = sizeof(uint8_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kUnsigned16:
      assert(size <= sizeof(uint16_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint16_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kUnsigned32:
      assert(size <= sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint32_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kUnsigned64:
      assert(size <= sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint64_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kSigned8:
      assert(size <= sizeof(int8_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int8_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kSigned16:
      assert(size <= sizeof(int16_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int16_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kSigned32:
      assert(size <= sizeof(int32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int32_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kSigned64:
      assert(size <= sizeof(int64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int64_t);
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kFloat32:
      /* Can't use reduced-length encoding on float; see RFC 5101,
       * Chapter 6, Verse 2. */
      assert(size == sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint32_t);
      d.encoded_length = sizeof(uint32_t);
      break;

    case LIBFC::IEType::kFloat64:
      assert(size == sizeof(uint32_t)
             || size == sizeof(uint64_t));

      d.unencoded_length = sizeof(uint64_t);
      d.encoded_length = size;
      if (d.unencoded_length == sizeof(uint32_t))
        d.type = encode_float_into_double_maybe_endianness;
      else
        d.type = encode_fixlen_maybe_endianness;
      break;

    case LIBFC::IEType::kBoolean:
      assert(size == sizeof(uint8_t));

      d.type = Decision::encode_boolean;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;

    case LIBFC::IEType::kMacAddress:
      /* RFC 5101 says to treat MAC addresses as 6-byte integers,
       * but Brian Trammell says that this is wrong and that the
       * RFC will be changed.  If for some reason this does not
       * come about, replace "encode_fixlen" with
       * "encode_fixlen_maybe_endianness". */
      assert(size == 6*sizeof(uint8_t));
             
      d.type = Decision::encode_fixlen;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case LIBFC::IEType::kString:
      if (size == LIBFC::kIpfixVarlen) {
        d.type = Decision::encode_varlen;
      } else {
        d.type = Decision::encode_fixlen_octets;
        d.encoded_length = size;
      }
      break;

    case LIBFC::IEType::kDateTimeSeconds:
      /* Must be encoded as a "32-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.7.
       *
       * The standard doesn't say whether the integer in question is
       * signed or unsigned, but since there is additional information
       * saying that "[t]he 32-bit integer allows the time encoding up
       * to 136 years", this makes sense only if the integer in
       * question is unsigned (signed integers give 68 years, in
       * either direction from the epoch). */
      assert(size == sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case LIBFC::IEType::kDateTimeMilliseconds:
      /* Must be encoded as a "64-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.8.
       *
       * The standard doesn't say whether the integer in question is
       * signed or unsigned, but in analogy with dateTimeSeconds, we
       * will assume the unsigned variant. */
      assert(size == sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case LIBFC::IEType::kDateTimeMicroseconds:
      /* Must be encoded as a "64-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.9. See dateTimeMilliseconds above. */
      assert(size == sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case LIBFC::IEType::kDateTimeNanoseconds:
      /* Must be encoded as a "64-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.10.  See dateTimeMicroseconds above. */
      assert(size == sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
                
    case LIBFC::IEType::kIpv4Address:
      /* RFC 5101 says to treat all addresses as integers. This
       * would mean endianness conversion for all of these address
       * types, including MAC addresses and IPv6 addresses. But the
       * only reasonable address type with endianness conversion is
       * the IPv4 address.  If for some reason this is not correct
       * replace "encode_fixlen_maybe_endianness" with
       * "encode_fixlen".
       *
       * Also, treating addresses as integers would subject them to
       * reduced-length encoding, a concept that is quite bizarre
       * since you can't do arithmetic on addresses.  We will
       * therefore not accept reduced-length encoding on addresses.
       */
      assert(size == sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case LIBFC::IEType::kIpv6Address:
      /* See comment on kIpv4Address. */
      assert(size == 16*sizeof(uint8_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    default: 
      report_error("Unknown IE type");
      break;
    }
    
    if ((d.type == Decision::encode_fixlen 
         || d.type == Decision::encode_fixlen_endianness)
        && d.encoded_length > d.unencoded_length) {
      /* Don't eliminate the temporary ie_spec.  If you do, the
       * temporary object created by toIESpec() may be deleted before
       * report_error is called, invalidating c_str(). */
      std::string ie_spec = (*ie)->toIESpec();
      report_error("IE %s encoded length %zu greater than native size %zu",
                   ie_spec.c_str(), d.encoded_length,
                   d.unencoded_length);
    }
    LOG4CPLUS_TRACE(logger, "encoding decision " << d.to_string());

    plan.push_back(d);
  }
}

std::string EncodePlan::Decision::to_string() const {
  std::stringstream sstr;

  sstr << "[";

  switch (type) {
  case encode_none: sstr << "encode_none"; break;
  case encode_boolean: sstr << "encode_boolean"; break;
  case encode_fixlen: sstr << "encode_fixlen"; break;
  case encode_fixlen_endianness: sstr << "encode_fixlen_endianness"; break;
  case encode_fixlen_octets: sstr << "encode_fixlen_octets"; break;
  case encode_varlen: sstr << "encode_varlen"; break;
  case encode_double_as_float_endianness:
    sstr << "encode_double_as_float_endianness";
    break;
  case encode_double_as_float:
    sstr << "encode_double_as_float";
    break;
  }

  sstr << "@" << address << "[" << encoded_length << "]";
  return sstr.str();
}

uint16_t EncodePlan::execute(uint8_t* buf, uint16_t offset,
                             uint16_t length) {
  uint16_t ret = 0;

  /* Make sure that there is space for at least one more octet. */
  assert(offset < length);

  for (auto i = plan.begin(); i != plan.end(); ++i) {
    /** An RFC 2579-encoded truth value.
     *
     * Really, look it up in http://tools.ietf.org/html/rfc2579 :
     *
     * TruthValue ::= TEXTUAL-CONVENTION
     *     STATUS       current
     *     DESCRIPTION
     *             "Represents a boolean value."
     *     SYNTAX       INTEGER { true(1), false(2) }
     *
     * Seriously, Internet? */
    static const uint8_t rfc2579_madness[] = { 2, 1 };

    uint16_t bytes_copied = 0;

    switch (i->type) {
    case Decision::encode_none:
      assert (0 == "being asked to encode_none");
      break;

    case Decision::encode_boolean:
      LOG4CPLUS_TRACE(logger, "encode_boolean");
      {
        const bool* p = static_cast<const bool*>(i->address);
        assert(offset + 1 <= length);
        buf[offset] = rfc2579_madness[static_cast<int>(*p != 0)];
        bytes_copied = 1;
      }
      break;

    case Decision::encode_fixlen:
      assert(offset + i->encoded_length <= length);
      memcpy(buf + offset,
             static_cast<const uint8_t*>(i->address) + i->unencoded_length - i->encoded_length,
             i->encoded_length);
      ret += i->encoded_length;
      offset += i->encoded_length;
      break;

    case Decision::encode_fixlen_endianness:
      {
        const uint8_t* src = static_cast<const uint8_t*>(i->address);
        uint8_t* dst = buf + offset + i->encoded_length - 1;

        assert(offset + i->encoded_length <= length);

        while (dst >= buf + offset)
          *dst-- = *src++;

        bytes_copied = i->encoded_length;
      }
      break;

    case Decision::encode_fixlen_octets:
      {
        const LIBFC::BasicOctetArray* src
          = static_cast<const LIBFC::BasicOctetArray*>(i->address);
        const size_t bytes_to_copy
          = std::min(src->get_length(), i->encoded_length);

        assert(offset + i->encoded_length <= length);

        memcpy(buf + offset, src->get_buf(), bytes_to_copy);
        memset(buf + offset + bytes_to_copy, '\0',
               i->encoded_length - bytes_to_copy);

        bytes_copied = i->encoded_length;
      }
      break;

    case Decision::encode_varlen:
      /* There seems to be no good way to do varlen encoding without
       * a lot of branches, either implicit or explicit.  It would
       * IMHO have been better if octetArray or string fields simply
       * had a 2-octet length field and be done with it. 
       *
       * Also, don't be worried about the many calls to get_length()
       * below; this is a const member function which allows the
       * compiler to optimise away all but one call to it. ---neuhaus */
      {
        const LIBFC::BasicOctetArray* src
          = static_cast<const LIBFC::BasicOctetArray*>(i->address);
        LOG4CPLUS_TRACE(logger,
                        "  encoding varlen length " << src->get_length());
        uint16_t memcpy_offset = src->get_length() < 255 ? 1 : 3;

        assert(offset + src->get_length() + memcpy_offset <= length);

        memcpy(buf + offset + memcpy_offset, src->get_buf(), 
               src->get_length());

        if (memcpy_offset == 1)
          buf[offset + 0] = static_cast<uint8_t>(src->get_length());
        else {
          buf[offset + 0] = UCHAR_MAX;
          buf[offset + 1] = static_cast<uint8_t>(src->get_length() >> 8);
          buf[offset + 2] = static_cast<uint8_t>(src->get_length() >> 0);
        }

        bytes_copied = src->get_length() + memcpy_offset;
      }
      break;

    case Decision::encode_double_as_float_endianness:
      {
        float f = *static_cast<const double*>(i->address);
        assert(sizeof(f) == sizeof(uint32_t));
        std::reverse_copy(reinterpret_cast<char*>(&f),
                          reinterpret_cast<char*>(&f) + sizeof(uint32_t) - 1,
                          buf);
        
        bytes_copied = sizeof(uint32_t);
      }
      break;

    case Decision::encode_double_as_float:
      {
        float f = *static_cast<const double*>(i->address);
        assert(sizeof(f) == sizeof(uint32_t));
        memcpy(buf, &f, sizeof(uint32_t));
        bytes_copied = sizeof(uint32_t);
      }
      break;
    }

    ret += bytes_copied;
    offset += bytes_copied;
  }

  return ret;
}


namespace LIBFC {

  static const unsigned int message_header_index = 0;
  static const unsigned int template_set_index = 1;

  PlacementExporter::PlacementExporter(ExportDestination& _os,
                                       uint32_t _observation_domain)
    : os(_os),
      current_template(0),
      current_template_id(255),
      sequence_number(0),
      observation_domain(_observation_domain), 
      n_message_octets(kIpfixMessageHeaderLen),
      template_set_size(0),
      plan(0)
#ifdef _LIBFC_HAVE_LOG4CPLUS_
    , logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("PlacementExporter")))
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
 {
    /* Push two empty iovecs into the iovec vector, to be filled later
     * with the message header and template set by flush(). */
   LOG4CPLUS_TRACE(logger, "First resize for message header and template set");
   iovecs.resize(2);
   iovecs[message_header_index].iov_base = 0;
   iovecs[message_header_index].iov_len = 0;
   iovecs[template_set_index].iov_base = 0;
   iovecs[template_set_index].iov_len = 0;
  }

  PlacementExporter::~PlacementExporter() {
    flush();

    delete plan;

    for (auto i = iovecs.begin(); i != iovecs.end(); ++i)
      delete[] static_cast<uint8_t*>(i->iov_base);
  }

  static void encode16(uint16_t val, uint8_t** buf,
                       const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint16_t) <= buf_end);
    *(*buf)++ = (val >> 8) & 0xff;
    *(*buf)++ = (val >> 0) & 0xff;
    assert(*buf <= buf_end);
  }

  static void encode32(uint32_t val, uint8_t** buf,
                       const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint32_t) <= buf_end);
    *(*buf)++ = (val >> 24) & 0xff;
    *(*buf)++ = (val >> 16) & 0xff;
    *(*buf)++ = (val >>  8) & 0xff;
    *(*buf)++ = (val >>  0) & 0xff;
    assert(*buf <= buf_end);
  }

  void PlacementExporter::finish_current_data_set() {
    iovec& l = iovecs.back();

    if (l.iov_len > 0) {
      LOG4CPLUS_TRACE(logger, "finishing current data set, len="
                      << l.iov_len);

      assert(l.iov_base != 0);
      uint8_t* buf = static_cast<uint8_t*>(l.iov_base);
      const uint8_t* buf_end = buf + 2*sizeof(uint16_t);
      
      encode16(current_template->get_template_id(), &buf, buf_end);
      encode16(l.iov_len, &buf, buf_end);
    }
  }

#ifdef _LIBFC_HAVE_LOG4CPLUS_
  static const char* make_time(uint32_t export_time) {
    struct tm tms;
    time_t then = export_time;
    static char gmtime_buf[100];

    gmtime_r(&then, &tms);
    strftime(gmtime_buf, sizeof gmtime_buf, "%c", &tms);
    gmtime_buf[sizeof(gmtime_buf) - 1] = '\0';

    return gmtime_buf;
  }
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

  bool PlacementExporter::flush() {
    LOG4CPLUS_TRACE(logger, "ENTER flush");
    /** Return value. */
    ssize_t ret = 0;

    /* Only write something if we have anything nontrivial to write. */
    if (n_message_octets > kIpfixMessageHeaderLen) {
      /** This message header.
       *
       * This variable is dynamically allocated so as to facilitate
       * its deletion later as part of the iovecs vector. */
      uint8_t* message_header = new uint8_t[kIpfixMessageHeaderLen];
      
      /** Points to the end of this message.
       *
       * Used for range checks. */
      const uint8_t* message_end = message_header + kIpfixMessageHeaderLen;
      
      /** Moves through the message header. */
      uint8_t* p = message_header;
      
      time_t now = time(0);
      if (now == static_cast<time_t>(-1))
        return false;
      
      /* Message header */
      encode16(kIpfixVersion, &p, message_end);
      encode16(static_cast<uint16_t>(n_message_octets), &p, message_end);
      encode32(static_cast<uint32_t>(now), &p, message_end);
      encode32(sequence_number++, &p, message_end);
      encode32(observation_domain, &p, message_end);
      
      iovecs[message_header_index].iov_base = message_header;
      iovecs[message_header_index].iov_len = kIpfixMessageHeaderLen;
      
      LOG4CPLUS_TRACE(logger, "writing message with "
                      << "version=" << kIpfixVersion
                      << ", length=" << n_message_octets
                      << ", export-time=" << make_time(now)
                      << ", sequence=" << (sequence_number - 1)
                      << ", domain=" << observation_domain);
      LOG4CPLUS_TRACE(logger, "" << iovecs.size() << " iovecs");

      /* Template set, if any */
      if (new_templates.size() != 0) {
        LOG4CPLUS_TRACE(logger, "writing template set...");

        iovecs[template_set_index].iov_len = template_set_size;
        iovecs[template_set_index].iov_base = new uint8_t[template_set_size];
        uint8_t* buf
          = static_cast<uint8_t*>(iovecs[template_set_index].iov_base);
        const uint8_t* buf_end = buf + template_set_size;

        encode16(2, &buf, buf_end);
        encode16(template_set_size, &buf, buf_end);

        for (auto t = new_templates.begin(); t != new_templates.end(); ++t) {
          const uint8_t* this_template;
          size_t this_template_size;

          (*t)->wire_template(0, &this_template, &this_template_size);
          assert(buf + this_template_size <= buf_end);
          memcpy(buf, this_template, this_template_size);
          buf += this_template_size;
        }
      }

      LOG4CPLUS_TRACE(logger, "finish 2");
      finish_current_data_set();

      ret = os.writev(iovecs);
      LOG4CPLUS_TRACE(logger, "wrote " << ret << " bytes");

#if defined(_LIBFC_HAVE_LOG4CPLUS_)
      int n = 0;
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */
      for (auto i = iovecs.begin(); i != iovecs.end(); ++i) {
        LOG4CPLUS_TRACE(logger, "  iovec " << ++n
                        << " size " << i->iov_len);
        delete[] static_cast<uint8_t*>(i->iov_base);
      }
      iovecs.clear();
      new_templates.clear();
      template_set_size = 0;

      LOG4CPLUS_TRACE(logger, "Making space for new message header");
      iovecs.resize(2);
      iovecs[message_header_index].iov_base = 0;
      iovecs[message_header_index].iov_len = 0;
      iovecs[template_set_index].iov_base = 0;
      iovecs[template_set_index].iov_len = 0;
      
      n_message_octets = kIpfixMessageHeaderLen;
    }
    return ret;
  }

  void PlacementExporter::place_values(const PlacementTemplate* tmpl) {
    LOG4CPLUS_TRACE(logger, "ENTER place_values");

    assert(n_message_octets <= kMaxMessageLen);

    /** The number of bytes added to the current message as a result
     * of issuing this new data record.  It might be as small as the
     * number of bytes in the representation of this data record, and
     * it might be as large as that number, plus the size of a new
     * template set containing the wire template for the template
     * used. */
    size_t record_size = tmpl->data_record_size();
    size_t new_bytes = record_size;
    bool make_new_data_set = false;

    LOG4CPLUS_TRACE(logger, "place_values: adding "
                    << new_bytes << " new bytes");

    /** Will contain tmpl if this template is hitherto unknown. */
    const PlacementTemplate* unknown_template = 0;

    if (tmpl != current_template) {
      LOG4CPLUS_TRACE(logger, "template not current");
      /* We need to insert a new template and start a new data set if
       *
       *  - the underlying transport is connection-oriented and we
       *    have never seen the template; or
       *  - the underlying transport is connectionless and we haven't
       *    seen the template in this message so far.
       */
      if (used_templates.find(tmpl) == used_templates.end()) {
        LOG4CPLUS_TRACE(logger, "template not known, inserting");
        unknown_template = tmpl;

        /* Need to create template set? */
        if (template_set_size == 0) {
          template_set_size += kIpfixSetHeaderLen;
          new_bytes += kIpfixSetHeaderLen;
          LOG4CPLUS_TRACE(logger, "need to create new template set, now "
                          << new_bytes << " new bytes");
        }

        /* Need to add a new template to the template record section */
        size_t template_bytes = 0;
        tmpl->wire_template(++current_template_id, 0, &template_bytes);
        new_bytes += template_bytes;
        template_set_size += template_bytes;
        new_templates.insert(tmpl);

        LOG4CPLUS_TRACE(logger, "computed wire template, now "
                        << new_bytes << " new bytes");

        /* Switch to new template here already, but only if this is
         * the very first template we see. */
        if (current_template == 0)
          current_template = tmpl;
      }

      LOG4CPLUS_TRACE(logger, "finish 1");
      finish_current_data_set();

      make_new_data_set = true;

      delete plan;
      plan = new EncodePlan(tmpl);
    }

    size_t prospective_data_set_header
      = make_new_data_set ? kIpfixSetHeaderLen : 0;
    if (n_message_octets + new_bytes + prospective_data_set_header
        > os.preferred_maximum_message_size()) {
      LOG4CPLUS_TRACE(logger,
                      "Flushing because n_message_octets ("
                      << n_message_octets
                      << ") + new_bytes (" << new_bytes
                      << ") > preferred ("
                      << os.preferred_maximum_message_size());
      flush();
      make_new_data_set = true;
    }

    if (make_new_data_set) {
      LOG4CPLUS_TRACE(logger, "make new data set");
      iovecs.resize(iovecs.size() + 1);

      iovec& l = iovecs.back();

      l.iov_base = new uint8_t[kMaxMessageLen];
      l.iov_len = kIpfixSetHeaderLen;
      new_bytes += kIpfixSetHeaderLen;
    }

#if defined(_LIBFC_HAVE_LOG4CPLUS_)
    if (n_message_octets + new_bytes > kMaxMessageLen)
      LOG4CPLUS_TRACE(logger,
                      "n_message_octets=" << n_message_octets
                      << ", new_bytes=" << new_bytes);
#endif /* defined(_LIBGC_HAVE_LOG4CPLUS_) */
    n_message_octets += new_bytes;
    assert(n_message_octets <= kMaxMessageLen);

    if (unknown_template != 0)
      used_templates.insert(unknown_template);

    iovec& l = iovecs.back();
    assert(l.iov_base != 0);
    uint16_t enc_bytes 
      = plan->execute(static_cast<uint8_t*>(l.iov_base),
                      l.iov_len, kMaxMessageLen);
    assert(enc_bytes == record_size);
    l.iov_len += enc_bytes;

    /* Either we already have current_template == tmpl, in which case
     * nothing happens, or current_template != tmpl, in which case we
     * need to switch to tmpl. */
    current_template = tmpl;


    assert(n_message_octets <= kMaxMessageLen);
  }

} // namespace LIBFC
