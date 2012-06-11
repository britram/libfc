/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_SOCKADDRMISSINGERROR_H
#  define IPFIX_SOCKADDRMISSINGERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** Missing socket address.
   *
   * This is a runtime error thrown when a port number is requested,
   * but no socket address has been provided yet.
   */
  class SockaddrMissingError : public Exception {
  public:
    explicit SockaddrMissingError();
  };

} // namespace IPFIX

#endif /* IPFIX_SOCKADDRMISSINGERROR_H */
