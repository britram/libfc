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

#define BOOST_TEST_DYN_LINK
#include <boost/test/parameterized_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/logger.h>
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#  define LOG4CPLUS_ERROR(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include <cstdio>
#include <ctime>
#include <iostream>

#include <fcntl.h>

#include "BufferInputSource.h"
#include "Constants.h"
#include "PrintContentHandler.h"
#include "V9MessageStreamParser.h"
#include "WandioInputSource.h"
  
using namespace LIBFC;

BOOST_AUTO_TEST_SUITE(V9MessageStream)

BOOST_AUTO_TEST_CASE(Basic) {
  static const unsigned char msg01[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xce,0xbc,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x0 };
#if 0
  static const unsigned char msg02[] = {
    0x00,0x0a,0x00,0x56,0x50,0x6a,0xce,0xbc,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x18,0x03,0xe9,0x00,0x04,0x01,0x36,0x00,0x04,0x00,0x52,0xff,0xff,0x01,0x37,0x00,0x08,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x2e,0x10,0x20,0x30,0x40,0x04,0x65,0x74,0x68,0x30,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65 };
#endif

  PrintContentHandler ph{kV9Version};
  V9MessageStreamParser ir;

#ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger 
      = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("V9MessageStreamParser"));
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

  ir.set_content_handler(&ph);

  BufferInputSource is(msg01, sizeof(msg01));
  std::shared_ptr<ErrorContext> e = ir.parse(is);
  if (e != 0) {
#ifdef _LIBFC_HAVE_LOG4CPLUS_
    LOG4CPLUS_ERROR(logger, e->to_string());
#else /* !_LIBFC_HAVE_LOG4CPLUS_ */
    std::cerr << "Error: " << e->to_string() << std::endl;
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
  }
}

BOOST_AUTO_TEST_CASE(File01) {
  const char* name = "/zp0/statdat/test/19991_00098765_1398697200.dat.bz2";
  io_t* io = wandio_create(name);

  if (io != 0) {
#ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger 
      = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("V9MessageStreamParser"));
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

    WandioInputSource is{io, name};
    PrintContentHandler ph{kV9Version};
    V9MessageStreamParser ir;

    ir.set_content_handler(&ph);
    std::shared_ptr<ErrorContext> e = ir.parse(is);
    if (e != 0) {
#ifdef _LIBFC_HAVE_LOG4CPLUS_
      LOG4CPLUS_ERROR(logger, e->to_string());
#else /* !_LIBFC_HAVE_LOG4CPLUS_ */
      std::cerr << "Error: " << e->to_string() << std::endl;
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
    }
  }

  wandio_destroy(io);
}

BOOST_AUTO_TEST_CASE(File02) {
  const char* name = "m.gz";
  io_t* io = wandio_create(name);

  if (io != 0) {
#ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger 
      = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("V9MessageStreamParser"));
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

    WandioInputSource is{io, name};
    PrintContentHandler ph{kV9Version};
    V9MessageStreamParser ir;

    ir.set_content_handler(&ph);
    std::shared_ptr<ErrorContext> e = ir.parse(is);
    if (e != 0) {
#ifdef _LIBFC_HAVE_LOG4CPLUS_
      LOG4CPLUS_ERROR(logger, e->to_string());
#else /* !_LIBFC_HAVE_LOG4CPLUS_ */
      std::cerr << "Error: " << e->to_string() << std::endl;
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
    }
  }

  wandio_destroy(io);
}

BOOST_AUTO_TEST_SUITE_END()
