#include "IOError.h"

namespace IPFIX {

  IOError::IOError(const std::string& message)
    : Exception("IOError: " + message) {}

} // namespace IPFIX
