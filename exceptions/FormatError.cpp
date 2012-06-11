#include "FormatError.h"

namespace IPFIX {

  FormatError::FormatError(const std::string& message)
    : Exception("FormatError: " + message) {}

} // namespace IPFIX
