#include "TestCommon.h"

using namespace IPFIX;

int main (int argc, char *argv[]) {


    // set up signal handlers
    install_quit_handler();

    // initialize information model for IPFIX, no biflows
    InfoModel::instance().defaultIPFIX();
    
    // create a file exporter for stdout
    FileWriter fw("test.ipfix", kTestDomain);

    // create templates for our structures
    StructTemplate sfstmpl;
    makeSimpleFlowTemplate(sfstmpl);

    // create (direct) export templates for these
    fw.getTemplate(kSimpleFlowTid)->mimic(sfstmpl);

    // export them
    fw.exportTemplatesForDomain();

    // write a test pattern until we're told to stop
    SimpleFlow sf;
    
    initSimpleFlow(sf);
    
    for (int i = 0; !didQuit() && i < kMaxFlows; ++i) {
        incrSimpleFlow(sf);
        fw.setTemplate(kSimpleFlowTid);
        fw.exportRecord(sfstmpl, reinterpret_cast<uint8_t*>(&sf));
    }
    
    fw.flush();

}