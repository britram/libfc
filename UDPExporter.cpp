#include "UDPExporter.h"
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace IPFIX {

void UDPExporter::connectfd() {
  struct addrinfo*  ai;
  struct addrinfo   hints;
  int ai_err;
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_ADDRCONFIG;
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
  
  if ((ai_err = getaddrinfo(host_.c_str(), port_.c_str(), &hints, &ai))) {
    throw IOError("UDP export address lookup failed");
  }
  
  do {
    fd_ = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (fd_ < 0) continue;
    if (!connect(fd_, ai->ai_addr, ai->ai_addrlen)) break;
    close(fd_);
    fd_ = -1;
  } while ((ai = ai->ai_next));

  freeaddrinfo(ai);
  
  if (fd_ < 0) {
    throw IOError("UDP export connection failed");
  }
}
  
void UDPExporter::_sendMessage(uint8_t *base, size_t len) {
  int rv;
  
  if (fd_ < 0) {
    connectfd();
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