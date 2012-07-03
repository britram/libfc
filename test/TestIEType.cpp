/* Copyright (c) 2011, NEC Europe Ltd, Consorzio Nazionale
 * Interuniversitario per le Telecomunicazioni, Institut
 * Telecom/Telecom Bretagne, ETH Zürich, INVEA-TECH a.s. All rights reserved.
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

#include "IEType.h"

BOOST_AUTO_TEST_SUITE(Basics)
BOOST_AUTO_TEST_SUITE(IEType)

BOOST_AUTO_TEST_CASE(OctetArray) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::octetArray(), IPFIX::IEType::octetArray());
}

BOOST_AUTO_TEST_CASE(Unsigned8) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::unsigned8(), IPFIX::IEType::unsigned8());
}

BOOST_AUTO_TEST_CASE(Unsigned16) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::unsigned16(), IPFIX::IEType::unsigned16());
}

BOOST_AUTO_TEST_CASE(Unsigned32) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::unsigned32(), IPFIX::IEType::unsigned32());
}

BOOST_AUTO_TEST_CASE(Unsigned64) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::unsigned64(), IPFIX::IEType::unsigned64());
}

BOOST_AUTO_TEST_CASE(Signed8) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::signed8(), IPFIX::IEType::signed8());
}

BOOST_AUTO_TEST_CASE(Signed16) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::signed16(), IPFIX::IEType::signed16());
}

BOOST_AUTO_TEST_CASE(Signed32) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::signed32(), IPFIX::IEType::signed32());
}

BOOST_AUTO_TEST_CASE(Signed64) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::signed64(), IPFIX::IEType::signed64());
}

BOOST_AUTO_TEST_CASE(Float32) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::float32(), IPFIX::IEType::float32());
}

BOOST_AUTO_TEST_CASE(Float64) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::float64(), IPFIX::IEType::float64());
}

BOOST_AUTO_TEST_CASE(Boolean) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::boolean(), IPFIX::IEType::boolean());
}

BOOST_AUTO_TEST_CASE(Macaddress) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::macAddress(), IPFIX::IEType::macAddress());
}

BOOST_AUTO_TEST_CASE(String) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::string(), IPFIX::IEType::string());
}
 
BOOST_AUTO_TEST_CASE(Datetimeseconds) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::dateTimeSeconds(), IPFIX::IEType::dateTimeSeconds());
}

BOOST_AUTO_TEST_CASE(Datetimemilliseconds) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::dateTimeMilliseconds(), IPFIX::IEType::dateTimeMilliseconds());
}

BOOST_AUTO_TEST_CASE(Datetimemicroseconds) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::dateTimeMicroseconds(), IPFIX::IEType::dateTimeMicroseconds());
}

BOOST_AUTO_TEST_CASE(Datetimenanoseconds) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::dateTimeNanoseconds(), IPFIX::IEType::dateTimeNanoseconds());
}

BOOST_AUTO_TEST_CASE(Ipv4address) {
  BOOST_CHECK_EQUAL(IPFIX::IEType::ipv4Address(), IPFIX::IEType::ipv4Address());
}

BOOST_AUTO_TEST_CASE(Ipv6address) {
    BOOST_CHECK_EQUAL(IPFIX::IEType::ipv6Address(), IPFIX::IEType::ipv6Address());
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
