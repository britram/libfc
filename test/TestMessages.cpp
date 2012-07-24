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

#include <iostream>
#include <sstream>
#include <vector>

#include <arpa/inet.h>

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "Constants.h"
#include "InfoModel.h"
#include "MBuf.h"
#include "OffsetCache.h"
#include "SetReceiver.h"

/*             WARNING: NO USER-SERVICEABLE PARTS INSIDE!
 * 
 * This Boost test suite has been generated automatically.  Any manual
 * changes you make here will be overwritten without any notice.
 *
 * Change at your own risk.
 */

class MyIp6Address {
public:
  MyIp6Address() {
    memset(buf, '\0', sizeof(buf));
  }

  MyIp6Address(uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3,
               uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7) {
    buf[0] = htons(v0);
    buf[1] = htons(v1);
    buf[2] = htons(v2);
    buf[3] = htons(v3);
    buf[4] = htons(v4);
    buf[5] = htons(v5);
    buf[6] = htons(v6);
    buf[7] = htons(v7);
  }

  bool operator==(const MyIp6Address& rhs) const {
    return memcmp(buf, rhs.buf, sizeof buf) == 0;
  }

  friend std::ostream& operator<<(std::ostream& os, const MyIp6Address& obj);

private:
  unsigned short buf[8];
};

std::ostream& operator<<(std::ostream& os, const MyIp6Address& obj) 
{ 
  for (unsigned int i = 0; i < sizeof(obj.buf)/sizeof(obj.buf[0]); i++) {
    if (i > 0)
      os << ',';
    os << std::hex << obj.buf[i];
  }
  return os;
} 

struct Fixture {
  Fixture() { IPFIX::InfoModel::instance().defaultIPFIX(); }
};

BOOST_AUTO_TEST_SUITE(Messages)

BOOST_FIXTURE_TEST_SUITE(IETypes, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message001) {
  static const unsigned char msg_001[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("protocolIdentifier"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_001), sizeof msg_001));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x00);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message002) {
  static const unsigned char msg_002[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("protocolIdentifier"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_002), sizeof msg_002));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x01);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message003) {
  static const unsigned char msg_003[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xfe  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("protocolIdentifier"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_003), sizeof msg_003));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xfe);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message004) {
  static const unsigned char msg_004[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xff  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("protocolIdentifier"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_004), sizeof msg_004));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xff);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message005) {
  static const unsigned char msg_005[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("protocolIdentifier"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_005), sizeof msg_005));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message006) {
  static const unsigned char msg_006[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("protocolIdentifier"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_006), sizeof msg_006));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x5a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(InformationElements, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message007) {
  static const unsigned char msg_007[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x01,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("octetDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_007), sizeof msg_007));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message008) {
  static const unsigned char msg_008[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x02,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("packetDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_008), sizeof msg_008));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message009) {
  static const unsigned char msg_009[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("protocolIdentifier"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_009), sizeof msg_009));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message010) {
  static const unsigned char msg_010[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x05,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipClassOfService"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_010), sizeof msg_010));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message011) {
  static const unsigned char msg_011[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x06,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpControlBits"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_011), sizeof msg_011));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message012) {
  static const unsigned char msg_012[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceTransportPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_012), sizeof msg_012));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message013) {
  static const unsigned char msg_013[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x08,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_013), sizeof msg_013));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message014) {
  static const unsigned char msg_014[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x09,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceIPv4PrefixLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_014), sizeof msg_014));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message015) {
  static const unsigned char msg_015[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ingressInterface"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_015), sizeof msg_015));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message016) {
  static const unsigned char msg_016[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0b,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("destinationTransportPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_016), sizeof msg_016));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message017) {
  static const unsigned char msg_017[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0c,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("destinationIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_017), sizeof msg_017));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message018) {
  static const unsigned char msg_018[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("destinationIPv4PrefixLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_018), sizeof msg_018));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message019) {
  static const unsigned char msg_019[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0e,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("egressInterface"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_019), sizeof msg_019));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message020) {
  static const unsigned char msg_020[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0f,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipNextHopIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_020), sizeof msg_020));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message021) {
  static const unsigned char msg_021[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x10,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("bgpSourceAsNumber"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_021), sizeof msg_021));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message022) {
  static const unsigned char msg_022[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x11,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("bgpDestinationAsNumber"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_022), sizeof msg_022));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message023) {
  static const unsigned char msg_023[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x12,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("bgpNextHopIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_023), sizeof msg_023));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message024) {
  static const unsigned char msg_024[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x13,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postMCastPacketDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_024), sizeof msg_024));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message025) {
  static const unsigned char msg_025[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x14,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postMCastOctetDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_025), sizeof msg_025));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message026) {
  static const unsigned char msg_026[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x15,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowEndSysUpTime"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_026), sizeof msg_026));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message027) {
  static const unsigned char msg_027[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x16,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowStartSysUpTime"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_027), sizeof msg_027));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message028) {
  static const unsigned char msg_028[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x17,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postOctetDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_028), sizeof msg_028));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message029) {
  static const unsigned char msg_029[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x18,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postPacketDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_029), sizeof msg_029));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message030) {
  static const unsigned char msg_030[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x19,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("minimumIpTotalLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_030), sizeof msg_030));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message031) {
  static const unsigned char msg_031[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("maximumIpTotalLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_031), sizeof msg_031));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message032) {
  static const unsigned char msg_032[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1b,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceIPv6Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_032), sizeof msg_032));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message033) {
  static const unsigned char msg_033[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1c,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("destinationIPv6Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_033), sizeof msg_033));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message034) {
  static const unsigned char msg_034[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceIPv6PrefixLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_034), sizeof msg_034));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message035) {
  static const unsigned char msg_035[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1e,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("destinationIPv6PrefixLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_035), sizeof msg_035));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message036) {
  static const unsigned char msg_036[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1f,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowLabelIPv6"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_036), sizeof msg_036));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message037) {
  static const unsigned char msg_037[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x20,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("icmpTypeCodeIPv4"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_037), sizeof msg_037));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message038) {
  static const unsigned char msg_038[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x21,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("igmpType"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_038), sizeof msg_038));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message039) {
  static const unsigned char msg_039[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x24,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowActiveTimeout"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_039), sizeof msg_039));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message040) {
  static const unsigned char msg_040[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x25,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowIdleTimeout"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_040), sizeof msg_040));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message041) {
  static const unsigned char msg_041[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x28,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exportedOctetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_041), sizeof msg_041));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message042) {
  static const unsigned char msg_042[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x29,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exportedMessageTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_042), sizeof msg_042));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message043) {
  static const unsigned char msg_043[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exportedFlowRecordTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_043), sizeof msg_043));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message044) {
  static const unsigned char msg_044[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2c,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceIPv4Prefix"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_044), sizeof msg_044));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message045) {
  static const unsigned char msg_045[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2d,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("destinationIPv4Prefix"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_045), sizeof msg_045));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message046) {
  static const unsigned char msg_046[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2e,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsTopLabelType"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_046), sizeof msg_046));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message047) {
  static const unsigned char msg_047[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2f,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsTopLabelIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_047), sizeof msg_047));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message048) {
  static const unsigned char msg_048[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x34,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("minimumTTL"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_048), sizeof msg_048));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message049) {
  static const unsigned char msg_049[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x35,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("maximumTTL"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_049), sizeof msg_049));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message050) {
  static const unsigned char msg_050[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x36,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("fragmentIdentification"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_050), sizeof msg_050));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message051) {
  static const unsigned char msg_051[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x37,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postIpClassOfService"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_051), sizeof msg_051));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message052) {
  static const unsigned char msg_052[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3a,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("vlanId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_052), sizeof msg_052));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message053) {
  static const unsigned char msg_053[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3b,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postVlanId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_053), sizeof msg_053));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message054) {
  static const unsigned char msg_054[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3c,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipVersion"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_054), sizeof msg_054));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message055) {
  static const unsigned char msg_055[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowDirection"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_055), sizeof msg_055));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message056) {
  static const unsigned char msg_056[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3e,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipNextHopIPv6Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_056), sizeof msg_056));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message057) {
  static const unsigned char msg_057[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3f,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("bgpNextHopIPv6Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_057), sizeof msg_057));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message058) {
  static const unsigned char msg_058[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x40,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipv6ExtensionHeaders"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_058), sizeof msg_058));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message059) {
  static const unsigned char msg_059[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x55,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("octetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_059), sizeof msg_059));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message060) {
  static const unsigned char msg_060[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x56,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("packetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_060), sizeof msg_060));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message061) {
  static const unsigned char msg_061[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x58,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("fragmentOffset"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_061), sizeof msg_061));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message062) {
  static const unsigned char msg_062[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x5b,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsTopLabelPrefixLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_062), sizeof msg_062));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message063) {
  static const unsigned char msg_063[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x62,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postIpDiffServCodePoint"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_063), sizeof msg_063));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message064) {
  static const unsigned char msg_064[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x80,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("bgpNextAdjacentAsNumber"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_064), sizeof msg_064));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message065) {
  static const unsigned char msg_065[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x81,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("bgpPrevAdjacentAsNumber"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_065), sizeof msg_065));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message066) {
  static const unsigned char msg_066[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x82,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exporterIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_066), sizeof msg_066));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message067) {
  static const unsigned char msg_067[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x83,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exporterIPv6Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_067), sizeof msg_067));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message068) {
  static const unsigned char msg_068[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x84,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("droppedOctetDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_068), sizeof msg_068));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message069) {
  static const unsigned char msg_069[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x85,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("droppedPacketDeltaCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_069), sizeof msg_069));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message070) {
  static const unsigned char msg_070[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x86,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("droppedOctetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_070), sizeof msg_070));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message071) {
  static const unsigned char msg_071[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x87,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("droppedPacketTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_071), sizeof msg_071));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message072) {
  static const unsigned char msg_072[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x88,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowEndReason"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_072), sizeof msg_072));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message073) {
  static const unsigned char msg_073[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x89,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("commonPropertiesId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_073), sizeof msg_073));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message074) {
  static const unsigned char msg_074[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8a,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("observationPointId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_074), sizeof msg_074));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message075) {
  static const unsigned char msg_075[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8b,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("icmpTypeCodeIPv6"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_075), sizeof msg_075));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message076) {
  static const unsigned char msg_076[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8c,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsTopLabelIPv6Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_076), sizeof msg_076));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message077) {
  static const unsigned char msg_077[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8d,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("lineCardId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_077), sizeof msg_077));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message078) {
  static const unsigned char msg_078[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8e,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("portId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_078), sizeof msg_078));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message079) {
  static const unsigned char msg_079[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8f,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("meteringProcessId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_079), sizeof msg_079));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message080) {
  static const unsigned char msg_080[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x90,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exportingProcessId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_080), sizeof msg_080));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message081) {
  static const unsigned char msg_081[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x91,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("templateId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_081), sizeof msg_081));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message082) {
  static const unsigned char msg_082[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x92,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("wlanChannelId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_082), sizeof msg_082));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message083) {
  static const unsigned char msg_083[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x94,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_083), sizeof msg_083));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message084) {
  static const unsigned char msg_084[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x95,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("observationDomainId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_084), sizeof msg_084));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message085) {
  static const unsigned char msg_085[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x96,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowStartSeconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_085), sizeof msg_085));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message086) {
  static const unsigned char msg_086[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x97,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowEndSeconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_086), sizeof msg_086));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message087) {
  static const unsigned char msg_087[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x98,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x1f,0x7c  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowStartMilliseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_087), sizeof msg_087));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x00000138b4671f7cULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message088) {
  static const unsigned char msg_088[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x99,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x20,0x76  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowEndMilliseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_088), sizeof msg_088));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x00000138b4672076ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message089) {
  static const unsigned char msg_089[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0x7f,0xff,0xff,0xff  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowStartMicroseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_089), sizeof msg_089));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x500d6a457fffffffULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message090) {
  static const unsigned char msg_090[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9b,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowEndMicroseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_090), sizeof msg_090));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message091) {
  static const unsigned char msg_091[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9c,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0x7f,0xff,0xff,0xff  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowStartNanoseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_091), sizeof msg_091));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x500d6a457fffffffULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message092) {
  static const unsigned char msg_092[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9d,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowEndNanoseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_092), sizeof msg_092));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message093) {
  static const unsigned char msg_093[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9e,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowStartDeltaMicroseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_093), sizeof msg_093));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message094) {
  static const unsigned char msg_094[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9f,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowEndDeltaMicroseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_094), sizeof msg_094));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message095) {
  static const unsigned char msg_095[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa0,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x20,0x76  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("systemInitTimeMilliseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_095), sizeof msg_095));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x00000138b4672076ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message096) {
  static const unsigned char msg_096[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa1,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowDurationMilliseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_096), sizeof msg_096));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message097) {
  static const unsigned char msg_097[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa2,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowDurationMicroseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_097), sizeof msg_097));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message098) {
  static const unsigned char msg_098[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa3,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("observedFlowTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_098), sizeof msg_098));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message099) {
  static const unsigned char msg_099[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa4,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ignoredPacketTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_099), sizeof msg_099));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message100) {
  static const unsigned char msg_100[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa5,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ignoredOctetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_100), sizeof msg_100));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message101) {
  static const unsigned char msg_101[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa6,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("notSentFlowTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_101), sizeof msg_101));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message102) {
  static const unsigned char msg_102[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("notSentPacketTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_102), sizeof msg_102));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message103) {
  static const unsigned char msg_103[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa8,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("notSentOctetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_103), sizeof msg_103));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message104) {
  static const unsigned char msg_104[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa9,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("destinationIPv6Prefix"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_104), sizeof msg_104));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message105) {
  static const unsigned char msg_105[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xaa,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceIPv6Prefix"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_105), sizeof msg_105));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message106) {
  static const unsigned char msg_106[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xab,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postOctetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_106), sizeof msg_106));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message107) {
  static const unsigned char msg_107[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xac,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postPacketTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_107), sizeof msg_107));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message108) {
  static const unsigned char msg_108[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xad,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("flowKeyIndicator"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_108), sizeof msg_108));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message109) {
  static const unsigned char msg_109[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xae,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postMCastPacketTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_109), sizeof msg_109));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message110) {
  static const unsigned char msg_110[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xaf,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postMCastOctetTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_110), sizeof msg_110));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message111) {
  static const unsigned char msg_111[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("icmpTypeIPv4"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_111), sizeof msg_111));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message112) {
  static const unsigned char msg_112[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb1,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("icmpCodeIPv4"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_112), sizeof msg_112));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message113) {
  static const unsigned char msg_113[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb2,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("icmpTypeIPv6"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_113), sizeof msg_113));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message114) {
  static const unsigned char msg_114[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb3,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("icmpCodeIPv6"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_114), sizeof msg_114));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message115) {
  static const unsigned char msg_115[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb4,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("udpSourcePort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_115), sizeof msg_115));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message116) {
  static const unsigned char msg_116[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb5,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("udpDestinationPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_116), sizeof msg_116));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message117) {
  static const unsigned char msg_117[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb6,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpSourcePort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_117), sizeof msg_117));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message118) {
  static const unsigned char msg_118[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb7,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpDestinationPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_118), sizeof msg_118));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message119) {
  static const unsigned char msg_119[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb8,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpSequenceNumber"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_119), sizeof msg_119));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message120) {
  static const unsigned char msg_120[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpAcknowledgementNumber"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_120), sizeof msg_120));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message121) {
  static const unsigned char msg_121[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xba,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpWindowSize"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_121), sizeof msg_121));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message122) {
  static const unsigned char msg_122[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbb,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpUrgentPointer"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_122), sizeof msg_122));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message123) {
  static const unsigned char msg_123[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbc,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpHeaderLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_123), sizeof msg_123));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message124) {
  static const unsigned char msg_124[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbd,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipHeaderLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_124), sizeof msg_124));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message125) {
  static const unsigned char msg_125[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbe,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("totalLengthIPv4"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_125), sizeof msg_125));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message126) {
  static const unsigned char msg_126[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbf,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("payloadLengthIPv6"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_126), sizeof msg_126));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message127) {
  static const unsigned char msg_127[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipTTL"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_127), sizeof msg_127));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message128) {
  static const unsigned char msg_128[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc1,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("nextHeaderIPv6"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_128), sizeof msg_128));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message129) {
  static const unsigned char msg_129[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc2,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsPayloadLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_129), sizeof msg_129));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message130) {
  static const unsigned char msg_130[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc3,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipDiffServCodePoint"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_130), sizeof msg_130));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message131) {
  static const unsigned char msg_131[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc4,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipPrecedence"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_131), sizeof msg_131));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message132) {
  static const unsigned char msg_132[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc5,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("fragmentFlags"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_132), sizeof msg_132));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message133) {
  static const unsigned char msg_133[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc6,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("octetDeltaSumOfSquares"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_133), sizeof msg_133));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message134) {
  static const unsigned char msg_134[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("octetTotalSumOfSquares"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_134), sizeof msg_134));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message135) {
  static const unsigned char msg_135[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc8,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsTopLabelTTL"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_135), sizeof msg_135));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message136) {
  static const unsigned char msg_136[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsLabelStackLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_136), sizeof msg_136));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message137) {
  static const unsigned char msg_137[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xca,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsLabelStackDepth"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_137), sizeof msg_137));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message138) {
  static const unsigned char msg_138[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcb,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("mplsTopLabelExp"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_138), sizeof msg_138));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message139) {
  static const unsigned char msg_139[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcc,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipPayloadLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_139), sizeof msg_139));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message140) {
  static const unsigned char msg_140[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcd,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("udpMessageLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_140), sizeof msg_140));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message141) {
  static const unsigned char msg_141[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xce,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("isMulticast"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_141), sizeof msg_141));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message142) {
  static const unsigned char msg_142[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcf,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipv4IHL"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_142), sizeof msg_142));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message143) {
  static const unsigned char msg_143[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd0,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipv4Options"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_143), sizeof msg_143));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message144) {
  static const unsigned char msg_144[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd1,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpOptions"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_144), sizeof msg_144));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message145) {
  static const unsigned char msg_145[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd3,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("collectorIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_145), sizeof msg_145));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message146) {
  static const unsigned char msg_146[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd4,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("collectorIPv6Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_146), sizeof msg_146));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      MyIp6Address value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message147) {
  static const unsigned char msg_147[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd5,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exportInterface"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_147), sizeof msg_147));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message148) {
  static const unsigned char msg_148[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exportProtocolVersion"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_148), sizeof msg_148));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message149) {
  static const unsigned char msg_149[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd7,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exportTransportProtocol"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_149), sizeof msg_149));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message150) {
  static const unsigned char msg_150[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd8,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("collectorTransportPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_150), sizeof msg_150));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message151) {
  static const unsigned char msg_151[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd9,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("exporterTransportPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_151), sizeof msg_151));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message152) {
  static const unsigned char msg_152[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xda,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpSynTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_152), sizeof msg_152));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message153) {
  static const unsigned char msg_153[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdb,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpFinTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_153), sizeof msg_153));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message154) {
  static const unsigned char msg_154[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdc,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpRstTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_154), sizeof msg_154));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message155) {
  static const unsigned char msg_155[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdd,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpPshTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_155), sizeof msg_155));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message156) {
  static const unsigned char msg_156[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xde,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpAckTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_156), sizeof msg_156));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message157) {
  static const unsigned char msg_157[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdf,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpUrgTotalCount"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_157), sizeof msg_157));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message158) {
  static const unsigned char msg_158[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe0,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ipTotalLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_158), sizeof msg_158));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message159) {
  static const unsigned char msg_159[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe1,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postNATSourceIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_159), sizeof msg_159));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message160) {
  static const unsigned char msg_160[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe2,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postNATDestinationIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_160), sizeof msg_160));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message161) {
  static const unsigned char msg_161[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe3,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postNAPTSourceTransportPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_161), sizeof msg_161));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message162) {
  static const unsigned char msg_162[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe4,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postNAPTDestinationTransportPort"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_162), sizeof msg_162));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message163) {
  static const unsigned char msg_163[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe5,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("natOriginatingAddressRealm"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_163), sizeof msg_163));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message164) {
  static const unsigned char msg_164[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("natEvent"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_164), sizeof msg_164));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message165) {
  static const unsigned char msg_165[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("initiatorOctets"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_165), sizeof msg_165));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message166) {
  static const unsigned char msg_166[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe8,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("responderOctets"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_166), sizeof msg_166));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message167) {
  static const unsigned char msg_167[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe9,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("firewallEvent"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_167), sizeof msg_167));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message168) {
  static const unsigned char msg_168[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xea,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ingressVRFID"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_168), sizeof msg_168));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message169) {
  static const unsigned char msg_169[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xeb,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("egressVRFID"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_169), sizeof msg_169));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message170) {
  static const unsigned char msg_170[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xed,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postMplsTopLabelExp"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_170), sizeof msg_170));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message171) {
  static const unsigned char msg_171[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xee,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("tcpWindowScale"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_171), sizeof msg_171));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message172) {
  static const unsigned char msg_172[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xef,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("biflowDirection"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_172), sizeof msg_172));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message173) {
  static const unsigned char msg_173[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ethernetHeaderLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_173), sizeof msg_173));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message174) {
  static const unsigned char msg_174[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf1,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ethernetPayloadLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_174), sizeof msg_174));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message175) {
  static const unsigned char msg_175[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf2,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ethernetTotalLength"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_175), sizeof msg_175));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message176) {
  static const unsigned char msg_176[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf3,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("dot1qVlanId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_176), sizeof msg_176));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message177) {
  static const unsigned char msg_177[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf4,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("dot1qPriority"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_177), sizeof msg_177));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message178) {
  static const unsigned char msg_178[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf5,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("dot1qCustomerVlanId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_178), sizeof msg_178));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message179) {
  static const unsigned char msg_179[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("dot1qCustomerPriority"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_179), sizeof msg_179));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message180) {
  static const unsigned char msg_180[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf8,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("metroEvcType"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_180), sizeof msg_180));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message181) {
  static const unsigned char msg_181[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("pseudoWireId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_181), sizeof msg_181));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message182) {
  static const unsigned char msg_182[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfa,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("pseudoWireType"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_182), sizeof msg_182));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message183) {
  static const unsigned char msg_183[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfb,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("pseudoWireControlWord"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_183), sizeof msg_183));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message184) {
  static const unsigned char msg_184[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfc,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ingressPhysicalInterface"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_184), sizeof msg_184));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message185) {
  static const unsigned char msg_185[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfd,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("egressPhysicalInterface"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_185), sizeof msg_185));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message186) {
  static const unsigned char msg_186[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfe,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postDot1qVlanId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_186), sizeof msg_186));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message187) {
  static const unsigned char msg_187[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xff,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postDot1qCustomerVlanId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_187), sizeof msg_187));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message188) {
  static const unsigned char msg_188[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x00,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("ethernetType"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_188), sizeof msg_188));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message189) {
  static const unsigned char msg_189[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x01,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("postIpPrecedence"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_189), sizeof msg_189));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message190) {
  static const unsigned char msg_190[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2d,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("selectionSequenceId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_190), sizeof msg_190));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message191) {
  static const unsigned char msg_191[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2e,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("selectorId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_191), sizeof msg_191));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message192) {
  static const unsigned char msg_192[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2f,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("informationElementId"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_192), sizeof msg_192));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message193) {
  static const unsigned char msg_193[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x30,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("selectorAlgorithm"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_193), sizeof msg_193));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message194) {
  static const unsigned char msg_194[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x31,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingPacketInterval"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_194), sizeof msg_194));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message195) {
  static const unsigned char msg_195[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x32,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingPacketSpace"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_195), sizeof msg_195));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message196) {
  static const unsigned char msg_196[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x33,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingTimeInterval"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_196), sizeof msg_196));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message197) {
  static const unsigned char msg_197[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x34,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingTimeSpace"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_197), sizeof msg_197));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message198) {
  static const unsigned char msg_198[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x35,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingSize"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_198), sizeof msg_198));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message199) {
  static const unsigned char msg_199[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x36,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingPopulation"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_199), sizeof msg_199));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message200) {
  static const unsigned char msg_200[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x37,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingProbability"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_200), sizeof msg_200));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      double value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message201) {
  static const unsigned char msg_201[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3e,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("selectorIdTotalPktsObserved"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_201), sizeof msg_201));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message202) {
  static const unsigned char msg_202[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3f,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("selectorIdTotalPktsSelected"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_202), sizeof msg_202));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message203) {
  static const unsigned char msg_203[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x40,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("absoluteError"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_203), sizeof msg_203));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      double value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message204) {
  static const unsigned char msg_204[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x41,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("relativeError"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_204), sizeof msg_204));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      double value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message205) {
  static const unsigned char msg_205[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x42,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("observationTimeSeconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_205), sizeof msg_205));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message206) {
  static const unsigned char msg_206[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x43,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x1f,0x7c  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("observationTimeMilliseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_206), sizeof msg_206));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x00000138b4671f7cULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message207) {
  static const unsigned char msg_207[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x44,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("observationTimeMicroseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_207), sizeof msg_207));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message208) {
  static const unsigned char msg_208[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x45,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("observationTimeNanoseconds"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_208), sizeof msg_208));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message209) {
  static const unsigned char msg_209[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x46,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("digestHashValue"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_209), sizeof msg_209));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message210) {
  static const unsigned char msg_210[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x47,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashIPPayloadOffset"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_210), sizeof msg_210));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message211) {
  static const unsigned char msg_211[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x48,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashIPPayloadSize"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_211), sizeof msg_211));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message212) {
  static const unsigned char msg_212[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x49,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashOutputRangeMin"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_212), sizeof msg_212));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message213) {
  static const unsigned char msg_213[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashOutputRangeMax"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_213), sizeof msg_213));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message214) {
  static const unsigned char msg_214[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4b,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashSelectedRangeMin"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_214), sizeof msg_214));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message215) {
  static const unsigned char msg_215[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4c,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashSelectedRangeMax"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_215), sizeof msg_215));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message216) {
  static const unsigned char msg_216[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4d,0x00,0x01,0x03,0xe9,0x00,0x05,0x01  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashDigestOutput"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_216), sizeof msg_216));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message217) {
  static const unsigned char msg_217[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4e,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("hashInitialiserValue"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_217), sizeof msg_217));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message218) {
  static const unsigned char msg_218[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x50,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("upperCILimit"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_218), sizeof msg_218));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      double value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message219) {
  static const unsigned char msg_219[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x51,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("lowerCILimit"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_219), sizeof msg_219));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      double value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message220) {
  static const unsigned char msg_220[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x52,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("confidenceLevel"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_220), sizeof msg_220));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      double value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message221) {
  static const unsigned char msg_221[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x53,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("informationElementDataType"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_221), sizeof msg_221));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message222) {
  static const unsigned char msg_222[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x56,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("informationElementRangeBegin"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_222), sizeof msg_222));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message223) {
  static const unsigned char msg_223[] = {
    0x00,0x0a,0x00,0x28,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x57,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("informationElementRangeEnd"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_223), sizeof msg_223));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint64_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message224) {
  static const unsigned char msg_224[] = {
    0x00,0x0a,0x00,0x21,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x58,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("informationElementSemantics"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_224), sizeof msg_224));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint8_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message225) {
  static const unsigned char msg_225[] = {
    0x00,0x0a,0x00,0x22,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x59,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("informationElementUnits"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_225), sizeof msg_225));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint16_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message226) {
  static const unsigned char msg_226[] = {
    0x00,0x0a,0x00,0x24,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x5a,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("privateEnterpriseNumber"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_226), sizeof msg_226));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Compound, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message227) {
  static const unsigned char msg_227[] = {
    0x00,0x0a,0x00,0x2c,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x10,0x03,0xe9,0x00,0x02,0x00,0x08,0x00,0x04,0x00,0x0c,0x00,0x04,0x03,0xe9,0x00,0x0c,0x7f,0x00,0x00,0x01,0x7f,0x00,0x00,0x02  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("sourceIPv4Address"),
    IPFIX::InfoModel::instance().lookupIE("destinationIPv4Address"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_227), sizeof msg_227));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    BOOST_REQUIRE(set_tmpl->contains(ies[1]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[1]),
                  set_tmpl->ieFor(ies[1]));
      BOOST_CHECK_EQUAL(value, 0x7f000002);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message228) {
  static const unsigned char msg_228[] = {
    0x00,0x0a,0x00,0x30,0x50,0x0e,0x60,0x54,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x10,0x03,0xe9,0x00,0x02,0x01,0x36,0x00,0x04,0x01,0x37,0x00,0x08,0x03,0xe9,0x00,0x10,0x10,0x20,0x30,0x40,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  std::vector<const IPFIX::InfoElement*> ies = {
    IPFIX::InfoModel::instance().lookupIE("samplingPopulation"),
    IPFIX::InfoModel::instance().lookupIE("samplingProbability"),
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg_228), sizeof msg_228));

  BOOST_CHECK(mbuf.deframe(s, session));

  IPFIX::Transcoder xc;
  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    n_set_lists++;
    const IPFIX::WireTemplate* set_tmpl = 
          session.getTemplate(mbuf.domain(), i->id);

    BOOST_REQUIRE(set_tmpl->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(set_tmpl, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    BOOST_REQUIRE(set_tmpl->contains(ies[0]));
    {
      uint32_t value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[0]),
                  set_tmpl->ieFor(ies[0]));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    BOOST_REQUIRE(set_tmpl->contains(ies[1]));
    {
      double value;
      xc.decodeAt(&value, sizeof(value), oc.offsetOf(ies[1]),
                  set_tmpl->ieFor(ies[1]));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  BOOST_CHECK_EQUAL(n_set_lists, 1);
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
