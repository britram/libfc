#include "TCPExporter.h"
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace IPFIX {
  
void TCPExporter::_sendMessage(uint8_t *base, size_t len) {
  int rv;
  
  if (fd_ < 0) {
    fd_ = addr_.create_socket();
  }

  if ((rv = send(fd_, base, len, 0)) < 0) {
    throw IOError(strerror(errno));
  }
  
  std::cerr << "after fwrite(), rv = " << rv << std::endl;
}

TCPExporter::~TCPExporter() {
  if (fd_ >= 0) {
    flush();
    close(fd_);
    fd_ = -1;
  }

}

}