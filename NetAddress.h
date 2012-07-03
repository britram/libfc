/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of ETH Zürich nor the names of other contributors 
 *      may be used to endorse or promote products derived from this software 
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/**
 * @file
 *
 * NetAddress represents a network (peer) endpoint (address, protocol, port),
 * whether looked up from a name or derived from a socket address.
 */
#ifndef IPFIX_NETADDRESS_H // idem
#define IPFIX_NETADDRESS_H // hack

#include <string>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <memory.h>


namespace IPFIX {
    
    class NetAddress {
        
    public:
        NetAddress(const std::string& hostname, 
                   const std::string& servname, 
                   uint8_t proto, 
                   int family = PF_UNSPEC, 
                   bool passive = false):
            hostname_(hostname),
            servname_(servname),
            sessionname_(),
            proto_(proto),
            family_(family),
            passive_(passive),
            nil_host_(hostname.length() == 0),
            sa_valid_(false) {}

        NetAddress(const std::string& servname, uint8_t proto, int family):
            hostname_(),
            servname_(servname),
            sessionname_(),
            proto_(proto),
            family_(family),
            passive_(true),
            nil_host_(true),
            sa_valid_(false)
            {}

        NetAddress(const struct sockaddr* sa, size_t sa_len, uint8_t proto):
            hostname_(),
            servname_(), 
            sessionname_(),
            proto_(proto),
            family_(PF_UNSPEC),
            passive_(false),
            nil_host_(false),
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
        bool                        nil_host_;
        bool                        sa_valid_;
    };
}

#endif //idem hack
