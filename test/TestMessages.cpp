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

#include "BufWriter.h"
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

  operator const void* () const {
    return buf;
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

static void
dump_comparison(const unsigned char* buf1, 
                const unsigned char* buf2, size_t len) {
  std::cerr << "off	me	ripfix" << std::endl;
  for (unsigned int i = 0; i < len; i++) {
    fprintf(stderr, "%02d	%02x	%02x", i, buf1[i], buf2[i]);
    if (buf1[i] != buf2[i]) 
      fprintf(stderr, " <-- ");
    std::cerr << std::endl;
  }
}

BOOST_AUTO_TEST_SUITE(Messages)

BOOST_FIXTURE_TEST_SUITE(IETypes, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message001) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("protocolIdentifier"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("protocolIdentifier"), static_cast<uint8_t>(0x00));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message002) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x01);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("protocolIdentifier"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("protocolIdentifier"), static_cast<uint8_t>(0x01));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message003) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xfe  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xfe);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("protocolIdentifier"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("protocolIdentifier"), static_cast<uint8_t>(0xfe));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message004) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xff);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("protocolIdentifier"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("protocolIdentifier"), static_cast<uint8_t>(0xff));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message005) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("protocolIdentifier"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("protocolIdentifier"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message006) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x5a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("protocolIdentifier"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("protocolIdentifier"), static_cast<uint8_t>(0x5a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message007) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0000);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceTransportPort"), static_cast<uint16_t>(0x0000));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message008) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceTransportPort"), static_cast<uint16_t>(0x0001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message009) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0xff,0xfe  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xfffe);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceTransportPort"), static_cast<uint16_t>(0xfffe));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message010) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xffff);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceTransportPort"), static_cast<uint16_t>(0xffff));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message011) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0x00,0x10  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0010);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceTransportPort"), static_cast<uint16_t>(0x0010));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message012) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000000);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressInterface"), static_cast<uint32_t>(0x00000000));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message013) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x00,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressInterface"), static_cast<uint32_t>(0x00000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message014) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0xff,0xff,0xff,0xfe  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xfffffffe);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressInterface"), static_cast<uint32_t>(0xfffffffe));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message015) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0xff,0xff,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xffffffff);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressInterface"), static_cast<uint32_t>(0xffffffff));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message016) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x5a,0xa5,0x69,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x5aa56996);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressInterface"), static_cast<uint32_t>(0x5aa56996));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(InformationElements, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message017) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x01,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("octetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("octetDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("octetDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message018) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x02,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("packetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("packetDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("packetDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message019) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x04,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("protocolIdentifier");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("protocolIdentifier"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("protocolIdentifier"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message020) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x05,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipClassOfService");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipClassOfService"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipClassOfService"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message021) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x06,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpControlBits");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpControlBits"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpControlBits"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message022) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x07,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceTransportPort"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message023) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x08,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message024) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x09,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceIPv4PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceIPv4PrefixLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceIPv4PrefixLength"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message025) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0a,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressInterface"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message026) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0b,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("destinationTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("destinationTransportPort"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message027) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0c,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("destinationIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("destinationIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message028) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationIPv4PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("destinationIPv4PrefixLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("destinationIPv4PrefixLength"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message029) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0e,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("egressInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("egressInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("egressInterface"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message030) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x0f,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipNextHopIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipNextHopIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipNextHopIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message031) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x10,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("bgpSourceAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("bgpSourceAsNumber"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("bgpSourceAsNumber"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message032) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x11,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("bgpDestinationAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("bgpDestinationAsNumber"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("bgpDestinationAsNumber"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message033) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x12,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("bgpNextHopIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("bgpNextHopIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("bgpNextHopIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message034) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x13,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postMCastPacketDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postMCastPacketDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postMCastPacketDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message035) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x14,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postMCastOctetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postMCastOctetDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postMCastOctetDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message036) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x15,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowEndSysUpTime");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowEndSysUpTime"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowEndSysUpTime"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message037) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x16,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowStartSysUpTime");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowStartSysUpTime"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowStartSysUpTime"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message038) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x17,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postOctetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postOctetDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postOctetDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message039) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x18,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postPacketDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postPacketDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postPacketDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message040) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x19,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("minimumIpTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("minimumIpTotalLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("minimumIpTotalLength"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message041) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("maximumIpTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("maximumIpTotalLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("maximumIpTotalLength"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message042) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1b,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceIPv6Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("sourceIPv6Address"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message043) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1c,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("destinationIPv6Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("destinationIPv6Address"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message044) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceIPv6PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceIPv6PrefixLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceIPv6PrefixLength"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message045) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1e,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationIPv6PrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("destinationIPv6PrefixLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("destinationIPv6PrefixLength"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message046) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x1f,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowLabelIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowLabelIPv6"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowLabelIPv6"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message047) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x20,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("icmpTypeCodeIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("icmpTypeCodeIPv4"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("icmpTypeCodeIPv4"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message048) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x21,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("igmpType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("igmpType"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("igmpType"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message049) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x24,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowActiveTimeout");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowActiveTimeout"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowActiveTimeout"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message050) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x25,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowIdleTimeout");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowIdleTimeout"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowIdleTimeout"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message051) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x28,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exportedOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exportedOctetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exportedOctetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message052) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x29,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exportedMessageTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exportedMessageTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exportedMessageTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message053) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exportedFlowRecordTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exportedFlowRecordTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exportedFlowRecordTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message054) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2c,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceIPv4Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceIPv4Prefix"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceIPv4Prefix"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message055) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2d,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationIPv4Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("destinationIPv4Prefix"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("destinationIPv4Prefix"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message056) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2e,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsTopLabelType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsTopLabelType"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsTopLabelType"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message057) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x2f,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsTopLabelIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsTopLabelIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsTopLabelIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message058) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x34,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("minimumTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("minimumTTL"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("minimumTTL"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message059) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x35,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("maximumTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("maximumTTL"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("maximumTTL"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message060) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x36,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("fragmentIdentification");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("fragmentIdentification"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("fragmentIdentification"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message061) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x37,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postIpClassOfService");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postIpClassOfService"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postIpClassOfService"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message062) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3a,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("vlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("vlanId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("vlanId"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message063) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3b,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postVlanId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postVlanId"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message064) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3c,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipVersion");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipVersion"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipVersion"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message065) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3d,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowDirection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowDirection"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowDirection"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message066) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3e,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipNextHopIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipNextHopIPv6Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("ipNextHopIPv6Address"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message067) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x3f,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("bgpNextHopIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("bgpNextHopIPv6Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("bgpNextHopIPv6Address"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message068) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x40,0x00,0x04,0x03,0xe9,0x00,0x08,0x11,0x22,0x33,0x44  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipv6ExtensionHeaders");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x11223344);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipv6ExtensionHeaders"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipv6ExtensionHeaders"), static_cast<uint32_t>(0x11223344));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message069) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x46,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsTopLabelStackSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsTopLabelStackSection"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsTopLabelStackSection"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsTopLabelStackSection"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message070) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x47,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection2");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection2"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection2"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection2"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message071) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x48,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection3");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection3"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection3"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection3"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message072) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x49,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection4"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection4"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection4"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message073) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4a,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection5");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection5"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection5"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection5"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message074) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4b,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection6"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection6"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection6"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message075) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4c,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection7");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection7"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection7"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection7"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message076) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4d,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection8");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection8"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection8"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection8"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message077) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4e,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection9");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection9"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection9"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection9"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message078) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x23,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x4f,0x00,0x03,0x03,0xe9,0x00,0x07,0x12,0x34,0x56  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(3, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection10");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 3, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(3, "\x12\x34\x56"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection10"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection10"), 3);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection10"), Varlen(3, "\x12\x34\x56"), 3);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message079) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x25,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x52,0xff,0xff,0x03,0xe9,0x00,0x09,0x04,0x65,0x74,0x68,0x30  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(4, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("interfaceName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 4, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(4, "eth0"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("interfaceName"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("interfaceName"), 4);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("interfaceName"), Varlen(4, "eth0"), 4);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message080) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x39,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x1d,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(24, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("interfaceDescription");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 24, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(24, "First ethernet interface"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("interfaceDescription"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("interfaceDescription"), 24);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("interfaceDescription"), Varlen(24, "First ethernet interface"), 24);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message081) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x55,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("octetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("octetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("octetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message082) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x56,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("packetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("packetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("packetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message083) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x58,0x00,0x02,0x03,0xe9,0x00,0x06,0xa5,0x5a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("fragmentOffset");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa55a);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("fragmentOffset"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("fragmentOffset"), static_cast<uint16_t>(0xa55a));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message084) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x5a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x12,0x34,0x56,0x78,0x13,0x24,0x35,0x46  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(8, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsVpnRouteDistinguisher");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 8, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(8, "\x12\x34\x56\x78\x13\x24\x35\x46"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsVpnRouteDistinguisher"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsVpnRouteDistinguisher"), 8);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsVpnRouteDistinguisher"), Varlen(8, "\x12\x34\x56\x78\x13\x24\x35\x46"), 8);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message085) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x5b,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsTopLabelPrefixLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsTopLabelPrefixLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsTopLabelPrefixLength"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message086) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x62,0x00,0x01,0x03,0xe9,0x00,0x05,0xa5  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postIpDiffServCodePoint");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0xa5);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postIpDiffServCodePoint"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postIpDiffServCodePoint"), static_cast<uint8_t>(0xa5));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message087) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x80,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("bgpNextAdjacentAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("bgpNextAdjacentAsNumber"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("bgpNextAdjacentAsNumber"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message088) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x81,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("bgpPrevAdjacentAsNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("bgpPrevAdjacentAsNumber"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("bgpPrevAdjacentAsNumber"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message089) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x82,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exporterIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exporterIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exporterIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message090) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x83,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("exporterIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exporterIPv6Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("exporterIPv6Address"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message091) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x84,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("droppedOctetDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("droppedOctetDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("droppedOctetDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message092) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x85,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("droppedPacketDeltaCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("droppedPacketDeltaCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("droppedPacketDeltaCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message093) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x86,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("droppedOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("droppedOctetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("droppedOctetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message094) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x87,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("droppedPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("droppedPacketTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("droppedPacketTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message095) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x88,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowEndReason");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowEndReason"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowEndReason"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message096) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x89,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("commonPropertiesId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("commonPropertiesId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("commonPropertiesId"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message097) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8a,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("observationPointId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("observationPointId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("observationPointId"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message098) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8b,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("icmpTypeCodeIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("icmpTypeCodeIPv6"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("icmpTypeCodeIPv6"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message099) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8c,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsTopLabelIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsTopLabelIPv6Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("mplsTopLabelIPv6Address"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message100) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8d,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("lineCardId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("lineCardId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("lineCardId"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message101) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8e,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("portId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("portId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("portId"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message102) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x8f,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("meteringProcessId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("meteringProcessId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("meteringProcessId"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message103) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x90,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exportingProcessId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exportingProcessId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exportingProcessId"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message104) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x91,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("templateId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("templateId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("templateId"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message105) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x92,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("wlanChannelId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("wlanChannelId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("wlanChannelId"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message106) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x40,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x93,0x00,0x20,0x03,0xe9,0x00,0x24,0x46,0x72,0x69,0x74,0x7a,0x21,0x42,0x6f,0x78,0x20,0x31,0x32,0x33,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(14, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("wlanSSID");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 14, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(14, "Fritz!Box 1234"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("wlanSSID"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("wlanSSID"), 14);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("wlanSSID"), Varlen(14, "Fritz!Box 1234"), 14);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message107) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x94,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowId"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message108) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x95,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("observationDomainId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("observationDomainId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("observationDomainId"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message109) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x96,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowStartSeconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowStartSeconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowStartSeconds"), static_cast<uint32_t>(0x12345678));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message110) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x97,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowEndSeconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowEndSeconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowEndSeconds"), static_cast<uint32_t>(0x12345678));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message111) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x98,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x1f,0x7c  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowStartMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4671f7cULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowStartMilliseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowStartMilliseconds"), static_cast<uint64_t>(0x00000138b4671f7cULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message112) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x99,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x20,0x76  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowEndMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4672076ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowEndMilliseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowEndMilliseconds"), static_cast<uint64_t>(0x00000138b4672076ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message113) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0x7f,0xff,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowStartMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a457fffffffULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowStartMicroseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowStartMicroseconds"), static_cast<uint64_t>(0x500d6a457fffffffULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message114) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9b,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowEndMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowEndMicroseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowEndMicroseconds"), static_cast<uint64_t>(0x500d6a45c00003ffULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message115) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9c,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0x7f,0xff,0xff,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowStartNanoseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a457fffffffULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowStartNanoseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowStartNanoseconds"), static_cast<uint64_t>(0x500d6a457fffffffULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message116) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9d,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowEndNanoseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowEndNanoseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowEndNanoseconds"), static_cast<uint64_t>(0x500d6a45c00003ffULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message117) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9e,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowStartDeltaMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowStartDeltaMicroseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowStartDeltaMicroseconds"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message118) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0x9f,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowEndDeltaMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowEndDeltaMicroseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowEndDeltaMicroseconds"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message119) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa0,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x20,0x76  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("systemInitTimeMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4672076ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("systemInitTimeMilliseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("systemInitTimeMilliseconds"), static_cast<uint64_t>(0x00000138b4672076ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message120) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa1,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowDurationMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowDurationMilliseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowDurationMilliseconds"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message121) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa2,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowDurationMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowDurationMicroseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowDurationMicroseconds"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message122) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa3,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("observedFlowTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("observedFlowTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("observedFlowTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message123) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa4,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ignoredPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ignoredPacketTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ignoredPacketTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message124) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa5,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ignoredOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ignoredOctetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ignoredOctetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message125) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa6,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("notSentFlowTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("notSentFlowTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("notSentFlowTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message126) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("notSentPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("notSentPacketTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("notSentPacketTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message127) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa8,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("notSentOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("notSentOctetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("notSentOctetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message128) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xa9,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationIPv6Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("destinationIPv6Prefix"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("destinationIPv6Prefix"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message129) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xaa,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceIPv6Prefix");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceIPv6Prefix"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("sourceIPv6Prefix"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message130) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xab,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postOctetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postOctetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message131) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xac,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postPacketTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postPacketTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message132) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xad,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("flowKeyIndicator");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("flowKeyIndicator"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("flowKeyIndicator"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message133) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xae,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postMCastPacketTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postMCastPacketTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postMCastPacketTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message134) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xaf,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postMCastOctetTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postMCastOctetTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postMCastOctetTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message135) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("icmpTypeIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("icmpTypeIPv4"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("icmpTypeIPv4"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message136) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb1,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("icmpCodeIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("icmpCodeIPv4"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("icmpCodeIPv4"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message137) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb2,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("icmpTypeIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("icmpTypeIPv6"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("icmpTypeIPv6"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message138) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb3,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("icmpCodeIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("icmpCodeIPv6"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("icmpCodeIPv6"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message139) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb4,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("udpSourcePort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("udpSourcePort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("udpSourcePort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message140) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb5,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("udpDestinationPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("udpDestinationPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("udpDestinationPort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message141) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb6,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpSourcePort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpSourcePort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpSourcePort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message142) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb7,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpDestinationPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpDestinationPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpDestinationPort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message143) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb8,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpSequenceNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpSequenceNumber"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpSequenceNumber"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message144) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xb9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpAcknowledgementNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpAcknowledgementNumber"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpAcknowledgementNumber"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message145) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xba,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpWindowSize");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpWindowSize"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpWindowSize"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message146) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbb,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpUrgentPointer");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpUrgentPointer"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpUrgentPointer"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message147) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbc,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpHeaderLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpHeaderLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpHeaderLength"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message148) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbd,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipHeaderLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipHeaderLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipHeaderLength"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message149) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbe,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("totalLengthIPv4");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("totalLengthIPv4"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("totalLengthIPv4"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message150) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xbf,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("payloadLengthIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("payloadLengthIPv6"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("payloadLengthIPv6"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message151) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipTTL"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipTTL"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message152) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc1,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("nextHeaderIPv6");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("nextHeaderIPv6"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("nextHeaderIPv6"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message153) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc2,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsPayloadLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsPayloadLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsPayloadLength"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message154) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc3,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipDiffServCodePoint");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipDiffServCodePoint"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipDiffServCodePoint"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message155) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc4,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipPrecedence");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipPrecedence"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipPrecedence"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message156) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc5,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("fragmentFlags");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("fragmentFlags"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("fragmentFlags"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message157) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc6,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("octetDeltaSumOfSquares");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("octetDeltaSumOfSquares"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("octetDeltaSumOfSquares"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message158) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("octetTotalSumOfSquares");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("octetTotalSumOfSquares"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("octetTotalSumOfSquares"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message159) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc8,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsTopLabelTTL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsTopLabelTTL"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsTopLabelTTL"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message160) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xc9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsLabelStackLength"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message161) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xca,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackDepth");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackDepth"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsLabelStackDepth"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message162) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcb,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("mplsTopLabelExp");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsTopLabelExp"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("mplsTopLabelExp"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message163) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcc,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipPayloadLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipPayloadLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipPayloadLength"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message164) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcd,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("udpMessageLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("udpMessageLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("udpMessageLength"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message165) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xce,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("isMulticast");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("isMulticast"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("isMulticast"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message166) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xcf,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipv4IHL");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipv4IHL"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipv4IHL"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message167) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd0,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipv4Options");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipv4Options"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipv4Options"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message168) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd1,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpOptions");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpOptions"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpOptions"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message169) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd2,0x00,0x01,0x03,0xe9,0x00,0x05,0x12  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(1, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("paddingOctets");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 1, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(1, "\x12"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("paddingOctets"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("paddingOctets"), 1);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("paddingOctets"), Varlen(1, "\x12"), 1);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message170) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd3,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("collectorIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("collectorIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("collectorIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message171) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd4,0x00,0x10,0x03,0xe9,0x00,0x14,0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x00,0x00,0x05,0x06,0x00,0x00,0x07,0x08,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      MyIp6Address value;
      const IPFIX::InfoElement* ie = model.lookupIE("collectorIPv6Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("collectorIPv6Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    MyIp6Address a = MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800);
    buf_writer.putValue(model.lookupIE("collectorIPv6Address"), &a, sizeof a);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message172) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd5,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exportInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exportInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exportInterface"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message173) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exportProtocolVersion");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exportProtocolVersion"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exportProtocolVersion"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message174) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd7,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exportTransportProtocol");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exportTransportProtocol"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exportTransportProtocol"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message175) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd8,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("collectorTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("collectorTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("collectorTransportPort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message176) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xd9,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("exporterTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("exporterTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("exporterTransportPort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message177) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xda,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpSynTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpSynTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpSynTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message178) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdb,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpFinTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpFinTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpFinTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message179) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdc,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpRstTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpRstTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpRstTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message180) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdd,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpPshTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpPshTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpPshTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message181) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xde,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpAckTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpAckTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpAckTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message182) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xdf,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpUrgTotalCount");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpUrgTotalCount"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpUrgTotalCount"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message183) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe0,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ipTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipTotalLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ipTotalLength"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message184) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe1,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postNATSourceIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postNATSourceIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postNATSourceIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message185) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe2,0x00,0x04,0x03,0xe9,0x00,0x08,0x7f,0x00,0x00,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postNATDestinationIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postNATDestinationIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postNATDestinationIPv4Address"), static_cast<uint32_t>(0x7f000001));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message186) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe3,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postNAPTSourceTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postNAPTSourceTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postNAPTSourceTransportPort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message187) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe4,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postNAPTDestinationTransportPort");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postNAPTDestinationTransportPort"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postNAPTDestinationTransportPort"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message188) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe5,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("natOriginatingAddressRealm");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("natOriginatingAddressRealm"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("natOriginatingAddressRealm"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message189) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("natEvent");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("natEvent"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("natEvent"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message190) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe7,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("initiatorOctets");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("initiatorOctets"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("initiatorOctets"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message191) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe8,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("responderOctets");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("responderOctets"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("responderOctets"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message192) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xe9,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("firewallEvent");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("firewallEvent"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("firewallEvent"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message193) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xea,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressVRFID");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressVRFID"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressVRFID"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message194) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xeb,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("egressVRFID");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("egressVRFID"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("egressVRFID"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message195) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xec,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x48,0x69,0x2d,0x48,0x6f  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("VRFname");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "Hi-Ho"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("VRFname"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("VRFname"), 5);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("VRFname"), Varlen(5, "Hi-Ho"), 5);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message196) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xed,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postMplsTopLabelExp");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postMplsTopLabelExp"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postMplsTopLabelExp"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message197) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xee,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("tcpWindowScale");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("tcpWindowScale"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("tcpWindowScale"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message198) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xef,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("biflowDirection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("biflowDirection"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("biflowDirection"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message199) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf0,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ethernetHeaderLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ethernetHeaderLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ethernetHeaderLength"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message200) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf1,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ethernetPayloadLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ethernetPayloadLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ethernetPayloadLength"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message201) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf2,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ethernetTotalLength");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ethernetTotalLength"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ethernetTotalLength"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message202) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf3,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("dot1qVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("dot1qVlanId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("dot1qVlanId"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message203) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf4,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("dot1qPriority");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("dot1qPriority"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("dot1qPriority"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message204) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf5,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("dot1qCustomerVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("dot1qCustomerVlanId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("dot1qCustomerVlanId"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message205) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf6,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("dot1qCustomerPriority");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("dot1qCustomerPriority"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("dot1qCustomerPriority"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message206) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x84,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf7,0x00,0x64,0x03,0xe9,0x00,0x68,0x4e,0x6f,0x20,0x49,0x64,0x65,0x61,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(7, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("metroEvcId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 7, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(7, "No Idea"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("metroEvcId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("metroEvcId"), 7);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("metroEvcId"), Varlen(7, "No Idea"), 7);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message207) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf8,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("metroEvcType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("metroEvcType"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("metroEvcType"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message208) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xf9,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("pseudoWireId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("pseudoWireId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("pseudoWireId"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message209) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfa,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("pseudoWireType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("pseudoWireType"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("pseudoWireType"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message210) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfb,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("pseudoWireControlWord");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("pseudoWireControlWord"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("pseudoWireControlWord"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message211) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfc,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ingressPhysicalInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ingressPhysicalInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ingressPhysicalInterface"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message212) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfd,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("egressPhysicalInterface");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("egressPhysicalInterface"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("egressPhysicalInterface"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message213) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xfe,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postDot1qVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postDot1qVlanId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postDot1qVlanId"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message214) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x00,0xff,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postDot1qCustomerVlanId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postDot1qCustomerVlanId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postDot1qCustomerVlanId"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message215) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x00,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("ethernetType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ethernetType"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("ethernetType"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message216) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x01,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("postIpPrecedence");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("postIpPrecedence"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("postIpPrecedence"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message217) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2d,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("selectionSequenceId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("selectionSequenceId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("selectionSequenceId"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message218) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2e,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("selectorId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("selectorId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("selectorId"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message219) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x2f,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementId");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementId"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("informationElementId"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message220) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x30,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("selectorAlgorithm");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("selectorAlgorithm"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("selectorAlgorithm"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message221) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x31,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingPacketInterval");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingPacketInterval"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("samplingPacketInterval"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message222) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x32,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingPacketSpace");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingPacketSpace"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("samplingPacketSpace"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message223) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x33,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingTimeInterval");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingTimeInterval"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("samplingTimeInterval"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message224) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x34,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingTimeSpace");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingTimeSpace"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("samplingTimeSpace"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message225) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x35,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingSize");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingSize"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("samplingSize"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message226) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x36,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingPopulation");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingPopulation"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("samplingPopulation"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message227) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x37,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingProbability");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingProbability"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("samplingProbability"), &d, sizeof d);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message228) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x39,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("ipHeaderPacketSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipHeaderPacketSection"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("ipHeaderPacketSection"), 5);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("ipHeaderPacketSection"), Varlen(5, "\x12\x34\x56\x78\x9a"), 5);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message229) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3a,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("ipPayloadPacketSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("ipPayloadPacketSection"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("ipPayloadPacketSection"), 5);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("ipPayloadPacketSection"), Varlen(5, "\x12\x34\x56\x78\x9a"), 5);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message230) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3c,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsLabelStackSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsLabelStackSection"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsLabelStackSection"), 5);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsLabelStackSection"), Varlen(5, "\x12\x34\x56\x78\x9a"), 5);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message231) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x26,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3d,0xff,0xff,0x03,0xe9,0x00,0x0a,0x05,0x12,0x34,0x56,0x78,0x9a  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(5, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("mplsPayloadPacketSection");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 5, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(5, "\x12\x34\x56\x78\x9a"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("mplsPayloadPacketSection"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("mplsPayloadPacketSection"), 5);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("mplsPayloadPacketSection"), Varlen(5, "\x12\x34\x56\x78\x9a"), 5);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message232) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3e,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("selectorIdTotalPktsObserved");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("selectorIdTotalPktsObserved"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("selectorIdTotalPktsObserved"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message233) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x3f,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("selectorIdTotalPktsSelected");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("selectorIdTotalPktsSelected"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("selectorIdTotalPktsSelected"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message234) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x40,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("absoluteError");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("absoluteError"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("absoluteError"), &d, sizeof d);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message235) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x41,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("relativeError");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("relativeError"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("relativeError"), &d, sizeof d);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message236) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x42,0x00,0x04,0x03,0xe9,0x00,0x08,0x12,0x34,0x56,0x78  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("observationTimeSeconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x12345678);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("observationTimeSeconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("observationTimeSeconds"), static_cast<uint32_t>(0x12345678));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message237) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x43,0x00,0x08,0x03,0xe9,0x00,0x0c,0x00,0x00,0x01,0x38,0xb4,0x67,0x1f,0x7c  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("observationTimeMilliseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x00000138b4671f7cULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("observationTimeMilliseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("observationTimeMilliseconds"), static_cast<uint64_t>(0x00000138b4671f7cULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message238) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x44,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("observationTimeMicroseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("observationTimeMicroseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("observationTimeMicroseconds"), static_cast<uint64_t>(0x500d6a45c00003ffULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message239) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x45,0x00,0x08,0x03,0xe9,0x00,0x0c,0x50,0x0d,0x6a,0x45,0xc0,0x00,0x03,0xff  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("observationTimeNanoseconds");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x500d6a45c00003ffULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("observationTimeNanoseconds"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("observationTimeNanoseconds"), static_cast<uint64_t>(0x500d6a45c00003ffULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message240) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x46,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("digestHashValue");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("digestHashValue"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("digestHashValue"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message241) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x47,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashIPPayloadOffset");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashIPPayloadOffset"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashIPPayloadOffset"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message242) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x48,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashIPPayloadSize");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashIPPayloadSize"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashIPPayloadSize"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message243) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x49,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashOutputRangeMin");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashOutputRangeMin"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashOutputRangeMin"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message244) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4a,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashOutputRangeMax");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashOutputRangeMax"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashOutputRangeMax"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message245) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4b,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashSelectedRangeMin");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashSelectedRangeMin"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashSelectedRangeMin"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message246) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4c,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashSelectedRangeMax");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashSelectedRangeMax"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashSelectedRangeMax"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message247) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4d,0x00,0x01,0x03,0xe9,0x00,0x05,0x01  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashDigestOutput");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashDigestOutput"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashDigestOutput"), static_cast<uint8_t>(0x1));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message248) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4e,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("hashInitialiserValue");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("hashInitialiserValue"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("hashInitialiserValue"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message249) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x4f,0xff,0xff,0x03,0xe9,0x00,0x0c,0x07,0x4e,0x6f,0x20,0x49,0x64,0x65,0x61  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(7, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("selectorName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 7, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(7, "No Idea"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("selectorName"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("selectorName"), 7);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("selectorName"), Varlen(7, "No Idea"), 7);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message250) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x50,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("upperCILimit");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("upperCILimit"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("upperCILimit"), &d, sizeof d);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message251) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x51,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("lowerCILimit");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("lowerCILimit"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("lowerCILimit"), &d, sizeof d);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message252) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x52,0x00,0x08,0x03,0xe9,0x00,0x0c,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("confidenceLevel");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("confidenceLevel"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("confidenceLevel"), &d, sizeof d);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message253) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x53,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementDataType");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementDataType"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("informationElementDataType"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message254) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x54,0xff,0xff,0x03,0xe9,0x00,0x14,0x0f,0x4d,0x79,0x20,0x76,0x65,0x72,0x79,0x20,0x6f,0x77,0x6e,0x20,0x49,0x45,0x21  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(15, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementDescription");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 15, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(15, "My very own IE!"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementDescription"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("informationElementDescription"), 15);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("informationElementDescription"), Varlen(15, "My very own IE!"), 15);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message255) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x55,0xff,0xff,0x03,0xe9,0x00,0x0c,0x07,0x6d,0x79,0x4f,0x77,0x6e,0x49,0x45  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      Varlen value(7, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 7, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(7, "myOwnIE"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementName"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("informationElementName"), 7);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("informationElementName"), Varlen(7, "myOwnIE"), 7);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message256) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x56,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementRangeBegin");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementRangeBegin"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("informationElementRangeBegin"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message257) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x28,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x57,0x00,0x08,0x03,0xe9,0x00,0x0c,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint64_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementRangeEnd");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x1020304050607080ULL);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementRangeEnd"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("informationElementRangeEnd"), static_cast<uint64_t>(0x1020304050607080ULL));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message258) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x21,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x58,0x00,0x01,0x03,0xe9,0x00,0x05,0x96  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint8_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementSemantics");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x96);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementSemantics"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("informationElementSemantics"), static_cast<uint8_t>(0x96));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message259) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x22,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x59,0x00,0x02,0x03,0xe9,0x00,0x06,0x96,0x69  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint16_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("informationElementUnits");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x9669);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("informationElementUnits"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("informationElementUnits"), static_cast<uint16_t>(0x9669));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message260) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x24,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x0c,0x03,0xe9,0x00,0x01,0x01,0x5a,0x00,0x04,0x03,0xe9,0x00,0x08,0x10,0x20,0x30,0x40  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("privateEnterpriseNumber");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("privateEnterpriseNumber"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("privateEnterpriseNumber"), static_cast<uint32_t>(0x10203040));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Compound, Fixture)

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message261) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x2c,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x10,0x03,0xe9,0x00,0x02,0x00,0x08,0x00,0x04,0x00,0x0c,0x00,0x04,0x03,0xe9,0x00,0x0c,0x7f,0x00,0x00,0x01,0x7f,0x00,0x00,0x02  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("sourceIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000001);
    }
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("destinationIPv4Address");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x7f000002);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("sourceIPv4Address"));
  wt->add(model.lookupIE("destinationIPv4Address"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("sourceIPv4Address"), static_cast<uint32_t>(0x7f000001));
  buf_writer.putValue(model.lookupIE("destinationIPv4Address"), static_cast<uint32_t>(0x7f000002));

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message262) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x30,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x10,0x03,0xe9,0x00,0x02,0x01,0x36,0x00,0x04,0x01,0x37,0x00,0x08,0x03,0xe9,0x00,0x10,0x10,0x20,0x30,0x40,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingPopulation");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingProbability");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingPopulation"));
  wt->add(model.lookupIE("samplingProbability"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.putValue(model.lookupIE("samplingPopulation"), static_cast<uint32_t>(0x10203040));
  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("samplingProbability"), &d, sizeof d);
  }

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message263) {
  static const unsigned char msg[] = {
    0x00,0x0a,0x00,0x56,0x50,0x6a,0xb0,0x45,0x00,0x00,0x00,0x00,0x00,0x01,0xe2,0x40,0x00,0x02,0x00,0x18,0x03,0xe9,0x00,0x04,0x01,0x36,0x00,0x04,0x00,0x52,0xff,0xff,0x01,0x37,0x00,0x08,0x00,0x53,0xff,0xff,0x03,0xe9,0x00,0x2e,0x10,0x20,0x30,0x40,0x04,0x65,0x74,0x68,0x30,0x3f,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x46,0x69,0x72,0x73,0x74,0x20,0x65,0x74,0x68,0x65,0x72,0x6e,0x65,0x74,0x20,0x69,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
    {
      uint32_t value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingPopulation");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x10203040);
    }
    {
      Varlen value(4, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("interfaceName");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 4, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(4, "eth0"));
    }
    {
      double value;
      const IPFIX::InfoElement* ie = model.lookupIE("samplingProbability");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(&value, sizeof value, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, 0x0.fP0);
    }
    {
      Varlen value(24, 0);
      const IPFIX::InfoElement* ie = model.lookupIE("interfaceDescription");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(value.ptr(), 24, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, Varlen(24, "First ethernet interface"));
    }
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(123456);
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(1001);

  wt->add(model.lookupIE("samplingPopulation"));
  wt->add(model.lookupIE("interfaceName"));
  wt->add(model.lookupIE("samplingProbability"));
  wt->add(model.lookupIE("interfaceDescription"));
  wt->activate();

  buf_writer.exportTemplatesForDomain();
  buf_writer.setTemplate(1001);

  buf_writer.beginRecord();

  buf_writer.reserveVarlen(model.lookupIE("interfaceName"), 4);
  buf_writer.reserveVarlen(model.lookupIE("interfaceDescription"), 24);
  buf_writer.commitVarlen();
  buf_writer.putValue(model.lookupIE("samplingPopulation"), static_cast<uint32_t>(0x10203040));
  buf_writer.putValue(model.lookupIE("interfaceName"), Varlen(4, "eth0"), 4);
  {
    double d = 0x0.fP0;
    buf_writer.putValue(model.lookupIE("samplingProbability"), &d, sizeof d);
  }
  buf_writer.putValue(model.lookupIE("interfaceDescription"), Varlen(24, "First ethernet interface"), 24);

  buf_writer.exportRecord();
  buf_writer.flush();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_REQUIRE(buf_writer.len() == sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, 4), 0);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12), 0);

  if (memcmp(buf_writer.buf(), msg, 4) != 0
      || memcmp(buf_writer.buf() + 12, msg + 12, sizeof msg - 12) != 0) {
    dump_comparison(buf_writer.buf(), msg, sizeof msg);
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
