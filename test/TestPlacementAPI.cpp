/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of NEC Europe Ltd, Consorzio Nazionale 
 *      Interuniversitario per le Telecomunicazioni, Institut Telecom/Telecom 
 *      Bretagne, ETH Zürich, INVEA-TECH a.s. nor the names of its contributors 
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

#include <fcntl.h>

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include <log4cplus/configurator.h>

#include "BufferInputSource.h"
#include "DataSetDecoder.h"
#include "FileInputSource.h"
#include "IPFIXReader.h"
#include "InfoModel.h"

#include "exceptions/FormatError.h"

using namespace IPFIX;

BOOST_AUTO_TEST_SUITE(PlacementInterface)

BOOST_AUTO_TEST_CASE(SkipDataSet) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x56,0x50,0x6a,0xce,0xbc,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x18,0x03,0xe9,0x00,0x04,0x01,0x36,0x00,0x04,0x00,0x52,0xff,0xff,0x01,0x37,0x00,0x08,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x2e,0x10,0x20,0x30,0x40,0x04,0x65,0x74,0x68,0x30,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65 };
  log4cplus::BasicConfigurator config;
  config.configure();

  InfoModel::instance().defaultIPFIX();
  DataSetDecoder dsr;
  IPFIXReader ir;

  ir.set_content_handler(&dsr);
  ir.set_error_handler(&dsr);

  BufferInputSource is(msg, sizeof(msg));
  ir.parse(is);

}

BOOST_AUTO_TEST_CASE(FileDataSet) {
  const char* filename = "dahlem-01.ipfix";
  log4cplus::BasicConfigurator config;
  config.configure();

  InfoModel& info_model = InfoModel::instance();
  info_model.defaultIPFIX();
  info_model.add("dahlem-01-001(29305/1)<unsigned64>[8]");
  info_model.add("dahlem-01-002(29305/2)<unsigned64>[8]");
  info_model.add("dahlem-01-003(6871/40)<unsigned16>[2]");
  info_model.add("dahlem-01-004(6871/16424)<unsigned16>[2]");
  info_model.add("dahlem-01-005(6871/21)<unsigned32>[4]");
  info_model.add("dahlem-01-006(29305/184)<unsigned32>[4]");
  info_model.add("dahlem-01-007(6871/14)<unsigned8>[1]");
  info_model.add("dahlem-01-008(6871/15)<unsigned8>[1]");
  info_model.add("dahlem-01-009(6871/16398)<unsigned8>[1]");
  info_model.add("dahlem-01-010(6871/16399)<unsigned8>[1]");
  info_model.add("dahlem-01-011(29305/58)<unsigned16>[2]");
  info_model.add("dahlem-01-012(6871/18)<octetarray>[65535]");
  info_model.add("dahlem-01-013(6871/16402)<octetarray>[65535]");

  /* These are so far missing from the standard information model;
   * Brian says that he'll one day add all the missing stuff from
   * IANA. */
  info_model.add("basicList(0/291)<octetarray>[65535]");
  info_model.add("subTemplateList(0/292)<octetarray>[65535]");
  info_model.add("subTemplateMultiList(0/293)<octetarray>[65535]");

  DataSetDecoder dsr;
  IPFIXReader ir;

  ir.set_content_handler(&dsr);
  ir.set_error_handler(&dsr);

  int fd = open(filename, O_RDONLY);
  if (fd >= 0) {
    FileInputSource is(fd);
    try {
      ir.parse(is);
    } catch (FormatError e) {
      BOOST_FAIL("Format error: " << e.msg());
    }
    (void) close(fd);
  }
}

BOOST_AUTO_TEST_SUITE_END()
