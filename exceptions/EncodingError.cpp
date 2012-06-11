#include "EncodingError.h"

namespace IPFIX {

  EncodingError::EncodingError(const std::string& message)
    : Exception("EncodingError: " + message) {}

} // namespace IPFIX
