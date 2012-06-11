#include <cerrno>
#include <cstring>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "UDPExporter.h"

#include "exceptions/IOError.h"

namespace IPFIX {
  
void UDPExporter::_sendMessage(uint8_t *base, size_t len) {
  int rv;
  
  if (fd_ < 0) {
    fd_ = addr_.create_socket();
  }

  if ((rv = send(fd_, base, len, 0)) < 0) {
    throw IOError(strerror(errno));
  }
  
  std::cerr << "after fwrite(), rv = " << rv << std::endl;
}

UDPExporter::~UDPExporter() {
  if (fd_ >= 0) {
    flush();
    close(fd_);
    fd_ = -1;
  }

}

}
