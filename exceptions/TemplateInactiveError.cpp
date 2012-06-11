#include "TemplateInactiveError.h"

namespace IPFIX {

  TemplateInactiveError::TemplateInactiveError(const std::string& additional_info) 
    : Exception(std::string("can't do this with an inactive template")
                + (additional_info == "" ? "" : ": ") 
                + additional_info) {
  }
  
} // namespace IPFIX
