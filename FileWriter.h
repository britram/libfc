#ifndef IPFIX_FILEWRITER_H // idem
#define IPFIX_FILEWRITER_H // hack

#include <cstdio>
#include "Exporter.h"

namespace IPFIX {
 
class FileWriter : public Exporter {
public:
  
  static const size_t kDefaultMTU = 65535;

  FileWriter(std::string filename, const InfoModel* model, uint32_t domain, size_t mtu=kDefaultMTU):
    Exporter(model, domain, mtu),
    filename_(filename),
    fp_(NULL) {}
  
  virtual ~FileWriter();
  
protected:
  virtual void sendMessage(uint8_t *base, size_t len);

private:
  std::string   filename_;
  mutable FILE *fp_;
};

} // namespace IPFIX

#endif // idem hack