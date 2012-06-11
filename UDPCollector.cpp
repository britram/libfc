#include <sstream>

#include <sys/socket.h>

#include "UDPCollector.h"

namespace IPFIX {
    
bool UDPCollector::ensureSocket() {
    if (sock_ > -1) return true;
    
    sock_ = netaddr_.create_socket();
    if (sock_ < 0) return false;
    
    std::cerr << "ensureSocket() got UDP socket " << sock_ << std::endl;
    
    int bufsize = 4096 * 1024;
    while (bufsize >= 8192) {
      if (setsockopt(sock_, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(int)) == 0) {
          std::cerr << "set udp receive buffer size to " << bufsize << std::endl;
          break;
      } else {
          bufsize /= 2;
      }
    }
    
    return true;
}
  
bool UDPCollector::_receiveMessage(MBuf& mbuf, std::tr1::shared_ptr<Session>& session) {

  struct sockaddr_storage sa;
  socklen_t sa_len;
  
  char packbuf[65536];
  
  // make sure we have a socket to read from
  if (!ensureSocket()) return false;

  // receive a packet into an array
  ssize_t len = recvfrom(sock_, packbuf, 65536, 0, reinterpret_cast<struct sockaddr*>(&sa), &sa_len);
  
  if (len == -1) {
    // FIXME handle errors consistently
    std::cerr << "recvfrom() returned -1" << std::endl;
    return false;
  }
  
  // wrap the packet in an istringstream
  std::string packstr(packbuf, len);
  std::istringstream packis(packstr);
  
  // return session
  session = session_;

  // FIXME everything in the same session, not 5101 compliant
  return mbuf.deframe(packis, *session_);      
}

UDPCollector::~UDPCollector() {
  if (sock_) {
    close(sock_);
    sock_ = -1;
  }
}


}
