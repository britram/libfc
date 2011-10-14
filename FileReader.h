#ifndef IPFIX_FILEREADER_H // idem
#define IPFIX_FILEREADER_H // hack

#include <cstdio>
#include "Exporter.h"

namespace IPFIX {
 
class FileReader : public Collector {
public:
  
  FileReader(std::string filename, const InfoModel* model):
    Collector(model),
    filename_(filename),
    fp_(NULL) {}

  virtual ~FileReader();

private:
  std::string   filename_;
  mutable FILE *fp_;
};

} // namespace IPFIX

#endif // idem hack