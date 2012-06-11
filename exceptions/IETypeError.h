/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_IETYPEERROR_H
#  define IPFIX_IETYPEERROR_H

#  include "exceptions/Exception.h"

namespace IPFIX {

  /** A logic error indicating a programming fault concerning the
   * handling of IPFIX types.
   */
  class IETypeError : public Exception {
  public:
    explicit IETypeError(const std::string& message);
  };

} // namespace IPFIX

#endif /* IPFIX_IETYPEERROR_H */
