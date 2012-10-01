#!/usr/bin/env ruby1.9
# -*- coding: utf-8 -*-
#
# Use with Brian Trammell's ripfix.
#
# Usage: Go to the ripfix directory, then type
#   /path/to/libfc/test/mkmsgs.rb > test/TestMessages.cpp
#
$LOAD_PATH << './lib'

require 'ipfix/iana'
require 'ipfix/message'

include IPFIX

$default_domain = 123456
$default_tid = 1001

class RecordElement
  attr_accessor :iespec, :iename, :value, :cpptype, :cppvalue

  def initialize(iespec, iename, value, cpptype, cppvalue)
    @iespec = iespec
    @iename = iename
    @value = value
    @cpptype = cpptype
    @cppvalue = cppvalue
  end
end

def make_message(model, res, domain = $default_domain, tid = $default_tid)
  session = Session.new(model)
  message = Message.new(session, domain)

  template = Template.new(model, tid)

  values = Hash.new()
  values[:_ipfix_tid] = tid

  res.each do |re|
    template << re.iespec
    values[re.iename] = re.value
  end

  message << template
  message << values

  message.string.unpack("C*").map{|c| ("0x%02x" % c)}.join(",")
end

def begin_file() 
print <<EOF
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
    memset(buf, '\\0', sizeof(buf));
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
EOF
end

def begin_test_suite(name)
print <<EOF

BOOST_FIXTURE_TEST_SUITE(#{name}, Fixture)
EOF
end

def end_test_suite()
print <<EOF

BOOST_AUTO_TEST_SUITE_END()
EOF
end

def end_file()
print <<EOF

BOOST_AUTO_TEST_SUITE_END()
EOF
end

$model = InfoModel.new.load_default
$n = 1

def make_test_case(res, domain = $default_domain, tid = $default_tid)
  test_no = "%03d" % $n
  $n = $n + 1

  print <<EOF

/* WARNING: AUTOMATICALLY GENERATED. MANUAL CHANGES ARE FUTILE! */
BOOST_AUTO_TEST_CASE(Message#{test_no}) {
  static const unsigned char msg[] = {
EOF
  print "    ", make_message($model, res)
  print <<EOF
  };

  IPFIX::MBuf mbuf;
  IPFIX::Session rsession;
  IPFIX::Transcoder xc;
  IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

  prepare_test_case(msg, sizeof(msg), mbuf, rsession, xc);

  for (IPFIX::SetListIter i = mbuf.begin(); i != mbuf.end(); ++i) {
    const IPFIX::WireTemplate* wt = rsession.getTemplate(mbuf.domain(), i->id);
    BOOST_REQUIRE(wt->isActive());

    IPFIX::CollectorOffsetCache oc = IPFIX::CollectorOffsetCache(wt, &xc);

    xc.focus(i->off + IPFIX::kSetHeaderLen, i->len - IPFIX::kSetHeaderLen);
EOF

  res.each { |re| 
    size = ''
    len = ''
    address = ''

    case re.cpptype
    when 'Varlen'
      size = re.value.length
      len = "(#{size}, 0)"
      address = 'value.ptr()'
    else
      size = 'sizeof value'
      address = '&value'
    end

    print <<EOF
    {
      #{re.cpptype} value#{len};
      const IPFIX::InfoElement* ie = model.lookupIE(\"#{re.iespec}\");

      BOOST_REQUIRE(wt->contains(ie));
      xc.decodeAt(#{address}, #{size}, oc.offsetOf(ie), wt->ieFor(ie));
      BOOST_CHECK_EQUAL(value, #{re.cppvalue});
    }
EOF
  }
 
  print <<EOF
    xc.defocus();
  }

  IPFIX::BufWriter buf_writer(#{domain});
  IPFIX::WireTemplate* wt = buf_writer.getTemplate(#{tid});

EOF

  res.each { |re| 
    print <<EOF
  wt->add(model.lookupIE(\"#{re.iespec}\"));
EOF
  }

  print <<EOF
  wt->activate();

EOF

  has_varlen = 0
  res.each { |re| 
    case re.cpptype
    when 'Varlen'
      print <<EOF
  buf_writer.reserveVarlen(model.lookupIE(\"#{re.iespec}\"), #{re.value.length});
EOF
      has_varlen = 1
    end
  }

  if has_varlen != 0
    print <<EOF
  buf_writer.commitVarlen();
EOF
  end

  print <<EOF

  buf_writer.exportTemplatesForDomain();

EOF

  res.each { |re| 
    case re.cpptype
    when 'Varlen'
      print <<EOF
  buf_writer.putValue(model.lookupIE(\"#{re.iespec}\"), \"#{re.value}\", #{re.value.length});
EOF
    when 'double'
      print <<EOF
  {
    double d = #{re.cppvalue};
    buf_writer.putValue(model.lookupIE(\"#{re.iespec}\"), &d, sizeof d);
  }
EOF
    when 'MyIp6Address'
      print <<EOF
  {
    MyIp6Address a = #{re.cppvalue};
    buf_writer.putValue(model.lookupIE(\"#{re.iespec}\"), &a, sizeof a);
  }
EOF
    else
      print <<EOF
  buf_writer.putValue(model.lookupIE(\"#{re.iespec}\"), static_cast<#{re.cpptype}>(#{re.cppvalue}));
EOF
    end

  }

  print <<EOF
  buf_writer.exportRecord();

  BOOST_CHECK_EQUAL(buf_writer.len(), sizeof msg);
  BOOST_CHECK_EQUAL(memcmp(buf_writer.buf(), msg, sizeof msg), 0);
}
EOF

end


begin_file()

begin_test_suite("IETypes")

# Information Element Types ---------------------------------------------------
#
# Test various IE types.  Can they hold all the values that they ought to
# hold? Is endianness correctly handled?  What about extreme values?

# unsigned8
#
make_test_case([ RecordElement.new('protocolIdentifier',
                                   :protocolIdentifier,
                                   0x00,
                                   'uint8_t', '0x00')
               ])

make_test_case([ RecordElement.new('protocolIdentifier',
                                   :protocolIdentifier,
                                   0x01,
                                   'uint8_t', '0x01')
               ])

make_test_case([ RecordElement.new('protocolIdentifier',
                                   :protocolIdentifier,
                                   0xfe,
                                   'uint8_t', '0xfe')
               ])

make_test_case([ RecordElement.new('protocolIdentifier',
                                   :protocolIdentifier,
                                   0xff,
                                   'uint8_t', '0xff')
               ])

make_test_case([ RecordElement.new('protocolIdentifier',
                                   :protocolIdentifier,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('protocolIdentifier',
                                   :protocolIdentifier,
                                   0x5a,
                                   'uint8_t', '0x5a')
               ])

# unsigned16
#
make_test_case([ RecordElement.new('sourceTransportPort',
                                   :sourceTransportPort,
                                   0x0000,
                                   'uint16_t', '0x0000')
               ])

make_test_case([ RecordElement.new('sourceTransportPort',
                                   :sourceTransportPort,
                                   0x0001,
                                   'uint16_t', '0x0001')
               ])

make_test_case([ RecordElement.new('sourceTransportPort',
                                   :sourceTransportPort,
                                   0xfffe,
                                   'uint16_t', '0xfffe')
               ])

make_test_case([ RecordElement.new('sourceTransportPort',
                                   :sourceTransportPort,
                                   0xffff,
                                   'uint16_t', '0xffff')
               ])

make_test_case([ RecordElement.new('sourceTransportPort',
                                   :sourceTransportPort,
                                   0x0010,
                                   'uint16_t', '0x0010')
               ])

# unsigned32
#
make_test_case([ RecordElement.new('ingressInterface',
                                   :ingressInterface,
                                   0x00000000,
                                   'uint32_t', '0x00000000')
               ])

make_test_case([ RecordElement.new('ingressInterface',
                                   :ingressInterface,
                                   0x00000001,
                                   'uint32_t', '0x00000001')
               ])

make_test_case([ RecordElement.new('ingressInterface',
                                   :ingressInterface,
                                   0xfffffffe,
                                   'uint32_t', '0xfffffffe')
               ])

make_test_case([ RecordElement.new('ingressInterface',
                                   :ingressInterface,
                                   0xffffffff,
                                   'uint32_t', '0xffffffff')
               ])

make_test_case([ RecordElement.new('ingressInterface',
                                   :ingressInterface,
                                   0x5aa56996,
                                   'uint32_t', '0x5aa56996')
               ])

#make_test_case([ RecordElement.new('octetDeltaCount',
#                                   :octetDeltaCount,
#                                   0x1020304050607080,
#                                   'uint64_t', '0x1020304050607080ULL')
#               ])

#make_test_case([ RecordElement.new('sourceIPv4Address',
#                                   :sourceIPv4Address,
#                                   IPAddr.new('127.0.0.1'),
#                                   'uint32_t', '0x7f000001')
#               ])

end_test_suite()

begin_test_suite("InformationElements")

# InformationElement records ---------------------------------------------------
#
# Records with only one IE.  We try all the IEs that are in the
# defaultIPFIX() Information Model.
#
make_test_case([ RecordElement.new('octetDeltaCount',
                                   :octetDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('packetDeltaCount',
                                   :packetDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('protocolIdentifier',
                                   :protocolIdentifier,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('ipClassOfService',
                                   :ipClassOfService,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('tcpControlBits',
                                   :tcpControlBits,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('sourceTransportPort',
                                   :sourceTransportPort,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('sourceIPv4Address',
                                   :sourceIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('sourceIPv4PrefixLength',
                                   :sourceIPv4PrefixLength,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('ingressInterface',
                                   :ingressInterface,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('destinationTransportPort',
                                   :destinationTransportPort,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('destinationIPv4Address',
                                   :destinationIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('destinationIPv4PrefixLength',
                                   :destinationIPv4PrefixLength,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('egressInterface',
                                   :egressInterface,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('ipNextHopIPv4Address',
                                   :ipNextHopIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('bgpSourceAsNumber',
                                   :bgpSourceAsNumber,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('bgpDestinationAsNumber',
                                   :bgpDestinationAsNumber,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('bgpNextHopIPv4Address',
                                   :bgpNextHopIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('postMCastPacketDeltaCount',
                                   :postMCastPacketDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('postMCastOctetDeltaCount',
                                   :postMCastOctetDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('flowEndSysUpTime',
                                   :flowEndSysUpTime,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('flowStartSysUpTime',
                                   :flowStartSysUpTime,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('postOctetDeltaCount',
                                   :postOctetDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('postPacketDeltaCount',
                                   :postPacketDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('minimumIpTotalLength',
                                   :minimumIpTotalLength,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('maximumIpTotalLength',
                                   :maximumIpTotalLength,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('sourceIPv6Address',
                                   :sourceIPv6Address,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('destinationIPv6Address',
                                   :destinationIPv6Address,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('sourceIPv6PrefixLength',
                                   :sourceIPv6PrefixLength,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('destinationIPv6PrefixLength',
                                   :destinationIPv6PrefixLength,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('flowLabelIPv6',
                                   :flowLabelIPv6,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('icmpTypeCodeIPv4',
                                   :icmpTypeCodeIPv4,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('igmpType',
                                   :igmpType,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('flowActiveTimeout',
                                   :flowActiveTimeout,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('flowIdleTimeout',
                                   :flowIdleTimeout,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('exportedOctetTotalCount',
                                   :exportedOctetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('exportedMessageTotalCount',
                                   :exportedMessageTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('exportedFlowRecordTotalCount',
                                   :exportedFlowRecordTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('sourceIPv4Prefix',
                                   :sourceIPv4Prefix,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('destinationIPv4Prefix',
                                   :destinationIPv4Prefix,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('mplsTopLabelType',
                                   :mplsTopLabelType,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('mplsTopLabelIPv4Address',
                                   :mplsTopLabelIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('minimumTTL',
                                   :minimumTTL,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('maximumTTL',
                                   :maximumTTL,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('fragmentIdentification',
                                   :fragmentIdentification,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('postIpClassOfService',
                                   :postIpClassOfService,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

#make_test_case([ RecordElement.new('sourceMacAddress',
#                                   :sourceMacAddress,
#                                   <macAddress>[6])
#               ])

#make_test_case([ RecordElement.new('postDestinationMacAddress',
#                                   :postDestinationMacAddress,
#                                   <macAddress>[6])
#               ])

make_test_case([ RecordElement.new('vlanId',
                                   :vlanId,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('postVlanId',
                                   :postVlanId,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('ipVersion',
                                   :ipVersion,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('flowDirection',
                                   :flowDirection,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('ipNextHopIPv6Address',
                                   :ipNextHopIPv6Address,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('bgpNextHopIPv6Address',
                                   :bgpNextHopIPv6Address,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('ipv6ExtensionHeaders',
                                   :ipv6ExtensionHeaders,
                                   0x11223344,
                                   'uint32_t', '0x11223344')
               ])

make_test_case([ RecordElement.new('mplsTopLabelStackSection',
                                   :mplsTopLabelStackSection,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection2',
                                   :mplsLabelStackSection2,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection3',
                                   :mplsLabelStackSection3,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection4',
                                   :mplsLabelStackSection4,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection5',
                                   :mplsLabelStackSection5,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection6',
                                   :mplsLabelStackSection6,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection7',
                                   :mplsLabelStackSection7,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection8',
                                   :mplsLabelStackSection8,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection9',
                                   :mplsLabelStackSection9,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection10',
                                   :mplsLabelStackSection10,
                                   [ 0x12, 0x34, 0x56 ],
                                   'Varlen', 'Varlen(3, "\x12\x34\x56")')
               ])

#make_test_case([ RecordElement.new('destinationMacAddress',
#                                   :destinationMacAddress,
#                                   <macAddress>[6])
#               ])

#make_test_case([ RecordElement.new('postSourceMacAddress',
#                                   :postSourceMacAddress,
#                                   <macAddress>[6])
#               ])

make_test_case([ RecordElement.new('interfaceName',
                                   :interfaceName,
                                   "eth0",
                                   'Varlen', 'Varlen(4, "eth0")')
               ])

make_test_case([ RecordElement.new('interfaceDescription',
                                   :interfaceDescription,
                                   "First ethernet interface",
                                   'Varlen', 'Varlen(24, "First ethernet interface")')
               ])

make_test_case([ RecordElement.new('octetTotalCount',
                                   :octetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('packetTotalCount',
                                   :packetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('fragmentOffset',
                                   :fragmentOffset,
                                   0xa55a,
                                   'uint16_t', '0xa55a')
               ])

make_test_case([ RecordElement.new('mplsVpnRouteDistinguisher',
                                   :mplsVpnRouteDistinguisher,
                                   [ 0x12, 0x34, 0x56, 0x78, 0x13, 0x24, 0x35, 0x46 ],
                                   'Varlen', 'Varlen(8, "\x12\x34\x56\x78\x13\x24\x35\x46")')
               ])

make_test_case([ RecordElement.new('mplsTopLabelPrefixLength',
                                   :mplsTopLabelPrefixLength,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('postIpDiffServCodePoint',
                                   :postIpDiffServCodePoint,
                                   0xa5,
                                   'uint8_t', '0xa5')
               ])

make_test_case([ RecordElement.new('bgpNextAdjacentAsNumber',
                                   :bgpNextAdjacentAsNumber,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('bgpPrevAdjacentAsNumber',
                                   :bgpPrevAdjacentAsNumber,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('exporterIPv4Address',
                                   :exporterIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('exporterIPv6Address',
                                   :exporterIPv6Address,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('droppedOctetDeltaCount',
                                   :droppedOctetDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('droppedPacketDeltaCount',
                                   :droppedPacketDeltaCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])
make_test_case([ RecordElement.new('droppedOctetTotalCount',
                                   :droppedOctetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])
                
make_test_case([ RecordElement.new('droppedPacketTotalCount',
                                   :droppedPacketTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])
                
make_test_case([ RecordElement.new('flowEndReason',
                                   :flowEndReason,
                                   0x96,
                                   'uint8_t', '0x96')
               ])
                
make_test_case([ RecordElement.new('commonPropertiesId',
                                   :commonPropertiesId,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])
                
make_test_case([ RecordElement.new('observationPointId',
                                   :observationPointId,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])
                
make_test_case([ RecordElement.new('icmpTypeCodeIPv6',
                                   :icmpTypeCodeIPv6,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('mplsTopLabelIPv6Address',
                                   :mplsTopLabelIPv6Address,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('lineCardId',
                                   :lineCardId,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('portId',
                                   :portId,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('meteringProcessId',
                                   :meteringProcessId,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('exportingProcessId',
                                   :exportingProcessId,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('templateId',
                                   :templateId,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('wlanChannelId',
                                   :wlanChannelId,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('wlanSSID',
                                   :wlanSSID,
                                   "Fritz!Box 1234",
                                   'Varlen', 'Varlen(14, "Fritz!Box 1234")')
               ])

make_test_case([ RecordElement.new('flowId',
                                   :flowId,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('observationDomainId',
                                   :observationDomainId,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('flowStartSeconds',
                                   :flowStartSeconds,
                                   0x12345678,
                                   'uint32_t', '0x12345678')
               ])

make_test_case([ RecordElement.new('flowEndSeconds',
                                   :flowEndSeconds,
                                   0x12345678,
                                   'uint32_t', '0x12345678')
               ])

make_test_case([ RecordElement.new('flowStartMilliseconds',
                                   :flowStartMilliseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.5),
                                   'uint64_t', '0x00000138b4671f7cULL')
               ])

make_test_case([ RecordElement.new('flowEndMilliseconds',
                                   :flowEndMilliseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.75),
                                   'uint64_t', '0x00000138b4672076ULL')
               ])

make_test_case([ RecordElement.new('flowStartMicroseconds',
                                   :flowStartMicroseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.5),
                                   'uint64_t', '0x500d6a457fffffffULL')
               ])

make_test_case([ RecordElement.new('flowEndMicroseconds',
                                   :flowEndMicroseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.75),
                                   'uint64_t', '0x500d6a45c00003ffULL')
               ])

make_test_case([ RecordElement.new('flowStartNanoseconds',
                                   :flowStartNanoseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.5),
                                   'uint64_t', '0x500d6a457fffffffULL')
               ])

make_test_case([ RecordElement.new('flowEndNanoseconds',
                                   :flowEndNanoseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.75),
                                   'uint64_t', '0x500d6a45c00003ffULL')
               ])

make_test_case([ RecordElement.new('flowStartDeltaMicroseconds',
                                   :flowStartDeltaMicroseconds,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('flowEndDeltaMicroseconds',
                                   :flowEndDeltaMicroseconds,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('systemInitTimeMilliseconds',
                                   :systemInitTimeMilliseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.75),
                                   'uint64_t', '0x00000138b4672076ULL')
               ])

make_test_case([ RecordElement.new('flowDurationMilliseconds',
                                   :flowDurationMilliseconds,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('flowDurationMicroseconds',
                                   :flowDurationMicroseconds,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('observedFlowTotalCount',
                                   :observedFlowTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('ignoredPacketTotalCount',
                                   :ignoredPacketTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('ignoredOctetTotalCount',
                                   :ignoredOctetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('notSentFlowTotalCount',
                                   :notSentFlowTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('notSentPacketTotalCount',
                                   :notSentPacketTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('notSentOctetTotalCount',
                                   :notSentOctetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('destinationIPv6Prefix',
                                   :destinationIPv6Prefix,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('sourceIPv6Prefix',
                                   :sourceIPv6Prefix,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('postOctetTotalCount',
                                   :postOctetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('postPacketTotalCount',
                                   :postPacketTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('flowKeyIndicator',
                                   :flowKeyIndicator,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('postMCastPacketTotalCount',
                                   :postMCastPacketTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('postMCastOctetTotalCount',
                                   :postMCastOctetTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('icmpTypeIPv4',
                                   :icmpTypeIPv4,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('icmpCodeIPv4',
                                   :icmpCodeIPv4,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('icmpTypeIPv6',
                                   :icmpTypeIPv6,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('icmpCodeIPv6',
                                   :icmpCodeIPv6,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('udpSourcePort',
                                   :udpSourcePort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('udpDestinationPort',
                                   :udpDestinationPort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('tcpSourcePort',
                                   :tcpSourcePort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('tcpDestinationPort',
                                   :tcpDestinationPort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('tcpSequenceNumber',
                                   :tcpSequenceNumber,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('tcpAcknowledgementNumber',
                                   :tcpAcknowledgementNumber,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('tcpWindowSize',
                                   :tcpWindowSize,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('tcpUrgentPointer',
                                   :tcpUrgentPointer,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('tcpHeaderLength',
                                   :tcpHeaderLength,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ipHeaderLength',
                                   :ipHeaderLength,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('totalLengthIPv4',
                                   :totalLengthIPv4,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('payloadLengthIPv6',
                                   :payloadLengthIPv6,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('ipTTL',
                                   :ipTTL,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('nextHeaderIPv6',
                                   :nextHeaderIPv6,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('mplsPayloadLength',
                                   :mplsPayloadLength,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('ipDiffServCodePoint',
                                   :ipDiffServCodePoint,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ipPrecedence',
                                   :ipPrecedence,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('fragmentFlags',
                                   :fragmentFlags,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('octetDeltaSumOfSquares',
                                   :octetDeltaSumOfSquares,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('octetTotalSumOfSquares',
                                   :octetTotalSumOfSquares,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('mplsTopLabelTTL',
                                   :mplsTopLabelTTL,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('mplsLabelStackLength',
                                   :mplsLabelStackLength,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('mplsLabelStackDepth',
                                   :mplsLabelStackDepth,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('mplsTopLabelExp',
                                   :mplsTopLabelExp,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ipPayloadLength',
                                   :ipPayloadLength,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('udpMessageLength',
                                   :udpMessageLength,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('isMulticast',
                                   :isMulticast,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ipv4IHL',
                                   :ipv4IHL,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ipv4Options',
                                   :ipv4Options,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('tcpOptions',
                                   :tcpOptions,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('paddingOctets',
                                   :paddingOctets,
                                   [ 0x12 ],
                                   'Varlen', 'Varlen(1, "\x12")')
               ])

make_test_case([ RecordElement.new('collectorIPv4Address',
                                   :collectorIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('collectorIPv6Address',
                                   :collectorIPv6Address,
                                   IPAddr.new('1:0200:3:0400:5:0600:7:0800'),
                                   'MyIp6Address', 
                                   'MyIp6Address(0x0001, 0x0200, 0x0003, 0x0400, 0x0005, 0x0600, 0x0007, 0x0800)')
               ])

make_test_case([ RecordElement.new('exportInterface',
                                   :exportInterface,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('exportProtocolVersion',
                                   :exportProtocolVersion,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('exportTransportProtocol',
                                   :exportTransportProtocol,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('collectorTransportPort',
                                   :collectorTransportPort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('exporterTransportPort',
                                   :exporterTransportPort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('tcpSynTotalCount',
                                   :tcpSynTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('tcpFinTotalCount',
                                   :tcpFinTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('tcpRstTotalCount',
                                   :tcpRstTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('tcpPshTotalCount',
                                   :tcpPshTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('tcpAckTotalCount',
                                   :tcpAckTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('tcpUrgTotalCount',
                                   :tcpUrgTotalCount,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('ipTotalLength',
                                   :ipTotalLength,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('postNATSourceIPv4Address',
                                   :postNATSourceIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('postNATDestinationIPv4Address',
                                   :postNATDestinationIPv4Address,
                                   IPAddr.new('127.0.0.1'),
                                   'uint32_t', '0x7f000001')
               ])

make_test_case([ RecordElement.new('postNAPTSourceTransportPort',
                                   :postNAPTSourceTransportPort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('postNAPTDestinationTransportPort',
                                   :postNAPTDestinationTransportPort,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('natOriginatingAddressRealm',
                                   :natOriginatingAddressRealm,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('natEvent',
                                   :natEvent,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('initiatorOctets',
                                   :initiatorOctets,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('responderOctets',
                                   :responderOctets,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('firewallEvent',
                                   :firewallEvent,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ingressVRFID',
                                   :ingressVRFID,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('egressVRFID',
                                   :egressVRFID,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('VRFname',
                                   :VRFname,
                                   "Hi-Ho",
                                   'Varlen', 'Varlen(5, "Hi-Ho")')
               ])

make_test_case([ RecordElement.new('postMplsTopLabelExp',
                                   :postMplsTopLabelExp,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('tcpWindowScale',
                                   :tcpWindowScale,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('biflowDirection',
                                   :biflowDirection,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ethernetHeaderLength',
                                   :ethernetHeaderLength,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('ethernetPayloadLength',
                                   :ethernetPayloadLength,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('ethernetTotalLength',
                                   :ethernetTotalLength,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('dot1qVlanId',
                                   :dot1qVlanId,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('dot1qPriority',
                                   :dot1qPriority,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('dot1qCustomerVlanId',
                                   :dot1qCustomerVlanId,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('dot1qCustomerPriority',
                                   :dot1qCustomerPriority,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('metroEvcId',
                                   :metroEvcId,
                                   "No Idea",
                                   'Varlen', 'Varlen(7, "No Idea")')
               ])

make_test_case([ RecordElement.new('metroEvcType',
                                   :metroEvcType,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('pseudoWireId',
                                   :pseudoWireId,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('pseudoWireType',
                                   :pseudoWireType,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('pseudoWireControlWord',
                                   :pseudoWireControlWord,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('ingressPhysicalInterface',
                                   :ingressPhysicalInterface,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('egressPhysicalInterface',
                                   :egressPhysicalInterface,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('postDot1qVlanId',
                                   :postDot1qVlanId,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('postDot1qCustomerVlanId',
                                   :postDot1qCustomerVlanId,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('ethernetType',
                                   :ethernetType,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('postIpPrecedence',
                                   :postIpPrecedence,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('selectionSequenceId',
                                   :selectionSequenceId,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('selectorId',
                                   :selectorId,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('informationElementId',
                                   :informationElementId,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('selectorAlgorithm',
                                   :selectorAlgorithm,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('samplingPacketInterval',
                                   :samplingPacketInterval,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('samplingPacketSpace',
                                   :samplingPacketSpace,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('samplingTimeInterval',
                                   :samplingTimeInterval,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('samplingTimeSpace',
                                   :samplingTimeSpace,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('samplingSize',
                                   :samplingSize,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('samplingPopulation',
                                   :samplingPopulation,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

make_test_case([ RecordElement.new('samplingProbability',
                                   :samplingProbability,
                                   0.9375,
                                   'double', '0x0.fP0')
               ])

make_test_case([ RecordElement.new('ipHeaderPacketSection',
                                   :ipHeaderPacketSection,
                                   [ 0x12, 0x34, 0x56, 0x78, 0x9a ],
                                   'Varlen', 'Varlen(5, "\x12\x34\x56\x78\x9a")')
               ])

make_test_case([ RecordElement.new('ipPayloadPacketSection',
                                   :ipPayloadPacketSection,
                                   [ 0x12, 0x34, 0x56, 0x78, 0x9a ],
                                   'Varlen', 'Varlen(5, "\x12\x34\x56\x78\x9a")')
               ])

make_test_case([ RecordElement.new('mplsLabelStackSection',
                                   :mplsLabelStackSection,
                                   [ 0x12, 0x34, 0x56, 0x78, 0x9a ],
                                   'Varlen', 'Varlen(5, "\x12\x34\x56\x78\x9a")')
               ])

make_test_case([ RecordElement.new('mplsPayloadPacketSection',
                                   :mplsPayloadPacketSection,
                                   [ 0x12, 0x34, 0x56, 0x78, 0x9a ],
                                   'Varlen', 'Varlen(5, "\x12\x34\x56\x78\x9a")')
               ])

make_test_case([ RecordElement.new('selectorIdTotalPktsObserved',
                                   :selectorIdTotalPktsObserved,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('selectorIdTotalPktsSelected',
                                   :selectorIdTotalPktsSelected,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('absoluteError',
                                   :absoluteError,
                                   0.9375,
                                   'double', '0x0.fP0')
               ])

make_test_case([ RecordElement.new('relativeError',
                                   :relativeError,
                                   0.9375,
                                   'double', '0x0.fP0')
               ])

make_test_case([ RecordElement.new('observationTimeSeconds',
                                   :observationTimeSeconds,
                                   0x12345678,
                                   'uint32_t', '0x12345678')
               ])

make_test_case([ RecordElement.new('observationTimeMilliseconds',
                                   :observationTimeMilliseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.5),
                                   'uint64_t', '0x00000138b4671f7cULL')
               ])

make_test_case([ RecordElement.new('observationTimeMicroseconds',
                                   :observationTimeMicroseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.75),
                                   'uint64_t', '0x500d6a45c00003ffULL')
               ])

make_test_case([ RecordElement.new('observationTimeNanoseconds',
                                   :observationTimeNanoseconds,
                                   Time.utc(2012, "jul", 23, 15, 14, 13.75),
                                   'uint64_t', '0x500d6a45c00003ffULL')
               ])

make_test_case([ RecordElement.new('digestHashValue',
                                   :digestHashValue,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('hashIPPayloadOffset',
                                   :hashIPPayloadOffset,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('hashIPPayloadSize',
                                   :hashIPPayloadSize,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('hashOutputRangeMin',
                                   :hashOutputRangeMin,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('hashOutputRangeMax',
                                   :hashOutputRangeMax,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('hashSelectedRangeMin',
                                   :hashSelectedRangeMin,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('hashSelectedRangeMax',
                                   :hashSelectedRangeMax,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('hashDigestOutput',
                                   :hashDigestOutput,
                                   0x1,
                                   'uint8_t', '0x1')
               ])

make_test_case([ RecordElement.new('hashInitialiserValue',
                                   :hashInitialiserValue,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('selectorName',
                                   :selectorName,
                                   "No Idea",
                                   'Varlen', 'Varlen(7, "No Idea")')
               ])

make_test_case([ RecordElement.new('upperCILimit',
                                   :upperCILimit,
                                   0.9375,
                                   'double', '0x0.fP0')
               ])

make_test_case([ RecordElement.new('lowerCILimit',
                                   :lowerCILimit,
                                   0.9375,
                                   'double', '0x0.fP0')
               ])

make_test_case([ RecordElement.new('confidenceLevel',
                                   :confidenceLevel,
                                   0.9375,
                                   'double', '0x0.fP0')
               ])

make_test_case([ RecordElement.new('informationElementDataType',
                                   :informationElementDataType,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('informationElementDescription',
                                   :informationElementDescription,
                                   "My very own IE!",
                                   'Varlen', 'Varlen(15, "My very own IE!")')
               ])

make_test_case([ RecordElement.new('informationElementName',
                                   :informationElementName,
                                   "myOwnIE",
                                   'Varlen', 'Varlen(7, "myOwnIE")')
               ])

make_test_case([ RecordElement.new('informationElementRangeBegin',
                                   :informationElementRangeBegin,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('informationElementRangeEnd',
                                   :informationElementRangeEnd,
                                   0x1020304050607080,
                                   'uint64_t', '0x1020304050607080ULL')
               ])

make_test_case([ RecordElement.new('informationElementSemantics',
                                   :informationElementSemantics,
                                   0x96,
                                   'uint8_t', '0x96')
               ])

make_test_case([ RecordElement.new('informationElementUnits',
                                   :informationElementUnits,
                                   0x9669,
                                   'uint16_t', '0x9669')
               ])

make_test_case([ RecordElement.new('privateEnterpriseNumber',
                                   :privateEnterpriseNumber,
                                   0x10203040,
                                   'uint32_t', '0x10203040')
               ])

end_test_suite()

# Compound records ------------------------------------------------------------
#
# Compound records contain two or more IEs.
#
begin_test_suite("Compound")

make_test_case([ RecordElement.new('sourceIPv4Address',
                                   :sourceIPv4Address,
                                   IPAddr.new("127.0.0.1"),
                                   'uint32_t', '0x7f000001'),
                 RecordElement.new('destinationIPv4Address',
                                   :destinationIPv4Address,
                                   IPAddr.new("127.0.0.2"),
                                   'uint32_t', '0x7f000002')
               ])

make_test_case([ RecordElement.new('samplingPopulation',
                                   :samplingPopulation,
                                   0x10203040,
                                   'uint32_t', '0x10203040'),
                 RecordElement.new('samplingProbability',
                                   :samplingProbability,
                                   0.9375,
                                   'double', '0x0.fP0')
               ])

make_test_case([ RecordElement.new('samplingPopulation',
                                   :samplingPopulation,
                                   0x10203040,
                                   'uint32_t', '0x10203040'),
                 RecordElement.new('interfaceName',
                                   :interfaceName,
                                   "eth0",
                                   'Varlen', 'Varlen(4, "eth0")'),
                 RecordElement.new('samplingProbability',
                                   :samplingProbability,
                                   0.9375,
                                   'double', '0x0.fP0'),
                 RecordElement.new('interfaceDescription',
                                   :interfaceDescription,
                                   "First ethernet interface",
                                   'Varlen', 'Varlen(24, "First ethernet interface")')
               ])

end_test_suite()

end_file()
