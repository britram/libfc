#include <string>

#include "ReservedTemplateIDError.h"

namespace IPFIX {

  ReservedTemplateIDError::ReservedTemplateIDError(uint16_t tid,
                                                   uint16_t min_id) 
    : Exception(std::string("attempt to set template with reserved ID ")
                + std::to_string(tid) + " (must be at least " 
                + std::to_string(min_id) + ")") {
  }
  
} // namespace IPFIX
