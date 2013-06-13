/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
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
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
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
 * Syntax: ipfix2csv -s [iespec-file] [ienames...] [ipfix-stream]
 *
 * E.g. ./ipfix2csv -s qof.iespec \
 *     sourceIPv4Address destinationIPv4Address \
 *     meanTcpRttMilliseconds reverseMeanTcpRttMilliseconds
 *   < ../qof/test/qof-test.ipfix \
 *   > qof-test-rtt-nocmin.csv
 *
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */
#include <cassert>
#include <cstddef>
#include <fstream>
#include <list>

#include <getopt.h>

#include "InfoElement.h"
#include "InfoModel.h"
#include "PlacementCollector.h"

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/configurator.h>
#  include <log4cplus/loggingmacros.h>
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

using namespace IPFIX;

static const char* spec_file_name = 0;
static int verbose_flag = false;
static int help_flag = false;
static std::list<const char*> ie_names;
  

/* Code patterned after http://www.gnu.org/software/libc/
 * manual/html_node/Getopt-Long-Option-Example.html
 * #Getopt-Long-Option-Example */
static void parse_options(int argc, char* const* argv) {

  while (1) {
    static struct option options[] = {
      { "help", no_argument, &help_flag, 1 },
      { "verbose", no_argument, &verbose_flag, 1 },
      { "specfile", required_argument, 0, 's' },
      { 0, 0, 0, 0 },
    };

    int option_index = 0;

    int c = getopt_long(argc, argv, "hs:v", options, &option_index);

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

int main(int argc, char* const* argv) {
#ifdef _LIBFC_HAVE_LOG4CPLUS_
  log4cplus::PropertyConfigurator config("log4cplus.properties");
  config.configure();
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

  InfoModel::instance().defaultIPFIX();
  parse_options(argc, argv);

  add_ies_from_spec_file();

  if (help_flag) {
    help();
    return EXIT_SUCCESS;
  }

  for (const char* s : ie_names) {
    std::cout << "IE " << s << std::endl;
  }
  return EXIT_SUCCESS;
}
