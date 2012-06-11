#include "IESpecError.h"

namespace IPFIX {

  IESpecError::IESpecError(const std::string& additional_info) 
    : Exception(std::string("IE specification error")
                + (additional_info == "" ? "" : ": ") 
                + additional_info) {
  }
  
} // namespace IPFIX
