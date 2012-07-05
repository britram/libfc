#define BOOST_TEST_DYN_LINK

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "InfoModel.h"
#include "Transcoder.h"

BOOST_AUTO_TEST_SUITE(Basics)
BOOST_AUTO_TEST_SUITE(Transcoder)

static const size_t buf_size = 130;

BOOST_AUTO_TEST_CASE(BufferManagement) {
  IPFIX::Transcoder tc;
  uint8_t* buf = new uint8_t[buf_size];

  for (unsigned int i = 0; i < buf_size; i++) 
    buf[i] = i & 0xff;

  tc.setBase(buf, buf_size);

  BOOST_CHECK_EQUAL(tc.len(), 0);
  BOOST_CHECK_EQUAL(tc.avail(), buf_size);

  tc.zero();
  std::for_each(buf, buf + buf_size,
                [] (uint8_t c) {
                  BOOST_CHECK_EQUAL(c, '\0');
                });

  BOOST_CHECK_EQUAL(tc.base(), buf);
  BOOST_CHECK_EQUAL(tc.cur(), buf);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Transcoder)

BOOST_AUTO_TEST_CASE(TranscodeIPv4Address) {
  IPFIX::InfoModel m = IPFIX::InfoModel::instance();
  m.defaultIPFIX();
  const IPFIX::InfoElement* ie_sip = m.lookupIE("sourceIPv4Address");
  BOOST_CHECK(ie_sip != 0);

  uint8_t ipbuf[4] = { 127, 0, 0, 1 };
  IPFIX::Transcoder tc;

  tc.setBase(ipbuf, sizeof(ipbuf));
  tc.zero();

  BOOST_CHECK_EQUAL(tc.encodeAt(&ipbuf[0], sizeof(ipbuf), 0, ie_sip), 
                    sizeof(ipbuf));

  BOOST_CHECK_EQUAL(ipbuf[0], 1);
  BOOST_CHECK_EQUAL(ipbuf[1], 0);
  BOOST_CHECK_EQUAL(ipbuf[2], 0);
  BOOST_CHECK_EQUAL(ipbuf[3], 127);
}

BOOST_AUTO_TEST_SUITE_END()
