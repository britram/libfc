#include "TestCommon.h"

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
  
  virtual void receive(const Collector* collector, 
                       XCoder& setxc, 
                       const WireTemplate* settmpl) {
    SimpleFlow sf;
    
    while (settmpl.decode(setxc, sfstmpl_, &sf)) {            
      incrSimpleFlow(vsf_);
      matchSimpleFlow(vsf_, sf_);
    }
  }
};

int main (int argc, char *argv[]) {

    // set up signal handlers
    install_quit_handler();

    // create an information model for IPFIX, no biflows
    InfoModel model(10, false);
    
    // create a file exporter for stdout
    FileReader fr("-", &model);

    // register them as minimal templates
    SimpleFlowReceiver sfsrecv;
    
    fr.registerReceiver(&sfstmpl, &sfsrecv);
    
    while (!didQuit()) {
      MBuf mbuf;
      fr.receiveMessage(mbuf);
    }
}