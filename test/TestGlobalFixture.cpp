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
#include "InfoModel.h"

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#ifdef _IPFIX_HAVE_LOG4CPLUS_
#  include <log4cplus/configurator.h>
#endif /* _IPFIX_HAVE_LOG4CPLUS_ */

using namespace IPFIX;

struct GlobalFixture {
  GlobalFixture();
  ~GlobalFixture();
};

GlobalFixture::GlobalFixture() {
  InfoModel& info_model = InfoModel::instance();
  info_model.defaultIPFIX();

  /* 5103 IEs that are not available in the default IPFIX (or in the
   * default5103() method of InfoModel). */
  info_model.add("dahlem-01-001(29305/1)<unsigned64>[8]");
  info_model.add("dahlem-01-002(29305/2)<unsigned64>[8]");
  info_model.add("dahlem-01-003(29305/184)<unsigned32>[4]");
  info_model.add("dahlem-01-004(29305/58)<unsigned16>[2]");

  /* YAF IEs. */
  info_model.add("dahlem-01-005(6871/40)<unsigned16>[2]");
  info_model.add("dahlem-01-006(6871/16424)<unsigned16>[2]");
  info_model.add("dahlem-01-007(6871/21)<unsigned32>[4]");
  info_model.add("dahlem-01-008(6871/14)<unsigned8>[1]");
  info_model.add("dahlem-01-009(6871/15)<unsigned8>[1]");
  info_model.add("dahlem-01-010(6871/16398)<unsigned8>[1]");
  info_model.add("dahlem-01-011(6871/16399)<unsigned8>[1]");
  info_model.add("dahlem-01-012(6871/18)<octetArray>[65535]");
  info_model.add("dahlem-01-013(6871/16402)<octetArray>[65535]");

  /* These are so far missing from the standard information model;
   * Brian says that he'll one day add all the missing stuff from
   * IANA. */
  info_model.add("basicList(291)<octetArray>[65535]");
  info_model.add("subTemplateList(292)<octetArray>[65535]");
  info_model.add("subTemplateMultiList(293)<octetArray>[65535]");

#ifdef _IPFIX_HAVE_LOG4CPLUS_
  log4cplus::BasicConfigurator config;
  config.configure();
#endif /* _IPFIX_HAVE_LOG4CPLUS_ */
}

GlobalFixture::~GlobalFixture() {
}

BOOST_GLOBAL_FIXTURE(GlobalFixture)
