#include "FileWriter.h"
#include <cerrno>
#include <cstring>

namespace IPFIX {
  
void FileWriter::_sendMessage(uint8_t *base, size_t len) {
  int rv;
  
  std::cerr << "FileWriter::sendMessage(" << reinterpret_cast<unsigned long> (base)<< ", " << len << ")" << std::endl;
  if (!fp_) {
    if (filename_ == "-") {
        fp_ = stdout;
    } else {
        fp_ = fopen(filename_.c_str(), "w");
    }
    if (!fp_) {
      throw IOError(strerror(errno));
    }
  }
	
	std::cerr << "fp is " << reinterpret_cast<unsigned long> (fp_) << std::endl;
  if ((rv = fwrite((void *)base, len, 1, fp_)) < 0) {
    throw IOError(strerror(errno));
  }
  
  
  std::cerr << "after fwrite(), rv = " << rv << std::endl;
}

FileWriter::~FileWriter() {
  if (fp_) {
    flush();
    fclose(fp_);
    fp_ = NULL;
  }

}

}