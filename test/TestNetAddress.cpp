#include <sstream>

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "NetAddress.h"

BOOST_AUTO_TEST_SUITE(Basics)

BOOST_AUTO_TEST_CASE(NetAddress01) {
  static const char* hostname = "www.google.com";
  static const char* servname = "http";

  IPFIX::NetAddress na(hostname, servname, AF_INET, PF_INET, false);

  BOOST_CHECK_EQUAL(na.hostname(), hostname);
  BOOST_CHECK_EQUAL(na.servname(), servname);
  BOOST_CHECK_EQUAL(na.protocol(), AF_INET);

  std::ostringstream os;

  os << hostname << "/" << servname << "/" << AF_INET;

  BOOST_CHECK_EQUAL(na.sessionname(), os.str());
}


BOOST_AUTO_TEST_SUITE_END()
