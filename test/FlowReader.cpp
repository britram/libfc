#include "TestCommon.h"
#include <cstdio>

class PrintingFlowReceiver : public IPFIX::SetReceiver {
private:
  StructTemplate vst_;
  std::tr1::shared_ptr<PrintingFlowReceiver> thissp_;
  
public:
  PrintingFlowReceiver() {
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
    static int set_count = 0;
    static int rec_count = 0;
    
    SimpleFlow sf;
    
    while (settmpl->decode(setxc, vst_, reinterpret_cast<uint8_t*>(&sf))) {            

      rec_count++;

     fprintf(stdout, "[%5u/%5u] %016llx - %016llx %08x:%04x -> %08x:%04x (%08x)\n",
             set_count, rec_count, 
             sf.flowStartMilliseconds, sf.flowEndMilliseconds, 
             sf.sourceIPv4Address, sf.sourceTransportPort, 
             sf.destinationIPv4Address, sf.destinationTransportPort,
             sf.packetDeltaCount);
    }
    set_count++;
  }
};


int main (int argc, char *argv[]) {

    // set up signal handlers
    install_quit_handler();

    // initialize an information model for IPFIX, no biflows
    InfoModel::instance().defaultIPFIX();
    
    // create a collector
    Collector *c;
    
    if (argc >= 2) {
      std::string inspec(argv[1]);
      if (inspec == "udp") {
        c = new UDPCollector();
      } else if (inspec == "tcp") {
        c = new TCPSingleCollector();
      } else {
        c = new FileReader(inspec);
      }
    } else {
      c = new FileReader("test.ipfix");
    }
    
    // register them as minimal templates
    PrintingFlowReceiver frecv;
    
    frecv.registerWithCollector(*c);
        
    while (!didQuit()) {
      MBuf mbuf;
      std::cerr << "waiting for message" << std::endl;
      if (!c->receiveMessage(mbuf)) { 
        std::cerr << "collector signaled end of stream" << std::endl;
        doQuit(0); 
      }
    }
    
    delete c;
}