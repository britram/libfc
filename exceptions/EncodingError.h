/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_ENCODINGERROR_H
#  define IPFIX_ENCODINGERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** IPFIX Encoding Errors.
   *
   * This is a runtime error thrown during decoding when a message is
   * poorly encodingted.  It usually indicates that the input stream
   * isn't really IPFIX.
   *
   * FIXME make sure this gets used properly.
   */
  class EncodingError : public Exception {
  public:
    explicit EncodingError(const std::string& message);
  };

} // namespace IPFIX

#endif /* IPFIX_ENCODINGERROR_H */
