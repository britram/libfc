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

#include "FileWriter.h"
#include "FileReader.h"
#include "UDPExporter.h"
#include "UDPCollector.h"
#include "TCPExporter.h"
#include "TCPSingleCollector.h"

static const uint32_t kTestDomain = 0x00C0FFEE;
static const uint32_t kSimpleFlowTid = 0xAB34;
static const uint32_t kCapfixPacketTid = 0xAB35;

static const int kMaxFlows = 100000;
using namespace IPFIX;

struct SimpleFlow {
    uint64_t flowStartMilliseconds;
    uint64_t flowEndMilliseconds;
    uint32_t sourceIPv4Address;
    uint32_t destinationIPv4Address;
    uint16_t sourceTransportPort;
    uint16_t destinationTransportPort;
    uint32_t packetDeltaCount;
};

struct CapfixPacket {
    uint64_t observationTimeMilliseconds;
    uint32_t ipTotalLength;
    VarlenField ipHeaderPacketSection;
};

void install_quit_handler();
bool didQuit();
void doQuit(int signum);

uint64_t sys_ms();

void makeSimpleFlowTemplate(StructTemplate& sfstmpl);
void makeCapfixPacketTemplate(StructTemplate& sfstmpl);

void initSimpleFlow(SimpleFlow& sf);
void incrSimpleFlow(SimpleFlow& sf);
void matchSimpleFlow(SimpleFlow& expected, SimpleFlow& actual);