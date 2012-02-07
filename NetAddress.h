/**
 * @file
 *
 * NetAddress represents a network (peer) endpoint (address, protocol, port),
 * whether looked up from a name or derived from a socket address.
 */
 
namespace IPFIX {
    
    class NetAddress{
        
        static const uint8_t NA_UDP = 17;
        static const uint8_t NA_TCP = 6;
        
    public:
        NetAddress(const std::string& host, const std::string& port, uint8_t protocol);

        NetAddress(const std::string& host, uint16_t port, uint8_t protocol);
        
        NetAddress(const sockaddr_storage* sa);
        
        NetAddress(int peersock);
        
        int create_connected_socket();
        
        const sockaddr_storage *get_sockaddr();
        
        const std::string& hostaddr();
        
        const uint16_t port();
        
        const uint8_t protocol();
        
    private:
        // FIXME figure out what the backing store is
    };
}
  // 
  // struct addrinfo*  ai;
  // struct addrinfo   hints;
  // int ai_err;
  // 
  // memset(&hints, 0, sizeof(hints));
  // hints.ai_flags = AI_ADDRCONFIG;
  // hints.ai_family = PF_UNSPEC;
  // hints.ai_socktype = SOCK_DGRAM;
  // hints.ai_protocol = IPPROTO_UDP;
  // 
  // if ((ai_err = getaddrinfo(host_.c_str(), port_.c_str(), &hints, &ai))) {
  //   throw IOError("UDP export address lookup failed");
  // }
  // 
  // do {
  //   fd_ = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
  //   if (fd_ < 0) continue;
  //   if (!connect(fd_, ai->ai_addr, ai->ai_addrlen)) break;
  //   close(fd_);
  //   fd_ = -1;
  // } while ((ai = ai->ai_next));
  // 
  // freeaddrinfo(ai);
  // 
  // if (fd_ < 0) {
  //   throw IOError("UDP export connection failed");
  // }
