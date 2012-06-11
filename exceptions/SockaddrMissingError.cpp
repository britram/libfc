#include "SockaddrMissingError.h"

namespace IPFIX {

  SockaddrMissingError::SockaddrMissingError() 
    : Exception(std::string("can't get a port without a struct sockaddr")) {
  }
  
} // namespace IPFIX
