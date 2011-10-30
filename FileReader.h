#ifndef IPFIX_FILEREADER_H // idem
#define IPFIX_FILEREADER_H // hack

#include <cstdio>
#include "Collector.h"

namespace IPFIX {
 
class FileReader : public Collector {
public:
  
  FileReader(std::string filename):
    Collector(),
    filename_(filename),
    fp_(NULL) {
      session_ = getSession(0);
    }

  virtual ~FileReader();

protected:
  void ensureFP();

  virtual bool _receiveMessage(MBuf& mbuf, std::tr1::shared_ptr<Session>& session);
  
private:
  std::tr1::shared_ptr<Session>    session_;
  std::string                      filename_;
  mutable FILE                     *fp_;
};

} // namespace IPFIX

#endif // idem hack