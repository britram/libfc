#include "TestCommon.h"

#include <cerrno>
#include <pcap.h>

const uint16_t kEthertypeIP4 = 0x0800;

struct ethhdr {
  uint8_t     smac[6];
  uint8_t     dmac[6];
  uint16_t    ethertype;
};

using namespace IPFIX;

static StructTemplate caftmpl;
static int datalink;

class CapfixReceiver : public SetReceiver {
private:
  pcap_dumper_t* dumper_;
  
public: 
  CapfixReceiver(pcap_dumper_t* dumper): dumper_(dumper) {}
  
  virtual void receive(const Collector* collector, 
                       Transcoder& setxc, 
                       const WireTemplate* wt) {
    
    CapfixPacket pkt;
    pcap_pkthdr caphdr;
    
    while (wt->decode(setxc, caftmpl, reinterpret_cast<uint8_t*>(&pkt))) {
      caphdr.ts.tv_sec = pkt.observationTimeMilliseconds / 1000;
      caphdr.ts.tv_usec = (pkt.observationTimeMilliseconds % 1000) * 1000;
      caphdr.len = pkt.ipTotalLength;
      caphdr.caplen = pkt.ipHeaderPacketSection.len;
      pcap_dump(reinterpret_cast<unsigned char*>(dumper_), &caphdr, pkt.ipHeaderPacketSection.cp);
    }
  }
};


void exportPacket (unsigned char *evp,
                   const pcap_pkthdr* caphdr,
                   const uint8_t* capbuf) {
  FileWriter* e = reinterpret_cast<FileWriter *>(evp);
  CapfixPacket pkt;
  size_t caplen = caphdr->caplen;
  
  if (datalink == DLT_EN10MB) {
    const ethhdr* eh = reinterpret_cast<const ethhdr*>(capbuf);
    if (ntohs(eh->ethertype) == kEthertypeIP4) {
      caplen -= sizeof(ethhdr);
      capbuf += sizeof(ethhdr);
    } else {
      return;
    }
  }
  
  pkt.observationTimeMilliseconds = (uint64_t)caphdr->ts.tv_sec * 1000;
  pkt.observationTimeMilliseconds += caphdr->ts.tv_usec / 1000;
  pkt.ipTotalLength = caphdr->len;
  pkt.ipHeaderPacketSection.len = (size_t)caphdr->caplen;
  pkt.ipHeaderPacketSection.cp = (uint8_t *)capbuf;
  
  e->setTemplate(kCapfixPacketTid);
  e->exportRecord(caftmpl, reinterpret_cast<uint8_t*>(&pkt));
}

std::string new_extension(const std::string& filename, const std::string& extension) {
  size_t dpos = filename.rfind('.');
  if (dpos == std::string::npos) {
    return filename + std::string(".") + extension;
  } else {
    return filename.substr(0, dpos + 1) + extension;
  }
}

int main_to_pcap(const std::string& filename) {

  // open an ipfix source
  FileReader fr(filename);
  pcap_t* pcap = pcap_open_dead(DLT_RAW, 65535);
  pcap_dumper_t* dumper = pcap_dump_open(pcap, new_extension(filename, std::string("pcap")).c_str());
  if (!dumper) {
    std::cerr << "failed to open pcap dumper: " << pcap_geterr(pcap) << std::endl;
    return 1;
  }
  
  // create a capfix receiver around the output file
  CapfixReceiver cr(dumper);
  
  // register our set receiver
  fr.registerReceiver(&caftmpl, &cr);
  
  while (!didQuit()) {
    MBuf mbuf;
    if (!fr.receiveMessage(mbuf)) { doQuit(0); }
  }
  
  // clean up (file reader closes automatically on destruction)
  pcap_dump_close(dumper);
  return 0;
}

int main_to_ipfix(const std::string& filename) {
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *pcap;
  
  // open a pcap source
  pcap = pcap_open_offline(filename.c_str(), errbuf);
  if (!pcap) {
    std::cerr << "failed to open pcap file: " << errbuf << std::endl;
    return 1;
  }
  
  // make sure we like the datalink
  datalink = pcap_datalink(pcap);
  switch (datalink) {
    case DLT_EN10MB:
    case DLT_RAW:
      break;
    default:
      std::cerr << "capfix only groks Ethernet and raw IP frames." << std::endl;
      return 1;
      break;
  }
  
  // set up output
  FileWriter fw(new_extension(filename, std::string("ipfix")), kTestDomain);
  makeCapfixPacketTemplate(caftmpl);
  fw.getTemplate(kCapfixPacketTid)->mimic(caftmpl);
  fw.exportTemplatesForDomain();
  
  // runloop
  int rv;
  while (!didQuit()) {
    int pcrv = pcap_dispatch(pcap, 1, exportPacket, reinterpret_cast<uint8_t *>(&fw));
    if (pcrv == 0) {
      // no packets, normal exit
      rv = 0;
      break;
    } else if (pcrv < 0) {
      // something broke, run away screaming
      std::cerr << "pcap error: " << errbuf << std::endl;
      rv = 1;
      break;
    }
  }
  
  // clean up
  fw.flush();
  pcap_close(pcap);
  return rv;
}

int main (int argc, char *argv[]) {
  
  // set up signal handlers
  install_quit_handler();
  
  // create an information model for IPFIX, no biflows
  InfoModel::instance().defaultIPFIX();
  
  // determine what kind of file we're looking at
  if (argc >= 2) {
    std::string filename(argv[1]);
    if (filename.find(".ipfix", filename.length() - 6) != std::string::npos) {
      return main_to_pcap(filename);
    } else if (filename.find(".pcap", filename.length() - 5) != std::string::npos) {
      return main_to_ipfix(filename);
    } 
  }
  
  std::cerr << "need a filename ending in .ipfix or .pcap to continue..." << std::endl;
  return 1;
}