#include "TranscoderDefocusedError.h"

namespace IPFIX {

  TranscoderDefocusedError::TranscoderDefocusedError(const std::string& additional_info) 
    : Exception(std::string("can't do this with a defocused transcoder")
                + (additional_info == "" ? "" : ": ") 
                + additional_info) {
  }
  
} // namespace IPFIX
