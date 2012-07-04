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

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "TestCommon.h"

using namespace IPFIX;

static Exporter* make_exporter(const std::string& protocol,
                               const std::string& outspec) {
  Exporter* ret = 0;

  if (protocol == "udp") {
    NetAddress na(outspec,"4739",IPPROTO_UDP);
    ret = new UDPExporter(na, kTestDomain);
  } else if (protocol == "tcp") {
    NetAddress na(outspec,"4739",IPPROTO_TCP);
    ret = new TCPExporter(na, kTestDomain, true);
  } else {
    ret = new FileWriter(outspec, kTestDomain);
  }

  return ret;
}

static int
test_struct_writer(const std::string& protocol, const std::string& outspec) {
    // create an exporter
    Exporter *e = make_exporter(protocol, outspec);

    // create templates for our structures
    StructTemplate sfstmpl;
    makeSimpleFlowTemplate(sfstmpl);

    // create (direct) export templates for these
    e->getTemplate(kSimpleFlowTid)->mimic(sfstmpl);

    // export them
    e->exportTemplatesForDomain();

    // write a test pattern until we're told to stop
    SimpleFlow sf;
    
    initSimpleFlow(sf);

    int ret = 0;

    for (int i = 0; i < kMaxFlows; ++i) {
        incrSimpleFlow(sf);
        try {
            e->setTemplate(kSimpleFlowTid);
            e->exportStruct(sfstmpl, reinterpret_cast<uint8_t*>(&sf));
        } catch (std::runtime_error const &e) {
            std::cerr << "I/O error on send: " << e.what() << std::endl;
            ret = 1;
            break;
        }
    }
    
    e->flush();

    delete e;
    return ret;
}

static int
test_export(const std::string& protocol, const std::string& outspec) {
    // create an exporter
    Exporter *e = make_exporter(protocol, outspec);

    // create templates for our structures
    // Note: even though the struct interface will not be used, the template
    // initialisation code should still work to set up the wire template
    StructTemplate sfstmpl;
    makeSimpleFlowTemplate(sfstmpl);

    // create (direct) export template
    e->getTemplate(kSimpleFlowTid)->mimic(sfstmpl);

    // export templates
    e->exportTemplatesForDomain();

    // write a test pattern until we're told to stop
    SimpleFlow sf;
    
    initSimpleFlow(sf);

    int ret = 0;

    // cache information elements
    const InfoElement* flowStartMilliseconds
      = InfoModel::instance().lookupIE("flowStartMilliseconds");
    const InfoElement* flowEndMilliseconds
      = InfoModel::instance().lookupIE("flowEndMilliseconds");
    const InfoElement* octetDeltaCount
      = InfoModel::instance().lookupIE("octetDeltaCount");
    const InfoElement* packetDeltaCount
      = InfoModel::instance().lookupIE("packetDeltaCount");
    const InfoElement* sourceIPv4Address
      = InfoModel::instance().lookupIE("sourceIPv4Address");
    const InfoElement* destinationIPv4Address
      = InfoModel::instance().lookupIE("destinationIPv4Address");
    const InfoElement* sourceTransportPort
      = InfoModel::instance().lookupIE("sourceTransportPort");
    const InfoElement* destinationTransportPort
      = InfoModel::instance().lookupIE("destinationTransportPort");
    const InfoElement* protocolIdentifier
      = InfoModel::instance().lookupIE("protocolIdentifier");

    e->setTemplate(kSimpleFlowTid);

    for (int i = 0; i < kMaxFlows; ++i) {
        incrSimpleFlow(sf);
        try {
            e->beginRecord();

            e->putValue(flowStartMilliseconds, sf.flowStartMilliseconds);
            e->putValue(flowEndMilliseconds, sf.flowEndMilliseconds);
            e->putValue(packetDeltaCount, sf.packetDeltaCount);
            e->putValue(sourceIPv4Address, sf.sourceIPv4Address);
            e->putValue(destinationIPv4Address, sf.destinationIPv4Address);
            e->putValue(sourceTransportPort, sf.sourceTransportPort);
            e->putValue(destinationTransportPort, sf.destinationTransportPort); 
            
            e->exportRecord();

        } catch (std::runtime_error const &e) {
            std::cerr << "I/O error on send: " << e.what() << std::endl;
            ret = 1;
            break;
        }
    }
    
    e->flush();

    delete e;
    return ret;
}

BOOST_AUTO_TEST_SUITE(Writer)

BOOST_AUTO_TEST_CASE(FileStruct) {
  const char* outspec = "test-struct-flow.ipfix";
  const char* protocol = "rfc5655"; // IPFIX file (default)

  // initialize information model for IPFIX, no biflows
  InfoModel::instance().defaultIPFIX();  
  
  BOOST_CHECK_EQUAL(test_struct_writer(protocol, outspec), 0);
}

BOOST_AUTO_TEST_CASE(FileExport) {
  const char* outspec = "test-ra-flow.ipfix";
  const char* protocol = "rfc5655"; // IPFIX file (default)

  // initialize information model for IPFIX, no biflows
  InfoModel::instance().defaultIPFIX();  
  
  BOOST_CHECK_EQUAL(test_export(protocol, outspec), 0);
}

BOOST_AUTO_TEST_SUITE_END()
