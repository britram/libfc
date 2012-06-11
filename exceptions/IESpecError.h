/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_IESPECERROR_H
#  define IPFIX_IESPECERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** A runtime error signifying a problem with parsing or handling an IESpec.
   */
  class IESpecError : public Exception {
  public:
    explicit IESpecError(const std::string& additional_info = "");
  };

} // namespace IPFIX

#endif /* IPFIX_IESPECERROR_H */
