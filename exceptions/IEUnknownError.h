/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_IEUNKNOWNERROR_H
#  define IPFIX_IEUNKNOWNERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** Attempt to retrieve an unknown IE.
   */
  class IEUnknownError : public Exception {
  public:
    explicit IEUnknownError(uint32_t iepen, uint16_t ienum, uint16_t ielen);
  };

} // namespace IPFIX

#endif /* IPFIX_IEUNKNOWNERROR_H */
