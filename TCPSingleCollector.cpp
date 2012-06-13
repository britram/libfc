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
