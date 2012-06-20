#include <cstdio>

#define BOOST_TEST_DYN_LINK
#include <boost/test/parameterized_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "TestCommon.h"


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
  
  void receive(const Collector* collector, 
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
  
  // create a collector
  Collector *c;
  
  if (inspec == "udp") {
    c = new UDPCollector();
  } else if (inspec == "tcp") {
    c = new TCPSingleCollector();
  } else {
    c = new FileReader(inspec);
  }
  
  // register them as minimal templates
  SimpleFlowReceiver sfsrecv;
  
  sfsrecv.registerWithCollector(*c);
  
  int ret = 0;

  MBuf mbuf;
  while (c->receiveMessage(mbuf))
    mbuf.clear();
  
  BOOST_TEST_MESSAGE("Received " << sfsrecv.get_rec_count() << " records");

  delete c;

  return ret;
}

// BOOST_AUTO_TEST_SUITE(Reader)
// 
// BOOST_AUTO_TEST_CASE(Consume) {
//   const char* inspecs[] = {/*"tcp", "udp",*/ "test.ipfix" };
// 
//   std::for_each(inspecs, inspecs + sizeof(inspecs)/sizeof(inspecs[0]),
//                 [] (const char* inspec) { 
//                   BOOST_CHECK_EQUAL(read_and_consume(inspec), 0); 
//                 });
// }
// 
// BOOST_AUTO_TEST_SUITE_END()
