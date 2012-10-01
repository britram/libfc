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

#include <cstdarg>
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

class MyIp6Address;
static std::ostream& operator<<(std::ostream& os, const MyIp6Address& obj);

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

static std::ostream& operator<<(std::ostream& os, const MyIp6Address& obj) 
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

class Varlen;
static std::ostream& operator<<(std::ostream& os, const Varlen& obj);

class Varlen {
public:
  Varlen(size_t size, const char* s) : size(size), buf(new unsigned char[size]) {
    if (s != 0)
      memcpy(buf, s, size);
  }
  ~Varlen() { delete[] buf; }

  bool operator==(const Varlen& rhs) const {
    return size == rhs.size
      && memcmp(buf, rhs.buf, size) == 0;
  }

  void* ptr() {
    return buf;
  }

  friend std::ostream& operator<<(std::ostream& os, const Varlen& obj);

private:
  size_t size;
  unsigned char* buf;
};

static std::ostream& operator<<(std::ostream& os, const Varlen& obj) 
{ 
  for (unsigned int i = 0; i < obj.size; i++) {
    if (i > 0)
      os << ',';
    os << std::hex << obj.buf[i];
  }
  return os;
} 

static void
prepare_test_case(const unsigned char* msg, size_t len, 
                  IPFIX::MBuf& mbuf, IPFIX::Session& session,
                  IPFIX::Transcoder& xc) {
  std::istringstream s(std::string(reinterpret_cast<const char*>(msg), len));

  BOOST_CHECK(mbuf.deframe(s, session));

  mbuf.transcodeBy(xc);

  unsigned int n_set_lists = 0;
  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i)
    n_set_lists++;

  BOOST_REQUIRE(n_set_lists == 1);
}

BOOST_AUTO_TEST_SUITE(Messages)

BOOST_FIXTURE_TEST_SUITE(IETypes, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message001) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message002) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x01);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message003) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xfe  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xfe);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message004) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xff);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message005) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message006) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x5a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message007) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0000);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message008) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message009) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0xff,0xfe  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xfffe);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message010) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xffff);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message011) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0x00,0x10  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0010);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message012) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000000);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message013) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x00,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message014) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0xff,0xff,0xff,0xfe  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xfffffffe);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message015) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0xff,0xff,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xffffffff);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message016) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x5a,0xa5,0x69,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x5aa56996);
    }
    xc.defocus();
  }
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(InformationElements, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message017) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x01,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("octetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message018) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x02,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("packetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message019) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message020) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x05,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipClassOfService");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message021) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x06,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpControlBits");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message022) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message023) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x08,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message024) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x09,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceIPv4PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message025) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message026) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0b,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message027) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0c,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message028) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationIPv4PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message029) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0e,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("egressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message030) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0f,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipNextHopIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message031) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x10,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("bgpSourceAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message032) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x11,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("bgpDestinationAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message033) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x12,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("bgpNextHopIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message034) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x13,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postMCastPacketDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message035) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x14,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postMCastOctetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message036) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x15,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowEndSysUpTime");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message037) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x16,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowStartSysUpTime");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message038) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x17,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postOctetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message039) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x18,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postPacketDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message040) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x19,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("minimumIpTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message041) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("maximumIpTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message042) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1b,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message043) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1c,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message044) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceIPv6PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message045) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1e,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationIPv6PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message046) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1f,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowLabelIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message047) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x20,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("icmpTypeCodeIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message048) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x21,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("igmpType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message049) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x24,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowActiveTimeout");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message050) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x25,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowIdleTimeout");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message051) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x28,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exportedOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message052) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x29,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exportedMessageTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message053) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exportedFlowRecordTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message054) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2c,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceIPv4Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message055) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2d,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationIPv4Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message056) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2e,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsTopLabelType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message057) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2f,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsTopLabelIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message058) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x34,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("minimumTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message059) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x35,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("maximumTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message060) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x36,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("fragmentIdentification");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message061) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x37,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postIpClassOfService");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message062) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3a,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("vlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message063) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3b,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message064) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3c,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipVersion");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message065) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowDirection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message066) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3e,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipNextHopIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message067) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3f,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("bgpNextHopIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message068) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x40,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipv6ExtensionHeaders");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message069) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x46,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsTopLabelStackSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message070) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x47,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection2");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message071) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x48,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection3");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message072) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x49,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message073) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4a,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection5");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message074) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4b,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message075) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4c,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection7");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message076) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4d,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection8");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message077) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4e,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection9");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message078) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4f,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection10");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message079) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x25,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x52,0xff,0xff,0x03,0xe9,0x00,0x09,0x04,0x65,0x74,0x68,0x30  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(4, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("interfaceName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 4, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(4, "eth0"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message080) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x39,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x1d,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(24, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("interfaceDescription");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 24, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(24, "First ethernet interface"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message081) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x55,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("octetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message082) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x56,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("packetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message083) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x58,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("fragmentOffset");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message084) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x5a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x12,0x34,0x56,0x78,0x13,0x24,0x35,0x46  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(8, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsVpnRouteDistinguisher");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 8, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(8, "\x12\x34\x56\x78\x13\x24\x35\x46"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message085) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x5b,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsTopLabelPrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message086) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x62,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postIpDiffServCodePoint");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message087) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x80,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("bgpNextAdjacentAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message088) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x81,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("bgpPrevAdjacentAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message089) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x82,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exporterIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message090) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x83,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exporterIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message091) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x84,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("droppedOctetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message092) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x85,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("droppedPacketDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message093) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x86,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("droppedOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message094) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x87,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("droppedPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message095) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x88,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowEndReason");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message096) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x89,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("commonPropertiesId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message097) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8a,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("observationPointId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message098) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8b,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("icmpTypeCodeIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message099) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8c,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsTopLabelIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message100) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8d,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("lineCardId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message101) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8e,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("portId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message102) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8f,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("meteringProcessId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message103) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x90,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exportingProcessId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message104) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x91,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("templateId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message105) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x92,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("wlanChannelId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message106) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x40,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x93,0x00,0x20,0x03,0xe9,0x00,0x24,0x46,0x72,0x69,0x74,0x7a,0x21,0x42,0x6f,0x78,0x20,0x31,0x32,0x33,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(14, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("wlanSSID");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 14, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(14, "Fritz!Box 1234"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message107) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x94,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message108) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x95,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("observationDomainId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message109) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x96,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowStartSeconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message110) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x97,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowEndSeconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message111) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x98,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x1f,0x7c  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowStartMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4671f7cULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message112) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x99,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x20,0x76  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowEndMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4672076ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message113) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0x7f,0xff,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowStartMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a457fffffffULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message114) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9b,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowEndMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message115) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9c,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0x7f,0xff,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowStartNanoseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a457fffffffULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message116) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9d,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowEndNanoseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message117) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9e,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowStartDeltaMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message118) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9f,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowEndDeltaMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message119) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa0,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x20,0x76  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("systemInitTimeMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4672076ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message120) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa1,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowDurationMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message121) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa2,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowDurationMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message122) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa3,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("observedFlowTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message123) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa4,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ignoredPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message124) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa5,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ignoredOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message125) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa6,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("notSentFlowTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message126) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("notSentPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message127) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa8,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("notSentOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message128) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa9,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationIPv6Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message129) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xaa,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceIPv6Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message130) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xab,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message131) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xac,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message132) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xad,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("flowKeyIndicator");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message133) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xae,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postMCastPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message134) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xaf,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postMCastOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message135) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("icmpTypeIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message136) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb1,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("icmpCodeIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message137) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb2,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("icmpTypeIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message138) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb3,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("icmpCodeIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message139) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb4,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("udpSourcePort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message140) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb5,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("udpDestinationPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message141) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb6,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpSourcePort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message142) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb7,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpDestinationPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message143) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb8,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpSequenceNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message144) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpAcknowledgementNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message145) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xba,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpWindowSize");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message146) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbb,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpUrgentPointer");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message147) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbc,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpHeaderLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message148) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbd,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipHeaderLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message149) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbe,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("totalLengthIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message150) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbf,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("payloadLengthIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message151) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message152) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc1,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("nextHeaderIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message153) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc2,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsPayloadLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message154) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc3,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipDiffServCodePoint");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message155) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc4,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipPrecedence");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message156) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc5,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("fragmentFlags");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message157) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc6,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("octetDeltaSumOfSquares");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message158) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("octetTotalSumOfSquares");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message159) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc8,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsTopLabelTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message160) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message161) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xca,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackDepth");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message162) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcb,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsTopLabelExp");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message163) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcc,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipPayloadLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message164) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcd,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("udpMessageLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message165) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xce,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("isMulticast");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message166) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcf,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipv4IHL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message167) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd0,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipv4Options");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message168) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd1,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpOptions");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message169) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd2,0x00,0x01,0x03,0xe9,0x00,0x05,0x12  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(1, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("paddingOctets");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 1, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(1, "\x12"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message170) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd3,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("collectorIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message171) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd4,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("collectorIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message172) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd5,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exportInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message173) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exportProtocolVersion");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message174) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd7,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exportTransportProtocol");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message175) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd8,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("collectorTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message176) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd9,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("exporterTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message177) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xda,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpSynTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message178) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdb,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpFinTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message179) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdc,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpRstTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message180) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdd,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpPshTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message181) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xde,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpAckTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message182) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdf,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpUrgTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message183) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe0,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message184) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe1,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postNATSourceIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message185) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe2,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postNATDestinationIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message186) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe3,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postNAPTSourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message187) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe4,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postNAPTDestinationTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message188) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe5,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("natOriginatingAddressRealm");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message189) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("natEvent");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message190) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("initiatorOctets");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message191) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe8,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("responderOctets");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message192) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe9,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("firewallEvent");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message193) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xea,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressVRFID");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message194) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xeb,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("egressVRFID");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message195) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xec,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x48,0x69,0x2d,0x48,0x6f  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("VRFname");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "Hi-Ho"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message196) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xed,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postMplsTopLabelExp");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message197) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xee,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("tcpWindowScale");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message198) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xef,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("biflowDirection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message199) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ethernetHeaderLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message200) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf1,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ethernetPayloadLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message201) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf2,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ethernetTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message202) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf3,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("dot1qVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message203) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf4,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("dot1qPriority");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message204) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf5,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("dot1qCustomerVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message205) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("dot1qCustomerPriority");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message206) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x84,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf7,0x00,0x64,0x03,0xe9,0x00,0x68,0x4e,0x6f,0x20,0x49,0x64,0x65,0x61,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(7, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("metroEvcId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 7, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(7, "No Idea"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message207) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf8,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("metroEvcType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message208) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("pseudoWireId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message209) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfa,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("pseudoWireType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message210) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfb,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("pseudoWireControlWord");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message211) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfc,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ingressPhysicalInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message212) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfd,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("egressPhysicalInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message213) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfe,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postDot1qVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message214) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xff,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postDot1qCustomerVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message215) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x00,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ethernetType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message216) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x01,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("postIpPrecedence");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message217) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2d,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("selectionSequenceId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message218) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2e,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("selectorId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message219) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2f,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message220) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x30,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("selectorAlgorithm");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message221) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x31,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingPacketInterval");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message222) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x32,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingPacketSpace");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message223) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x33,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingTimeInterval");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message224) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x34,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingTimeSpace");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message225) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x35,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingSize");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message226) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x36,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingPopulation");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message227) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x37,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingProbability");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message228) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x39,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipHeaderPacketSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message229) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3a,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("ipPayloadPacketSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message230) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3c,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsLabelStackSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message231) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3d,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("mplsPayloadPacketSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message232) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3e,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("selectorIdTotalPktsObserved");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message233) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3f,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("selectorIdTotalPktsSelected");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message234) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x40,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("absoluteError");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message235) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x41,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("relativeError");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message236) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x42,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("observationTimeSeconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message237) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x43,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x1f,0x7c  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("observationTimeMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4671f7cULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message238) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x44,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("observationTimeMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message239) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x45,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("observationTimeNanoseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message240) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x46,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("digestHashValue");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message241) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x47,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashIPPayloadOffset");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message242) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x48,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashIPPayloadSize");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message243) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x49,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashOutputRangeMin");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message244) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashOutputRangeMax");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message245) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4b,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashSelectedRangeMin");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message246) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4c,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashSelectedRangeMax");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message247) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4d,0x00,0x01,0x03,0xe9,0x00,0x05,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashDigestOutput");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message248) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4e,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("hashInitialiserValue");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message249) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4f,0xff,0xff,0x03,0xe9,0x00,0x0c,0x07,0x4e,0x6f,0x20,0x49,0x64,0x65,0x61  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(7, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("selectorName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 7, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(7, "No Idea"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message250) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x50,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("upperCILimit");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message251) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x51,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("lowerCILimit");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message252) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x52,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("confidenceLevel");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message253) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x53,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementDataType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message254) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x54,0xff,0xff,0x03,0xe9,0x00,0x14,0x0f,0x4d,0x79,0x20,0x76,0x65,0x72,0x79,0x20,0x6f,0x77,0x6e,0x20,0x49,0x45,0x21  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(15, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementDescription");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 15, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(15, "My very own IE!"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message255) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x55,0xff,0xff,0x03,0xe9,0x00,0x0c,0x07,0x6d,0x79,0x4f,0x77,0x6e,0x49,0x45  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(7, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 7, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(7, "myOwnIE"));
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message256) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x56,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementRangeBegin");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message257) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x57,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementRangeEnd");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message258) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x58,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementSemantics");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message259) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x59,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("informationElementUnits");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message260) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x5a,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("privateEnterpriseNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Compound, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message261) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x2c,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x10,0x03,0xe9,0x00,0x02,0x00,0x08,0x00,0x04,0x00,0x0c,0x00,0x04,0x03,0xe9,0x00,0x0c,0x7f,0x00,0x00,0x01,0x7f,0x00,0x00,0x02  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("sourceIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("destinationIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000002);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message262) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x10,0x03,0xe9,0x00,0x02,0x01,0x36,0x00,0x04,0x01,0x37,0x00,0x08,0x03,0xe9,0x00,0x10,0x10,0x20,0x30,0x40,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingPopulation");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingProbability");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }
}


/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message263) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x56,0x50,0x69,0x7c,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x18,0x03,0xe9,0x00,0x04,0x01,0x36,0x00,0x04,0x00,0x52,0xff,0xff,0x01,0x37,0x00,0x08,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x2e,0x10,0x20,0x30,0x40,0x04,0x65,0x74,0x68,0x30,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65  };

  IPFIX::MBuf mbuf;
  IPFIX::Session session;
  IPFIX::Transcoder xc;

  prepare_test_case(msg, sizeof(msg), mbuf, session, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = session.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingPopulation");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    {
      Varlen value(4, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("interfaceName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 4, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(4, "eth0"));
    }
    {
      double value;
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("samplingProbability");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    {
      Varlen value(24, 0);
      const IPFIX::InfoElement* ie 
        = IPFIX::InfoModel::instance().lookupIE("interfaceDescription");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 24, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(25, "Second ethernet interface"));
    }
    xc.defocus();
  }
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
