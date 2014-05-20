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

#include <iostream>

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#ifdef _IPFIX_HAVE_LOG4CPLUS_
#  include <log4cplus/logger.h>
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_DEBUG(logger, expr)
#endif /* _IPFIX_HAVE_LOG4CPLUS_ */

#include "BufferInputSource.h"
#include "PlacementContentHandler.h"
#include "FileInputSource.h"
#include "IPFIXMessageStreamParser.h"
#include "InfoModel.h"
#include "PlacementCollector.h"

#include "exceptions/FormatError.h"

using namespace LIBFC;

BOOST_AUTO_TEST_SUITE(ErrorContext)

static const unsigned char good_msg[] = {
  0x00,0x0a,0x00,0x56,0x50,0x6a,0xce,0xbc,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x18,0x03,0xe9,0x00,0x04,0x01,0x36,0x00,0x04,0x00,0x52,0xff,0xff,0x01,0x37,0x00,0x08,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x2e,0x10,0x20,0x30,0x40,0x04,0x65,0x74,0x68,0x30,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65 };

static unsigned char* copy_message() {
  unsigned char* ret = new unsigned char[sizeof good_msg];
  return static_cast<unsigned char*>(memcpy(ret, good_msg, sizeof good_msg));
}
  

BOOST_AUTO_TEST_CASE(WrongMagic) {
  PlacementContentHandler dsr(PlacementContentHandler::ipfix);
  IPFIXMessageStreamParser ir;

  ir.set_content_handler(&dsr);

  unsigned char* msg = copy_message();
  msg[1] = 0x0b; // Wrong message version number 11

  /* Use good_msg for sizeof, not msg! */
  BufferInputSource is(msg, sizeof good_msg);
  std::shared_ptr<LIBFC::ErrorContext> err = ir.parse(is);

  BOOST_CHECK(err != 0);

  std::cout << err->to_string() << '\n';
  
  delete msg;
}

BOOST_AUTO_TEST_CASE(ShortMsg) {
  PlacementContentHandler dsr(PlacementContentHandler::ipfix);
  IPFIXMessageStreamParser ir;

  ir.set_content_handler(&dsr);

  unsigned char* msg = copy_message();
  msg[3] = 17; /* Artificially short message */

  /* Use good_msg for sizeof, not msg! */
  BufferInputSource is(msg, sizeof good_msg);
  std::shared_ptr<LIBFC::ErrorContext> err = ir.parse(is);

  BOOST_CHECK(err != 0);

  std::cout << err->to_string() << '\n';
  
  delete msg;
}

BOOST_AUTO_TEST_SUITE_END()
