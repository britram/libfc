#ifndef IPFIX_TCPEXPORTER_H // idem
#define IPFIX_TCPEXPORTER_H // hack

#include <cstdio>
#include "Exporter.h"
#include "NetAddress.h"

namespace IPFIX {
 
class TCPExporter : public Exporter {
public:
  
  static const size_t kDefaultMTU = 65535;
  static const time_t retry_delay_min_ = 1;
  static const time_t retry_delay_max_ = 64;
  
  
  TCPExporter(const NetAddress& addr, 
              uint32_t domain,
              bool retry = false,
              size_t mtu = kDefaultMTU):
    Exporter(domain, mtu),
    addr_(addr),
    retry_(retry),
    retry_delay_(retry_delay_min_),
    fd_(-1) {}
  
  virtual ~TCPExporter();
  
protected:
  virtual void _sendMessage(uint8_t *base, size_t len);

private:
  
  NetAddress    addr_;
  bool          retry_;
  time_t        retry_delay_;
  mutable int   fd_;
};

}
#endif // idem hack