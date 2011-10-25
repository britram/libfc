#include "FileReader.h"
#include <cerrno>
#include <cstring>

namespace IPFIX {

void FileReader::ensureFP() {
  if (!fp_) {
    if (filename_ == "-") {
        fp_ = stdin;
    } else {
        fp_ = fopen(filename_.c_str(), "r");
    }
    if (!fp_) {
      throw IOError(strerror(errno));
    }
  }
}

bool FileReader::_receiveMessage(MBuf& mbuf, std::tr1::shared_ptr<Session>& session) {
  // make sure we have a file to read from
  this->ensureFP();

  // return session (we're a single file reader, we only have one)
  session = session_;
    
  return mbuf.deframe(fp_, *session_);  
}

FileReader::~FileReader() {
  if (fp_) {
    fclose(fp_);
    fp_ = NULL;
  }
}

}