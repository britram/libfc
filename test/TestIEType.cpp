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
