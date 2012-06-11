#include "IETypeError.h"

namespace IPFIX {

  IETypeError::IETypeError(const std::string& message)
    : Exception(message) {
  }

} // namespace IPFIX
