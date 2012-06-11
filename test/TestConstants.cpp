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
