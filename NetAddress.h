/**
 * @file
 *
 * NetAddress represents a network (peer) endpoint (address, protocol, port),
 * whether looked up from a name or derived from a socket address.
 */

#include <string>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <boost/lexical_cast.hpp>

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
            passive_(passive),
            sa_valid_(false)
            {}

        NetAddress(const struct sockaddr* sa, size_t sa_len, uint8_t proto):
            hostname_(),
            servname_(), 
            proto_(proto),
            family_(PF_UNSPEC),
            passive_(false),
            sa_valid_(false)
        {
            set_sockaddr(sa, sa_len);
        }

        const struct sockaddr* sockaddr() {
            if (sa_valid_) {
                return reinterpret_cast<struct sockaddr*>(&sa_);
            } else {
                return NULL;
            }
        }
        
        const std::string& hostname() const {
            if (hostname_.length() == 0) cache_names();
            return hostname_;
        }

        const std::string& servname() const {
            if (servname_.length() == 0) cache_names();
            return servname_;
        }

        const std::string& sessionname() const {
            if (sessionname_.length() == 0) cache_names();
            return sessionname_;
        }
        
        const uint8_t protocol() const {
            return proto_;
        }
        
        bool operator==(const NetAddress& rhs) const;

        int create_socket() {
            if (sa_valid_) {
                return create_socket_sa();
            } else {
                return create_socket_ai();
            }
        }
        
    protected:
        
        int socktype() const;
        size_t addrlen() const;
        int create_socket_sa();
        int create_socket_ai();
        uint16_t port() const;
        void cache_names() const;
        
        void set_sockaddr(const struct sockaddr* sa, size_t len) {
            memcpy(&sa_, sa, len);
            family_ = sa->sa_family;
            sa_valid_ = true;            
        }
        
    private:

        mutable std::string         hostname_;
        mutable std::string         servname_;
        mutable std::string         sessionname_;
        
        mutable uint8_t             proto_;
        mutable int                 family_;

        struct sockaddr_storage     sa_;
        bool                        passive_;
        bool                        sa_valid_;
    };
}
