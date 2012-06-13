#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "TestCommon.h"

using namespace IPFIX;

static int
test_writer(const std::string& protocol, const std::string& outspec) {
    // initialize information model for IPFIX, no biflows
    InfoModel::instance().defaultIPFIX();
    
    // create an exporter
    Exporter *e;
    
    if (protocol == "udp") {
      NetAddress na(outspec,"4739",IPPROTO_UDP);
      e = new UDPExporter(na, kTestDomain);
    } else if (protocol == "tcp") {
      NetAddress na(outspec,"4739",IPPROTO_TCP);
      e = new TCPExporter(na, kTestDomain, true);
    } else {
      e = new FileWriter(outspec, kTestDomain);
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

    int ret = 0;

    for (int i = 0; i < kMaxFlows; ++i) {
        incrSimpleFlow(sf);
        try {
            e->setTemplate(kSimpleFlowTid);
            e->exportStruct(sfstmpl, reinterpret_cast<uint8_t*>(&sf));
        } catch (std::runtime_error const &e) {
            std::cerr << "I/O error on send: " << e.what() << std::endl;
            ret = 1;
            break;
        }
    }
    
    e->flush();

    delete e;
    return ret;
}

// BOOST_AUTO_TEST_SUITE(Writer)
// 
// BOOST_AUTO_TEST_CASE(File) {
//   const char* outspec = "localhost";
//   const char* protocols[] = {/*"tcp", "udp", */ "test.ipfix" };
// 
//   std::for_each(protocols, protocols + sizeof(protocols)/sizeof(protocols[0]),
//                 [outspec] (const char* proto) { 
//                   BOOST_CHECK_EQUAL(test_writer(outspec, proto), 0);
//                 });
// }
// 
// BOOST_AUTO_TEST_SUITE_END()
