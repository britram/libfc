/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ETH Zürich, nor the names of its contributors 
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

#include <fcntl.h>

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#if defined(_LIBFC_HAVE_LOG4CPLUS_)
#  include <log4cplus/logger.h>
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_DEBUG(logger, expr)
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */

#include "BufferInputSource.h"
#include "PlacementContentHandler.h"
#include "FileInputSource.h"
#include "IPFIXMessageStreamParser.h"
#include "InfoModel.h"
#include "PlacementCollector.h"

#include "exceptions/FormatError.h"

using namespace libfc;

BOOST_AUTO_TEST_SUITE(PlacementInterface)

BOOST_AUTO_TEST_CASE(SkipDataSet) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x56,0x50,0x6a,0xce,0xbc,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x18,0x03,0xe9,0x00,0x04,0x01,0x36,0x00,0x04,0x00,0x52,0xff,0xff,0x01,0x37,0x00,0x08,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x2e,0x10,0x20,0x30,0x40,0x04,0x65,0x74,0x68,0x30,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65 };

  PlacementContentHandler dsr;
  IPFIXMessageStreamParser ir;

  ir.set_content_handler(&dsr);

  BufferInputSource is(msg, sizeof(msg));
  ir.parse(is);

}

BOOST_AUTO_TEST_CASE(FileDataSet) {
  const char* filename = "dahlem-01.ipfix";

  class MyCollector : public PlacementCollector {
  public:
    MyCollector()
      : PlacementCollector(PlacementCollector::ipfix)
#if defined(_LIBFC_HAVE_LOG4CPLUS_)
      ,
      logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("logger")))
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */
    {
      PlacementTemplate* my_template = new PlacementTemplate();

      const InfoElement* sipv4a
        = InfoModel::instance().lookupIE("sourceIPv4Address");
      assert(sipv4a != 0);
      my_template->register_placement(sipv4a, &source_ipv4_address, 0);

      register_placement_template(my_template);
    }

    std::shared_ptr<ErrorContext>
        start_placement(const PlacementTemplate* tmpl) {
      LOG4CPLUS_DEBUG(logger, "MyCollector: START placement");
      LIBFC_RETURN_OK();
    }

    std::shared_ptr<ErrorContext>
        end_placement(const PlacementTemplate* tmpl) {
      LOG4CPLUS_DEBUG(logger, "MyCollector: END placement, address="
                      << std::hex << source_ipv4_address);
      LIBFC_RETURN_OK();
    }

  private:
#if defined(_LIBFC_HAVE_LOG4CPLUS_)
    log4cplus::Logger logger;
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */
    uint32_t source_ipv4_address;
  };

  MyCollector cb;

  int fd = open(filename, O_RDONLY);
  if (fd >= 0) {
    FileInputSource is(fd, filename);
    try {
      cb.collect(is);
    } catch (FormatError e) {
      BOOST_FAIL("Format error: " << e.what());
    }
    (void) close(fd);
  }
}

BOOST_AUTO_TEST_SUITE_END()
