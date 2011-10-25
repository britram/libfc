#include "TestCommon.h"

using namespace IPFIX;

gboolean cafAppendPacket(Exporter& e,
                         const pcap_pkthdr* caphdr,
                         const uint8_t* capbuf);
                         fBuf_t                      *fbuf,
                         const struct pcap_pkthdr    *caphdr,
                         const uint8_t               *capbuf,
                         GError                      **err)
{
  CapfixPacket pkt;
  
  fbrec.observationTimeMilliseconds = (uint64_t)caphdr->ts.tv_sec * 1000;
  fbrec.observationTimeMilliseconds += caphdr->ts.tv_usec / 1000;
  fbrec.ipTotalLength = caphdr->len;
  fbrec.ipHeaderPacketSection.len = (size_t)caphdr->caplen;
  fbrec.ipHeaderPacketSection.buf = (uint8_t *)capbuf;
  
  if (!fBufSetExportTemplate(fbuf, PKT_TID, err)) return FALSE;
  if (!fBufSetInternalTemplate(fbuf, PKT_TID, err)) return FALSE;
  return fBufAppend(fbuf, (uint8_t *)&fbrec, sizeof(fbrec), err);
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