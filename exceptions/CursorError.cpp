#include "CursorError.h"

namespace IPFIX {

  CursorError::CursorError(const std::string& message)
    : Exception("CursorError: " + message) {}

} // namespace IPFIX
