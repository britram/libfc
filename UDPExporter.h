#ifndef IPFIX_UDPEXPORTER_H // idem
#define IPFIX_UDPEXPORTER_H // hack

#include <cstdio>
#include "Exporter.h"

namespace IPFIX {
 
class UDPExporter : public Exporter {
public:
  
  static const size_t kDefaultMTU = 1476;

  UDPExporter(std::string host, std::string port, 
              uint32_t domain, 
              size_t mtu=kDefaultMTU):
    Exporter(domain, mtu),
    host_(host),
    port_(port),
    fd_(-1) {}
  
  virtual ~UDPExporter();
  
protected:
  virtual void _sendMessage(uint8_t *base, size_t len);

private:
  void connectfd();
  
  std::string   host_;
  std::string   port_;
  mutable int   fd_;
};

}
#endif // idem hack