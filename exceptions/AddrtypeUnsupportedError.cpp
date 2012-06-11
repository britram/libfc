#include <string>

#include "AddrtypeUnsupportedError.h"

namespace IPFIX {

  AddrtypeUnsupportedError::AddrtypeUnsupportedError(int protocol) 
    : Exception(std::string("address type ")
                + std::to_string(protocol)
                + " unsupported") {
  }
  
} // namespace IPFIX
