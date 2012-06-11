/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_MTUERROR_H
#  define IPFIX_MTUERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** Logic error signifying that a too-small MTU has been set on an
   * Exporter for a given message.
   */
  class MTUError : public Exception {
  public:
    explicit MTUError(const std::string& additional_info = "");
  };

} // namespace IPFIX

#endif /* IPFIX_MTUERROR_H */
