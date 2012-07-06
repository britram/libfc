#define BOOST_TEST_DYN_LINK

#include <boost/detail/endian.hpp>

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "InfoModel.h"
#include "Transcoder.h"

BOOST_AUTO_TEST_SUITE(Basics)
BOOST_AUTO_TEST_SUITE(Transcoder)


BOOST_AUTO_TEST_CASE(BufferManagement) {
  static const size_t buf_size = 130;

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

static uint32_t r32() {
  static uint32_t seed = 12345678;

  return seed = 69069*seed + 314159269;
}

static const unsigned int n_ipv4_transcode_tests = 1000;

BOOST_AUTO_TEST_SUITE(Transcoder)

BOOST_AUTO_TEST_CASE(TranscodeIPv4Address) {
  IPFIX::InfoModel m = IPFIX::InfoModel::instance();
  m.defaultIPFIX();
  const IPFIX::InfoElement* ie_sip = m.lookupIE("sourceIPv4Address");
  BOOST_CHECK(ie_sip != 0);

  for (unsigned int i = 0; i < n_ipv4_transcode_tests; i++) {
    uint32_t ip = r32();
    uint8_t* ipbuf = reinterpret_cast<uint8_t*>(&ip);
    uint8_t trbuf[4];
    IPFIX::Transcoder tc;

    tc.setBase(trbuf, sizeof(trbuf));
    tc.zero();

    BOOST_CHECK_EQUAL(tc.encode(&ipbuf[0], sizeof(ipbuf), ie_sip), true);

    /* Transcoding of IPv4 addresses changes byte order only
     * on little-endian machines. */
#if defined(BOOST_LITTLE_ENDIAN)
    BOOST_CHECK_EQUAL(trbuf[0], ipbuf[3]);
    BOOST_CHECK_EQUAL(trbuf[1], ipbuf[2]);
    BOOST_CHECK_EQUAL(trbuf[2], ipbuf[1]);
    BOOST_CHECK_EQUAL(trbuf[3], ipbuf[0]);
#else
    BOOST_CHECK_EQUAL(trbuf[0], ipbuf[0]);
    BOOST_CHECK_EQUAL(trbuf[1], ipbuf[1]);
    BOOST_CHECK_EQUAL(trbuf[2], ipbuf[2]);
    BOOST_CHECK_EQUAL(trbuf[3], ipbuf[3]);
#endif

    BOOST_CHECK_EQUAL(tc.avail(), 0);
    BOOST_CHECK_EQUAL(tc.len(), sizeof(trbuf));
  }
}

static const unsigned int n_ipv6_transcode_tests = 1000;

BOOST_AUTO_TEST_CASE(TranscodeIPv6Address) {
  IPFIX::InfoModel m = IPFIX::InfoModel::instance();
  m.defaultIPFIX();
  const IPFIX::InfoElement* ie_sip = m.lookupIE("sourceIPv6Address");
  BOOST_CHECK(ie_sip != 0);

  for (unsigned int i = 0; i < n_ipv6_transcode_tests; i++) {
    uint32_t ip[4] = { r32(), r32(), r32(), r32() };
    uint8_t* ipbuf = reinterpret_cast<uint8_t*>(&ip);
    uint8_t trbuf[16];
    IPFIX::Transcoder tc;

    tc.setBase(trbuf, sizeof(trbuf));
    tc.zero();

    // Use sizeof(ip), not sizeof(ipbuf).
    BOOST_CHECK_EQUAL(tc.encode(&ipbuf[0], sizeof(ip), ie_sip), true);
    
    /* Transcoding of IPv4 addresses never changes the byte order
     * because in IPv6, host byte order is big endian by
     * definition. */
    BOOST_CHECK_EQUAL(trbuf[ 0], ipbuf[ 0]);
    BOOST_CHECK_EQUAL(trbuf[ 1], ipbuf[ 1]);
    BOOST_CHECK_EQUAL(trbuf[ 2], ipbuf[ 2]);
    BOOST_CHECK_EQUAL(trbuf[ 3], ipbuf[ 3]);
    BOOST_CHECK_EQUAL(trbuf[ 4], ipbuf[ 4]);
    BOOST_CHECK_EQUAL(trbuf[ 5], ipbuf[ 5]);
    BOOST_CHECK_EQUAL(trbuf[ 6], ipbuf[ 6]);
    BOOST_CHECK_EQUAL(trbuf[ 7], ipbuf[ 7]);
    BOOST_CHECK_EQUAL(trbuf[ 8], ipbuf[ 8]);
    BOOST_CHECK_EQUAL(trbuf[ 9], ipbuf[ 9]);
    BOOST_CHECK_EQUAL(trbuf[10], ipbuf[10]);
    BOOST_CHECK_EQUAL(trbuf[11], ipbuf[11]);
    BOOST_CHECK_EQUAL(trbuf[12], ipbuf[12]);
    BOOST_CHECK_EQUAL(trbuf[13], ipbuf[13]);
    BOOST_CHECK_EQUAL(trbuf[14], ipbuf[14]);
    BOOST_CHECK_EQUAL(trbuf[15], ipbuf[15]);

    BOOST_CHECK_EQUAL(tc.avail(), 0);
    BOOST_CHECK_EQUAL(tc.len(), sizeof(trbuf));
  }
}

BOOST_AUTO_TEST_CASE(Focus) {
  static const size_t buf_size = 100;
  static const size_t offset = 51;

  uint8_t buf[buf_size];
  IPFIX::Transcoder tc;

  tc.setBase(buf, sizeof(buf));
  tc.zero();

  tc.focus(offset, buf_size - offset);
  BOOST_CHECK_EQUAL(tc.cur(), tc.base() + offset);
  BOOST_CHECK_EQUAL(tc.avail(), buf_size - offset);
}

BOOST_AUTO_TEST_CASE(FocusAndTranscodeOddAddress) {
  static const size_t buf_size = 100;
  static const size_t offset = 51;

  uint8_t buf[buf_size];
  IPFIX::Transcoder tc;

  tc.setBase(buf, sizeof(buf));
  tc.zero();

  tc.focus(offset, buf_size - offset);
  IPFIX::InfoModel m = IPFIX::InfoModel::instance();
  m.defaultIPFIX();
  const IPFIX::InfoElement* ie_sip = m.lookupIE("sourceIPv4Address");
  BOOST_CHECK(ie_sip != 0);

  uint32_t ip = r32();
  
  BOOST_CHECK_EQUAL(tc.encode(reinterpret_cast<uint8_t*>(&ip),
                              sizeof(ip), ie_sip), true);

#if defined(BOOST_LITTLE_ENDIAN)
    BOOST_CHECK_EQUAL(buf[offset + 0], reinterpret_cast<uint8_t*>(&ip)[3]);
    BOOST_CHECK_EQUAL(buf[offset + 1], reinterpret_cast<uint8_t*>(&ip)[2]);
    BOOST_CHECK_EQUAL(buf[offset + 2], reinterpret_cast<uint8_t*>(&ip)[1]);
    BOOST_CHECK_EQUAL(buf[offset + 3], reinterpret_cast<uint8_t*>(&ip)[0]);
#else
    BOOST_CHECK_EQUAL(buf[offset + 0], reinterpret_cast<uint8_t*>(&ip)[0]);
    BOOST_CHECK_EQUAL(buf[offset + 1], reinterpret_cast<uint8_t*>(&ip)[1]);
    BOOST_CHECK_EQUAL(buf[offset + 2], reinterpret_cast<uint8_t*>(&ip)[2]);
    BOOST_CHECK_EQUAL(buf[offset + 3], reinterpret_cast<uint8_t*>(&ip)[3]);
#endif

}

BOOST_AUTO_TEST_SUITE_END()
