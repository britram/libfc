#include "TemplateActiveError.h"

namespace IPFIX {

  TemplateActiveError::TemplateActiveError(const std::string& additional_info) 
    : Exception(std::string("can't do this with an active template")
                + (additional_info == "" ? "" : ": ") 
                + additional_info) {
  }
  
} // namespace IPFIX
