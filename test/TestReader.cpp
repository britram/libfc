#include "TestCommon.h"
#include <cstdio>

using namespace IPFIX;

class SimpleFlowReceiver : public SetReceiver {
private:
  SimpleFlow vsf_;
  StructTemplate vst_;
  
public:
  SimpleFlowReceiver() {
    initSimpleFlow(vsf_);
    makeSimpleFlowTemplate(vst_);
  }
  
  const IETemplate *structTemplate() {
    return &vst_;
  }
  
  void registerWithCollector(Collector& c) {
    c.registerReceiver(&vst_, this);
  }
  
  virtual void receive(const Collector* collector, 
                       Transcoder& setxc, 
                       const WireTemplate* settmpl) {
    SimpleFlow sf;
    
    while (settmpl->decode(setxc, vst_, reinterpret_cast<uint8_t*>(&sf))) {            
      incrSimpleFlow(vsf_);
      matchSimpleFlow(vsf_, sf);

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
  }
};

int main (int argc, char *argv[]) {

    // set up signal handlers
    install_quit_handler();

    // initialize an information model for IPFIX, no biflows
    InfoModel::instance().defaultIPFIX();
    
    // create a file exporter for stdout
    FileReader fr("test.ipfix");

    // register them as minimal templates
    SimpleFlowReceiver sfsrecv;
    
    sfsrecv.registerWithCollector(fr);
        
    while (!didQuit()) {
      MBuf mbuf;
      if (!fr.receiveMessage(mbuf)) { doQuit(0); }
    }
}