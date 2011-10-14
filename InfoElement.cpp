#include <sstream>
#include "InfoElement.h"

namespace IPFIX {

std::string InfoElement::toIESpec() const {
  std::ostringstream os;

  os << name_ << "(";
  if (pen_) os << pen_ << "/";
  os << number_ << ")";
  os << "<" << ietype_->name() << ">";
  os << "[" << len_ << "]";
  
  return os.str();
};

const InfoElement* InfoElement::forLen(uint16_t len) const {
  if (len_ == len || len == 0) return this;
  
  if (!rle_[len]) {
    rle_[len] = InfoElement_SP(new InfoElement(*this, len));
  }

  return rle_[len].get();
}

}
