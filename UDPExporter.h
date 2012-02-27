#ifndef IPFIX_UDPEXPORTER_H // idem
#define IPFIX_UDPEXPORTER_H // hack

#include <cstdio>
#include "Exporter.h"
#include "NetAddress.h"

namespace IPFIX {
 
class UDPExporter : public Exporter {
public:
  
  static const size_t kDefaultMTU = 1476;
  
  UDPExporter(const NetAddress& addr, 
              uint32_t domain, 
              size_t mtu=kDefaultMTU):
    Exporter(domain, mtu),
    addr_(addr),
    fd_(-1) {}
  
  virtual ~UDPExporter();
  
protected:
  virtual void _sendMessage(uint8_t *base, size_t len);

private:
  
  NetAddress    addr_;
  mutable int   fd_;
};

}
#endif // idem hack