/* Copyright (c) 2011, NEC Europe Ltd, Consorzio Nazionale 
 * Interuniversitario per le Telecomunicazioni, Institut 
 * Telecom/Telecom Bretagne, ETH Zürich, INVEA-TECH a.s. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of NEC Europe Ltd, Consorzio Nazionale 
 *      Interuniversitario per le Telecomunicazioni, Institut Telecom/Telecom 
 *      Bretagne, ETH Zürich, INVEA-TECH a.s. nor the names of its contributors 
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
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "NetAddress.h"

namespace IPFIX {

int NetAddress::create_socket_sa() {    
    assert(sa_valid_);
    /* grab family from sockaddr */
    family_ = reinterpret_cast<struct sockaddr*>(&sa_)->sa_family;
    
    int sock = socket(family_, 
                      socktype(), 
                      proto_);
    if (sock < 0) return sock;
    
    int rv;
    if (passive_) {
        rv = bind(sock, reinterpret_cast<struct sockaddr*>(&sa_), addrlen());
    } else {
        rv = connect(sock, reinterpret_cast<struct sockaddr*>(&sa_), addrlen());
    }
    
    if (rv < 0) {
        close(sock);
        return -1;
    }
    
    return sock;
}

int NetAddress::create_socket_ai() {
    addrinfo hints, *lai, *ai;
    int ai_error;

    hints.ai_flags = AI_ADDRCONFIG;
    if (passive_) hints.ai_flags |= AI_PASSIVE;
    hints.ai_family = family_;
    hints.ai_socktype = socktype();
    hints.ai_protocol = proto_;

    const char* hostname_cs;
    if (hostname_.length() == 0 || hostname_ == "*") {
        hostname_cs = NULL;
    } else {
        hostname_cs = hostname_.c_str();
    }

    const char* servname_cs;
    if (servname_.length() == 0) {
        servname_cs = NULL;
    } else {
        servname_cs = servname_.c_str();
    }
        
    if (hostname_cs) {
        std::cerr << "hostname_cs in create_socket_ai is " << hostname_cs << std::endl;
    } else {
        std::cerr << "hostname_cs in create_socket_ai is NULL" << std::endl;        
    }

    if ((ai_error = getaddrinfo(hostname_cs, servname_cs, &hints, &lai))) {
        // FIXME handle lookup error -- cache this in the object itself?
        std::cerr << "lookup error " << ai_error << std::endl;
        return -1;                
    }
    
    int sock = -1;
    for (ai = lai; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, 
                      ai->ai_socktype, 
                      ai->ai_protocol);
        if (sock < 0) continue;
        
        int rv;
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

int NetAddress::socktype() const {
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

size_t NetAddress::addrlen() const {
    assert(family_ == PF_INET || family_ == PF_INET6);
    return family_ ==  PF_INET 
      ? sizeof(struct sockaddr_in) 
      : sizeof(struct sockaddr_in6);
}

uint16_t NetAddress::port() const {
    assert(sa_valid_);
    assert(family_ == PF_INET || family_ == PF_INET6);
    return family_ == PF_INET
      ? ntohs(reinterpret_cast<const struct sockaddr_in*>(&sa_)->sin_port)
      : ntohs(reinterpret_cast<const struct sockaddr_in6*>(&sa_)->sin6_port);
}

void NetAddress::cache_names() const {
    if (!hostname_.length()) {
        if (nil_host_) {
            hostname_ = std::string("*");
        } else {
            char pcstr[INET6_ADDRSTRLEN];
            inet_ntop(family_, &sa_, pcstr, INET6_ADDRSTRLEN);
            hostname_ = std::string(pcstr);
        }
    }
    
    if (!servname_.length()) {
      servname_ = std::to_string(port());
    }
    
    if (!sessionname_.length()) {
        sessionname_ = hostname_ + "/" + 
                       servname_ + "/" +
          std::to_string(proto_);
    }
}


}
