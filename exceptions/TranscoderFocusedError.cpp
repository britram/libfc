#include "TranscoderFocusedError.h"

namespace IPFIX {

  TranscoderFocusedError::TranscoderFocusedError(const std::string& additional_info) 
    : Exception(std::string("can't do this with a focused transcoder")
                + (additional_info == "" ? "" : ": ") 
                + additional_info) {
  }
  
} // namespace IPFIX
