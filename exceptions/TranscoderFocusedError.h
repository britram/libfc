/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_TRANSCODERFOCUSEDERROR_H
#  define IPFIX_TRANSCODERFOCUSEDERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** Attempt to focus a transcoder that is already focused.
   */
  class TranscoderFocusedError : public Exception {
  public:
    explicit TranscoderFocusedError(const std::string& additional_info = "");
  };

} // namespace IPFIX

#endif /* IPFIX_TRANSCODERFOCUSEDERROR_H */
