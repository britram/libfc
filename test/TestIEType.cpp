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
#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "IEType.h"

BOOST_AUTO_TEST_SUITE(Basics)
BOOST_AUTO_TEST_SUITE(IEType)

/* Numerical values taken from IANA at https://www.iana.org/assignments/
 * ipfix/ipfix.xhtml#ipfix-information-element-data-types independently
 * of the implementation. */ 

BOOST_AUTO_TEST_CASE(OctetArray) {
  BOOST_CHECK_EQUAL(libfc::IEType::octetArray()->number(), 0);
}

BOOST_AUTO_TEST_CASE(Unsigned8) {
  BOOST_CHECK_EQUAL(libfc::IEType::unsigned8()->number(), 1);
}

BOOST_AUTO_TEST_CASE(Unsigned16) {
  BOOST_CHECK_EQUAL(libfc::IEType::unsigned16()->number(), 2);
}

BOOST_AUTO_TEST_CASE(Unsigned32) {
  BOOST_CHECK_EQUAL(libfc::IEType::unsigned32()->number(), 3);
}

BOOST_AUTO_TEST_CASE(Unsigned64) {
  BOOST_CHECK_EQUAL(libfc::IEType::unsigned64()->number(), 4);
}

BOOST_AUTO_TEST_CASE(Signed8) {
  BOOST_CHECK_EQUAL(libfc::IEType::signed8()->number(), 5);
}

BOOST_AUTO_TEST_CASE(Signed16) {
  BOOST_CHECK_EQUAL(libfc::IEType::signed16()->number(), 6);
}

BOOST_AUTO_TEST_CASE(Signed32) {
  BOOST_CHECK_EQUAL(libfc::IEType::signed32()->number(), 7);
}

BOOST_AUTO_TEST_CASE(Signed64) {
  BOOST_CHECK_EQUAL(libfc::IEType::signed64()->number(), 8);
}

BOOST_AUTO_TEST_CASE(Float32) {
  BOOST_CHECK_EQUAL(libfc::IEType::float32()->number(), 9);
}

BOOST_AUTO_TEST_CASE(Float64) {
  BOOST_CHECK_EQUAL(libfc::IEType::float64()->number(), 10);
}

BOOST_AUTO_TEST_CASE(Boolean) {
  BOOST_CHECK_EQUAL(libfc::IEType::boolean()->number(), 11);
}

BOOST_AUTO_TEST_CASE(Macaddress) {
  BOOST_CHECK_EQUAL(libfc::IEType::macAddress()->number(), 12);
}

BOOST_AUTO_TEST_CASE(String) {
  BOOST_CHECK_EQUAL(libfc::IEType::string()->number(), 13);
}
 
BOOST_AUTO_TEST_CASE(Datetimeseconds) {
  BOOST_CHECK_EQUAL(libfc::IEType::dateTimeSeconds()->number(), 14);
}

BOOST_AUTO_TEST_CASE(Datetimemilliseconds) {
  BOOST_CHECK_EQUAL(libfc::IEType::dateTimeMilliseconds()->number(), 15);
}

BOOST_AUTO_TEST_CASE(Datetimemicroseconds) {
  BOOST_CHECK_EQUAL(libfc::IEType::dateTimeMicroseconds()->number(), 16);
}

BOOST_AUTO_TEST_CASE(Datetimenanoseconds) {
  BOOST_CHECK_EQUAL(libfc::IEType::dateTimeNanoseconds()->number(), 17);
}

BOOST_AUTO_TEST_CASE(Ipv4address) {
  BOOST_CHECK_EQUAL(libfc::IEType::ipv4Address()->number(), 18);
}

BOOST_AUTO_TEST_CASE(Ipv6address) {
    BOOST_CHECK_EQUAL(libfc::IEType::ipv6Address()->number(), 19);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
