/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_TEMPLATEINACTIVEERROR_H
#  define IPFIX_TEMPLATEINACTIVEERROR_H

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
  class TemplateInactiveError : public Exception {
  public:
    explicit TemplateInactiveError(const std::string& additional_info = "");
  };

} // namespace IPFIX

#endif /* IPFIX_TEMPLATEINACTIVEERROR_H */
