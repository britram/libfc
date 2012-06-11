/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_TRANSCODERDEFOCUSEDERROR_H
#  define IPFIX_TRANSCODERDEFOCUSEDERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** Attempt to defocus a transcoder that is already defocused.
   */
  class TranscoderDefocusedError : public Exception {
  public:
    explicit TranscoderDefocusedError(const std::string& additional_info = "");
  };

} // namespace IPFIX

#endif /* IPFIX_TRANSCODERDEFOCUSEDERROR_H */
