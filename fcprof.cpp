/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * The name of ETH Zürich nor the names of other contributors 
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

#include <cstdio>

#include "FileReader.h"
#include "test/TestCommon.h"

using namespace IPFIX;

class SimpleFlowReceiver : public SetReceiver {
private:
  SimpleFlow vsf_;
  StructTemplate vst_;
  std::shared_ptr<SimpleFlowReceiver> thissp_;
  int set_count;
  int rec_count;
  
public:
  SimpleFlowReceiver() : set_count(0), rec_count(0) {
    initSimpleFlow(vsf_);
    makeSimpleFlowTemplate(vst_);
  }
  
  int get_set_count() const {
    return set_count;
  }

  int get_rec_count() const {
    return rec_count;
  }

  const IETemplate *structTemplate() {
    return &vst_;
  }

  void registerWithCollector(Collector& c) {
    c.registerReceiver(&vst_, this);
  }
  
  void receiveSet(const Collector* collector, 
                       Transcoder& setxc, 
                       const WireTemplate* settmpl)  {
    
    SimpleFlow sf;
    
    while (settmpl->decodeStruct(setxc, vst_, reinterpret_cast<uint8_t*>(&sf))) {            

      //std::cerr << " in set " << std::dec << set_count << " record " << rec_count << 
      //" at " << std::hex << reinterpret_cast<uint64_t>(setxc.cur()) << std::endl;
      
      incrSimpleFlow(vsf_);
      matchSimpleFlow(vsf_, sf);
      rec_count++;

//      fprintf(stdout, "[X %5u] %016llx - %016llx %08x:%04x -> %08x:%04x (%08x)\n",
//              packet_count, 
//              vsf_.flowStartMilliseconds, vsf_.flowEndMilliseconds, 
//              vsf_.sourceIPv4Address, vsf_.sourceTransportPort, 
//              vsf_.destinationIPv4Address, vsf_.destinationTransportPort,
//              vsf_.packetDeltaCount);      
//      fprintf(stdout, "[A %5u] %016llx - %016llx %08x:%04x -> %08x:%04x (%08x)\n",
//              packet_count++, 
//              sf.flowStartMilliseconds, sf.flowEndMilliseconds, 
//              sf.sourceIPv4Address, sf.sourceTransportPort, 
//              sf.destinationIPv4Address, sf.destinationTransportPort,
//              sf.packetDeltaCount);
    }
    set_count++;
  }
};


static int read_and_consume (std::string inspec) {
  // initialize an information model for IPFIX, no biflows
  InfoModel::instance().defaultIPFIX();
  
  for (int i = 0; i < 10000; i++) {
    // create a collector
    Collector *c = new FileReader(inspec);
    
    // register them as minimal templates
    SimpleFlowReceiver sfsrecv;
    
    sfsrecv.registerWithCollector(*c);
    
    MBuf mbuf;
    while (c->receiveMessage(mbuf))
      mbuf.clear();
  
    delete c;
  }

  return 0;
}

int main(void) {
  read_and_consume("test-struct-flow.ipfix");
}
