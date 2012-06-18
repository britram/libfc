/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_CURSORERROR_H
#  define IPFIX_CURSORERROR_H

#  include "Exception.h"

namespace IPFIX {

  /** IPFIX Cursor Errors.
   *
   * Runtime error thrown when a cursor is not properly used.
   */
  class CursorError : public Exception {
  public:
    explicit CursorError(const std::string& message);
  };

} // namespace IPFIX

#endif /* IPFIX_CURSORERROR_H */
