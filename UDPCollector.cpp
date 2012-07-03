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
  
bool UDPCollector::_receiveMessage(MBuf& mbuf, std::shared_ptr<Session>& session) {

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
