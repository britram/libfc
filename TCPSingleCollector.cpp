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

#include <TCPSingleCollector.h>
#include <sys/socket.h>

namespace IPFIX {
    
bool TCPSingleCollector::ensureSocket() {
    struct sockaddr_storage sa;
    socklen_t sa_len;

    // Short-circuit if we have a receiver socket
    if (fp_) return true;
    
    // First make sure we have a listener
    if (lsock_ < 0) {
        lsock_ = netaddr_.create_socket();
        if (lsock_ < 0) {
            // FIXME handle error
            return false;
        }
        
        // FIXME check return
        if (listen(lsock_, 5)) {
            // FIXME handle error
            return false;
        }
    
        std::cerr << "ensureSocket() listening on TCP passive socket " << lsock_ << std::endl;
    }
    
    // We have a listener socket, and we don't have a receiver socket.
    // Accept a connection. This blocks the thread.
    sock_ = accept(lsock_, reinterpret_cast<struct sockaddr*>(&sa), &sa_len);
    
    if (sock_ == -1) {
        // FIXME handle error
        return false;
    }
    
    std::cerr << "ensureSocket() got TCP socket " << sock_ << std::endl;
    
    // Wrap the socket in a file pointer (buffers segment boundaries)
    fp_ = fdopen(sock_, "r");
    
    // We have a socket. Bounce the session.
    session_->reset();
    return true;
}
  
bool TCPSingleCollector::_receiveMessage(MBuf& mbuf, std::shared_ptr<Session>& session) {

  // make sure we have a socket to read from
  if (!ensureSocket()) return false;

  // return session (we're a single TCP reader, we only have one)
  session = session_;
  
  // deframe directly from the socket
  return mbuf.deframe(fp_, *session_);
  
  // FIXME need to handle closed socket
}

TCPSingleCollector::~TCPSingleCollector() {
  if (fp_) {
    fclose(fp_);
    fp_ = NULL;
    sock_ = -1;
  }

  if (lsock_ >= 0) {
    close(lsock_);
    lsock_ = -1;
  }
}

}
