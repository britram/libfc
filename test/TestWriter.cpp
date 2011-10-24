#include "TestCommon.h"

using namespace IPFIX;

int main (int argc, char *argv[]) {


    // set up signal handlers
    install_quit_handler();

    // create an information model for IPFIX, no biflows
    InfoModel model(10, false);
    
    // create a file exporter for stdout
    FileWriter fw("test.ipfix", &model, kTestDomain);

    // create templates for our structures
    StructTemplate sfstmpl;
    makeSimpleFlowTemplate(model, sfstmpl);

    StructTemplate cpstmpl;
    makeCapfixPacketTemplate(model, cpstmpl);

    // create (direct) export templates for these
    fw.getTemplate(kSimpleFlowTid)->mimic(sfstmpl);
    fw.getTemplate(kCapfixPacketTid)->mimic(cpstmpl);

    // export them
    fw.exportTemplatesForDomain();

    // write a test pattern until we're told to stop
    SimpleFlow sf;
    
    initSimpleFlow(sf);
    
    for (int i = 0; !didQuit() && i < kMaxFlows; ++i) {
        incrSimpleFlow(sf);
        fw.setTemplate(kSimpleFlowTid);
        fw.exportRecord(sfstmpl, reinterpret_cast<void*>(&sf));
    }
    
    fw.flush();

}