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

#include "TestCommon.h"

#include <signal.h>
#include <sys/time.h>
#include <cerrno>

static int quit_counter = 0;

void doQuit(int signum) {
    quit_counter++;
}

void install_quit_handler() {
    struct sigaction sa, osa;

    sa.sa_handler = doQuit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT,&sa,&osa)) {
      throw std::runtime_error(std::string("sigaction(SIGINT) failed: ") + strerror(errno));
    }

    sa.sa_handler = doQuit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGTERM,&sa,&osa)) {
      throw std::runtime_error(std::string("sigaction(SIGTERM) failed: ") + strerror(errno));
    }
}

bool didQuit() {
    return (quit_counter > 0);
}

uint64_t sys_ms() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<uint64_t>(tv.tv_sec) * 1000 +  
           static_cast<uint64_t>(tv.tv_usec) / 1000;
}
void makeSimpleFlowTemplate(IPFIX::StructTemplate& sfstmpl) {
    IPFIX::InfoModel& model = IPFIX::InfoModel::instance();
  
    sfstmpl.add(model.lookupIE("flowStartMilliseconds"),    offsetof(SimpleFlow, flowStartMilliseconds));
    sfstmpl.add(model.lookupIE("flowEndMilliseconds"),      offsetof(SimpleFlow, flowEndMilliseconds));
    sfstmpl.add(model.lookupIE("sourceIPv4Address"),        offsetof(SimpleFlow, sourceIPv4Address));
    sfstmpl.add(model.lookupIE("destinationIPv4Address"),   offsetof(SimpleFlow, destinationIPv4Address));
    sfstmpl.add(model.lookupIE("sourceTransportPort"),      offsetof(SimpleFlow, sourceTransportPort));
    sfstmpl.add(model.lookupIE("destinationTransportPort"), offsetof(SimpleFlow, destinationTransportPort));
    sfstmpl.add(model.lookupIE("packetDeltaCount[4]"),      offsetof(SimpleFlow, packetDeltaCount));
    sfstmpl.activate();
}

void makeCapfixPacketTemplate(IPFIX::StructTemplate& cpstmpl) {
    IPFIX::InfoModel& model = IPFIX::InfoModel::instance();

    cpstmpl.add(model.lookupIE("observationTimeMilliseconds"), 
        offsetof(CapfixPacket, observationTimeMilliseconds));
    cpstmpl.add(model.lookupIE("ipTotalLength[4]"),
        offsetof(CapfixPacket, ipTotalLength));
    cpstmpl.add(model.lookupIE("ipHeaderPacketSection"),       
        offsetof(CapfixPacket, ipHeaderPacketSection));
    cpstmpl.activate();
}


void initSimpleFlow(SimpleFlow &sf) {
    sf.sourceIPv4Address = 0x0AFFFFFF;
    sf.destinationIPv4Address = 0x0A000000;
    sf.sourceTransportPort = 0;
    sf.destinationTransportPort = 32768;
    sf.packetDeltaCount = 0;
}

void incrSimpleFlow(SimpleFlow &sf) {
    sf.flowEndMilliseconds = sys_ms();
    sf.flowStartMilliseconds = sf.flowEndMilliseconds - 1000;
    --sf.sourceIPv4Address;
    if (sf.sourceIPv4Address < 0x0AFF0000) 
        sf.sourceIPv4Address = 0x0AFFFFFF;
    ++sf.destinationIPv4Address;
    if (sf.destinationIPv4Address > 0x0A00FFFF) 
        sf.destinationIPv4Address = 0x0A000000;
    ++sf.sourceTransportPort;
    ++sf.destinationTransportPort;
    ++sf.packetDeltaCount;
}


void matchSimpleFlow(SimpleFlow& expected, SimpleFlow& actual) {
    if (expected.packetDeltaCount != actual.packetDeltaCount) {
        std::cout << "packet count mismatch expected " <<
                     expected.packetDeltaCount << " got " <<
                     actual.packetDeltaCount << std::endl;
    }
}
