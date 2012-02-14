/**
 * @file
 *
 * NetAddress represents a network (peer) endpoint (address, protocol, port),
 * whether looked up from a name or derived from a socket address.
 */

// Stores itself as a sockaddr_storage 

namespace IPFIX {
    
    class NetAddress {
        
    public:
        NetAddress(const std::string& hostname, 
                   const std::string& servname, 
                   uint8_t proto, 
                   int family, 
                   bool passive):
            hostname_(hostname),
            servname_(servname),
            proto_(proto),
            family_(family),
            passive_(passive)
            sa_valid_(false)
            {}

        NetAddress(const sockaddr* sa, size_t sa_len, uint8_t proto):
            hostname_(),
            servname_(), 
            proto_(proto),
            family_(PF_UNSPEC),
            passive_(false),
            sa_valid_(false)
        {
            set_sockaddr(sa, sa_len);
        }

        const sockaddr *sockaddr() {
            if (sa_valid_) {
                return &sa_;
            } else {
                return NULL;
            }
        }
        
        const std::string& hostname() const {
            if (hostname_.length == 0) cache_names();
            return hostname_;
        }

        const std::string& servname() const {
            if (servname_.length == 0) cache_names();
            return servname_;
        }
        
        const uint8_t protocol() const {
            return proto_;
        }
        
        bool operator==(const NetAddress& rhs) const;

        // FIXME passive should be a property of the netaddress, no?
        int create_socket(bool passive) {
            if (sa_valid_) {
                return create_socket_sa(passive);
            } else {
                return create_socket_ai(passive);
            }
        }

    protected:        

        const int socktype() const {
            switch(proto_) {
                case IPPROTO_TCP:
                return SOCK_STREAM;
                case IPPROTO_UDP:
                return SOCK_DGRAM;
                case IPPROTO_SCTP:
                return SOCK_SEQPACKET;
                default:
                return 0;
            }
        }

        const size_t addrlen() const {
            if (!sa_valid_) 
            switch (family_) {
                case PF_INET:
                return sizeof(sockaddr_in);
                case PF_INET6:
                return sizeof(sockaddr_in6);
                default:
                throw std::argument_error("unsupported addrtype");
            }
        }

        int create_socket_sa() {
            /* grab family from sockaddr */
            family_ = static_cast<sockaddr*>(&sa_)->sa_family;
            
            int sock = socket(family_, 
                              socktype(), 
                              proto_);
            if (sock < 0) return sock;
            
            if (passive_) {
                rv = bind(sock, sa_, addrlen());
            } else {
                rv = connect(sock, sa_, addrlen());
            }
            
            if (rv < 0) {
                close(sock);
                return -1;
            }
            
            return sock;
        }

        int create_socket_ai() {
            addrconfig hints, *lai, *ai;
            int ai_error;

            hints.ai_flags = AI_ADDRCONFIG;
            if (passive) hints.ai_flags |= AI_PASSIVE;
            hints.ai_family = family_;
            hints.ai_socktype = socktype();
            hints.ai_proto = proto_;

            char* hostname_cs = hostname_.length ? hostname_.c_str : NULL;
            char* servname_cs = servname_.length ? servname_.c_str : NULL;

            if ((ai_error = getaddrinfo(hostname_cs, servname_cs, &hints, &ai))) {
                // FIXME handle lookup error -- cache this in the object itself?
                return -1;                
            }
            
            for (ai = lai; ai; ai = ai->ai_next) {
                int sock = socket(ai->ai_family, 
                                  ai->ai_socktype, 
                                  ai->ai_protocol);
                if (sock < 0) continue;

                if (passive_) {
                    // FIXME only binds the first bindable
                    rv = bind(sock, ai->ai_addr, ai->ai_addrlen);
                } else {
                    rv = connect(sock, ai->ai_addr, ai->ai_addrlen);
                }
                if (rv < 0) {
                    close(sock);
                    sock = -1;
                    continue;                    
                }
                
                // store the address
                set_sockaddr(ai->ai_addr, ai->ai_addrlen);
                break;
            }
            
            freeaddrinfo(lai);
            return sock;
        }
        
        void set_sockaddr(const sockaddr* sa, size_t len) {
            memcpy(sa_, sa, len);
            family_ = sa->sa_family;
            sa_valid_ = true;            
        }

        void cache_names() const {
            char pcstr[INET6_ADDRSTRLEN];
            inet_ntop(family_, sa_, pcstr, INET6_ADDRSTRLEN);
            hostname_ = std::string(pcstr);
            servname_ = boost::lexical_cast<std::string>(port());
        }

        const uint16_t port() const {
            if (!sa_valid_) {
                throw std::argument_error("can't get a port without a sockaddr");
            }
            switch(family_) {
                case PF_INET:
                return ntohs(static_cast<sockaddr_in*>(&sa_)->sin_port);
                case PF_INET6:
                return ntohs(static_cast<sockaddr_in6*>(&sa_)->sin6_port);
                default:
                return 0;
            }
        }
        
    private:
        sockaddr_storage            sa_
        bool                        sa_valid_;
        
        mutable std::string         hostname_;
        mutable std::string         servname_;
        mutable uint8_t             proto_;
        mutable int                 family_;
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
