#include "StructTemplate.h"

namespace IPFIX {
  
void StructTemplate::add(const InfoElement* ie, size_t offset) {
  // Can't add to an active template
  if (active_) {
    throw std::logic_error("Cannot add IEs to an active template");
  }

  // Add the IE to the IE vector
  ies_.push_back(ie);
  
  // Add the IE to the index map
  index_map_[ie] = ies_.size()-1;

  // Calculate offset if necessary
  if (!offset) {
    if (offsets_.empty()) {
      offset = 0;
    } else if (ie->len() == kVarlen) {
      offset = offsets_.back() + sizeof(VarlenField);
    } else {
      offset = offsets_.back() + ie->len();
    }
  }
  
  // Check offset monotonity invariant (only in danger for two-argument form)
  if (!offsets_.empty() && offset < offsets_.back()) {
    throw IETypeError("Cannot add IE with negative offset");
  }

  // Add offset to offset table
  offsets_.push_back(offset);
  
  // Add the length of the IE to the minimum length
  // (This won't track in the two-argument form, but we'll never use it there)
  if (ie->len() == kVarlen) {
    minlen_ += sizeof(VarlenField);
  } else {
    minlen_ += ie->len();
  }
  
  // Add the length of the IE's fields to the template record length
  trlen_ += ie->pen() ? 8 : 4;
}

}