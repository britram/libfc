/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_ADDRTYPEUNSUPPORTEDERROR_H
#  define IPFIX_ADDRTYPEUNSUPPORTEDERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** Address type is unsupported.
   *
   * This is a runtime error indicating that the address type used is
   * not supported.  At the moment, only PF_INET and PF_INET6 are
   * supported.
   */
  class AddrtypeUnsupportedError : public Exception {
  public:
    explicit AddrtypeUnsupportedError(int protocol);
  };

} // namespace IPFIX

#endif /* IPFIX_ADDRTYPEUNSUPPORTEDERROR_H */
