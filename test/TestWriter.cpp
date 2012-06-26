/* Copyright (c) 2011, NEC Europe Ltd, Consorzio Nazionale 
 * Interuniversitario per le Telecomunicazioni, Institut 
 * Telecom/Telecom Bretagne, ETH Zürich, INVEA-TECH a.s. All rights reserved.
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

static int
test_writer(const std::string& protocol, const std::string& outspec) {
    // initialize information model for IPFIX, no biflows
    InfoModel::instance().defaultIPFIX();
    
    // create an exporter
    Exporter *e;
    
    if (protocol == "udp") {
      NetAddress na(outspec,"4739",IPPROTO_UDP);
      e = new UDPExporter(na, kTestDomain);
    } else if (protocol == "tcp") {
      NetAddress na(outspec,"4739",IPPROTO_TCP);
      e = new TCPExporter(na, kTestDomain, true);
    } else {
      e = new FileWriter(outspec, kTestDomain);
    }

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

// BOOST_AUTO_TEST_SUITE(Writer)
// 
// BOOST_AUTO_TEST_CASE(File) {
//   const char* outspec = "localhost";
//   const char* protocols[] = {/*"tcp", "udp", */ "test.ipfix" };
// 
//   std::for_each(protocols, protocols + sizeof(protocols)/sizeof(protocols[0]),
//                 [outspec] (const char* proto) { 
//                   BOOST_CHECK_EQUAL(test_writer(outspec, proto), 0);
//                 });
// }
// 
// BOOST_AUTO_TEST_SUITE_END()
