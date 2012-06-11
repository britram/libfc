/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_FORMATERROR_H
#  define IPFIX_FORMATERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** IPFIX Format Errors.
   *
   * This is a runtime error thrown during decoding when a message is
   * poorly formatted.  It usually indicates that the input stream
   * isn't really IPFIX.
   *
   * FIXME make sure this gets used properly.
   */
  class FormatError : public Exception {
  public:
    explicit FormatError(const std::string& message);
  };

} // namespace IPFIX

#endif /* IPFIX_FORMATERROR_H */
