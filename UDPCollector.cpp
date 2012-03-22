#include <UDPCollector.h>

namespace IPFIX {
  
bool UDPCollector::_receiveMessage(MBuf& mbuf, std::tr1::shared_ptr<Session>& session) {

  struct sockaddr_storage sa;
  socklen_t sa_len;
  
  char packbuf[65536];
  
  // make sure we have a socket to read from
  if (sock_ == -1) {
      std::cerr << "LibFC UDPCollector starting on " << netaddr_.sessionname() << " : ";
      sock_ = netaddr_.create_socket();
      std::cerr << "socket " << sock_ << std::endl;
  }

  // receive a packet into an array
  ssize_t len = recvfrom(sock_, packbuf, 65536, 0, reinterpret_cast<struct sockaddr*>(&sa), &sa_len);
  
  if (len == -1) {
    // FIXME handle errors consistently
    std::cerr << "recvfrom() returned -1" << std::endl;
    return false;
  }
  
  // FIXME slow -- get a session name for each packet  
  std::string sk = NetAddress(reinterpret_cast<struct sockaddr*>(&sa), sa_len, IPPROTO_UDP).sessionname();
  
  // wrap the packet in an istringstream
  std::string packstr(packbuf, len);
  std::istringstream packis(packstr);

  session = getSession(sk);

  return mbuf.deframe(packis, *session);      
}

UDPCollector::~UDPCollector() {
  if (sock_) {
    close(sock_);
    sock_ = -1;
  }
}


}
