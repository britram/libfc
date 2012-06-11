/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_IOERROR_H
#  define IPFIX_IOERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** IPFIX I/O Errors.
   *
   * Runtime error thrown when a low-level IO error occurs during
   * transcoding.
   *
   * FIXME make sure this gets used properly.
   */
  class IOError : public Exception {
  public:
    explicit IOError(const std::string& message);
  };

} // namespace IPFIX

#endif /* IPFIX_IOERROR_H */
