#include <string>

#include "IEUnknownError.h"

namespace IPFIX {

  IEUnknownError::IEUnknownError(uint32_t iepen, uint16_t ienum, uint16_t ielen) 
    : Exception(std::string("IE (") + std::to_string(iepen)
                + ", " + std::to_string(ienum)
                + ", " + std::to_string(ielen)
                + ") unknown") {
  }
  
} // namespace IPFIX
