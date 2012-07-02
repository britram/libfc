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

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "Constants.h"

BOOST_AUTO_TEST_SUITE(Basics)

BOOST_AUTO_TEST_CASE(Constants) {
  BOOST_CHECK_EQUAL(IPFIX::kIpfixVersion, 10);
  BOOST_CHECK_EQUAL(IPFIX::kMessageHeaderLen, 16);
  BOOST_CHECK_EQUAL(IPFIX::kSetHeaderLen, 4);
  BOOST_CHECK_EQUAL(IPFIX::kSetLenOffset, 2);
  BOOST_CHECK_EQUAL(IPFIX::kTemplateHeaderLen, 4);
  BOOST_CHECK_EQUAL(IPFIX::kMaxMessageLen, 65535);
  BOOST_CHECK_EQUAL(IPFIX::kMaxSetLen, 65519);
  BOOST_CHECK_EQUAL(IPFIX::kTemplateSetID, 2);
  BOOST_CHECK_EQUAL(IPFIX::kOptionTemplateSetID, 3);
  BOOST_CHECK_EQUAL(IPFIX::kMinSetID, 0x0100);
  BOOST_CHECK_EQUAL(IPFIX::kVarlen, 65535);
  BOOST_CHECK_EQUAL(IPFIX::kEnterpriseBit, 0x8000);
}

BOOST_AUTO_TEST_SUITE_END()
