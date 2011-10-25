#include "TestCommon.h"

using namespace IPFIX;

bool exportPacket (Exporter& e,
                   const pcap_pkthdr* caphdr,
                   const uint8_t* capbuf);
  CapfixPacket pkt;
  
  pkt.observationTimeMilliseconds = (uint64_t)caphdr->ts.tv_sec * 1000;
  pkt.observationTimeMilliseconds += caphdr->ts.tv_usec / 1000;
  pkt.ipTotalLength = caphdr->len;
  pkt.ipHeaderPacketSection.len = (size_t)caphdr->caplen;
  pkt.ipHeaderPacketSection.cp = (uint8_t *)capbuf;
  
  e.setTemplate(kCapfixPacketTid);
  e.exportRecord(caftmpl, reinterpret_cast<void*>(&pkt));
}

bool importPacket (Transcoder& xc,
                   const pcap_pkthdr* caphdr,
                   const uint8_t* capbuf);
CapfixPacket pkt;

pkt.observationTimeMilliseconds = (uint64_t)caphdr->ts.tv_sec * 1000;
pkt.observationTimeMilliseconds += caphdr->ts.tv_usec / 1000;
pkt.ipTotalLength = caphdr->len;
pkt.ipHeaderPacketSection.len = (size_t)caphdr->caplen;
pkt.ipHeaderPacketSection.cp = (uint8_t *)capbuf;

e.setTemplate(kCapfixPacketTid);
e.exportRecord(caftmpl, reinterpret_cast<void*>(&pkt));
}



int main (int argc, char *argv[]) {
  
  // set up signal handlers
  install_quit_handler();
  
  // create an information model for IPFIX, no biflows
  InfoModel model(10, false);
  
  // create a file exporter for stdout
  FileWriter fw("test.ipfix", &model, kTestDomain);
  
  StructTemplate cpstmpl;
  makeCapfixPacketTemplate(model, cpstmpl);
  
  fw.getTemplate(kCapfixPacketTid)->mimic(cpstmpl);

}