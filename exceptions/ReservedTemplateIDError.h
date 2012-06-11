/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_RESERVEDTEMPLATEIDERROR_H
#  define IPFIX_RESERVEDTEMPLATEIDERROR_H

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
  class ReservedTemplateIDError : public Exception {
  public:
    explicit ReservedTemplateIDError(uint16_t tid, uint16_t min_id);
  };

} // namespace IPFIX

#endif /* IPFIX_RESERVEDTEMPLATEIDERROR_H */
