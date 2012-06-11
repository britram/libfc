#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "MBuf.h"

BOOST_AUTO_TEST_SUITE(Basics)

BOOST_AUTO_TEST_CASE(MBuf01) {
  IPFIX::MBuf buf;
  BOOST_CHECK_EQUAL(buf.domain(), 0);
  BOOST_CHECK_EQUAL(buf.sequence(), 0);
  BOOST_CHECK_EQUAL(buf.export_time(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
