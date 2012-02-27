#include "TestCommon.h"

using namespace IPFIX;

int main (int argc, char *argv[]) {

    // set up signal handlers
    install_quit_handler();

    // initialize information model for IPFIX, no biflows
    InfoModel::instance().defaultIPFIX();
    
    // create an exporter
    Exporter *e;
    
    if (argc >= 3) {
      std::string protocol(argv[1]);
      std::string outspec(argv[2]);
      if (protocol == "udp") {
        NetAddress na(outspec,"4739",IPPROTO_UDP);
        e = new UDPExporter(na, kTestDomain);
      } else {
        e = new FileWriter(outspec, kTestDomain);
      }
    } else {
      e = new FileWriter("test.ipfix", kTestDomain);
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
    
    for (int i = 0; !didQuit() && i < kMaxFlows; ++i) {
        incrSimpleFlow(sf);
        e->setTemplate(kSimpleFlowTid);
        e->exportRecord(sfstmpl, reinterpret_cast<uint8_t*>(&sf));
    }
    
    e->flush();

    delete e;
}