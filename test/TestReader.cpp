#include "TestCommon.h"
#include <cstdio>

using namespace IPFIX;

class SimpleFlowReceiver : public SetReceiver {
private:
  SimpleFlow vsf_;
  StructTemplate vst_;
  
public:
  SimpleFlowReceiver(InfoModel& model) {
    initSimpleFlow(vsf_);
    makeSimpleFlowTemplate(model, vst_);
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
    static int packet_count = 0;
    
    while (settmpl->decode(setxc, vst_, &sf)) {            
      incrSimpleFlow(vsf_);
      matchSimpleFlow(vsf_, sf);

      fprintf(stdout, "[X %5u] %016llx - %016llx %08x:%04x -> %08x:%04x (%08x)\n",
              packet_count, 
              vsf_.flowStartMilliseconds, vsf_.flowEndMilliseconds, 
              vsf_.sourceIPv4Address, vsf_.sourceTransportPort, 
              vsf_.destinationIPv4Address, vsf_.destinationTransportPort,
              vsf_.packetDeltaCount);      
      fprintf(stdout, "[A %5u] %016llx - %016llx %08x:%04x -> %08x:%04x (%08x)\n",
              packet_count++, 
              sf.flowStartMilliseconds, sf.flowEndMilliseconds, 
              sf.sourceIPv4Address, sf.sourceTransportPort, 
              sf.destinationIPv4Address, sf.destinationTransportPort,
              sf.packetDeltaCount);
    }
  }
};

int main (int argc, char *argv[]) {

    // set up signal handlers
    install_quit_handler();

    // create an information model for IPFIX, no biflows
  InfoModel& model = InfoModel::instance();
    model.defaultIPFIX();
    
    // create a file exporter for stdout
    FileReader fr("test.ipfix", &model);

    // register them as minimal templates
    SimpleFlowReceiver sfsrecv(model);
    
    sfsrecv.registerWithCollector(fr);
        
    while (!didQuit()) {
      MBuf mbuf;
      fr.receiveMessage(mbuf);
    }
}