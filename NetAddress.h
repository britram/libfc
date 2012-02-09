/**
 * @file
 *
 * NetAddress represents a network (peer) endpoint (address, protocol, port),
 * whether looked up from a name or derived from a socket address.
 */
 
namespace IPFIX {
    
    class NetAddress{
        
    public:
        NetAddress(const std::string* hostname, const std::string* servname, uint8_t proto):
            hostname_(hostname),
            servname_(servname),
            proto_(proto),
            ai_(NULL) {}

        NetAddress(const sockaddr_storage* sa):
            hostname_(), servname_(), proto_(0), ai_(NULL);
        {
            memcpy(&sa_, sa, sizeof(*sa));
        }
        
        NetAddress(int peersock);
        
        int create_connected_socket();
        
        int create_passive_socket();
        
        const sockaddr_storage *get_sockaddr();
        
        const std::string& hostname() const;

        const std::string& servname() const;
        
        const uint8_t protocol() const;
        
    private:
        mutable std::string         hostname_;
        mutable std::string         servname_;
        mutable uint8_t             proto_;
        mutable addrinfo*           ai_;
        mutable sockaddr_storage    sa_;
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
