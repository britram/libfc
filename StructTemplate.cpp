#include "StructTemplate.h"

namespace IPFIX {
  
void StructTemplate::add(const InfoElement* ie, size_t offset) {
  // Add IE to maps
  add_inner(ie);
  
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
  
  // Check offset monotonity invariant
  if (!offsets_.empty() && offset < offsets_.back()) {
    throw IETypeError("Cannot add IE with negative offset");
  }

  // Add offset to offset table
  offsets_.push_back(offset);
  
  // calculate minlen
  size_t ielen = ie->len();
  if (ielen == kVarlen) ielen = sizeof(VarlenField);  
  if ((offset + ielen) > minlen_) minlen_ = offset + ielen;
}

}