#include "MTUError.h"

namespace IPFIX {

  MTUError::MTUError(const std::string& additional_info) 
    : Exception(std::string("MTU too small")
                + (additional_info == "" ? "" : ": ") 
                + additional_info) {
  }
  
} // namespace IPFIX
