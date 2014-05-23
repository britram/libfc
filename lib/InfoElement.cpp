/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of ETH Zürich nor the names of other contributors 
 *      may be used to endorse or promote products derived from this software 
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH 
 * ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <sstream>
#include "InfoElement.h"

namespace LIBFC {
  
#if 0
  InfoElement::InfoElement()
    : name_(std::string()),
      pen_(0), 
      number_(0), 
      ietype_(IEType::octetArray()),
      len_(0) {
  }
#endif

  InfoElement::InfoElement(const InfoElement& rhs)
    : name_(rhs.name()),
      pen_(rhs.pen()),
      number_(rhs.number()),
      ietype_(rhs.ietype()),
      len_(rhs.len()) {
  }   

  InfoElement::InfoElement(const std::string &name, 
			   uint32_t pen, 
			   uint16_t number, 
			   const IEType* ietype, 
			   uint16_t len)
    : name_(name),
      pen_(pen),
      number_(number),
      ietype_(ietype),
      len_(len) {
  }

  InfoElement::InfoElement(const InfoElement &rhs, uint16_t nlen)
    : name_(rhs.name()),
      pen_(rhs.pen()),
      number_(rhs.number()),
      ietype_(rhs.ietype()),
      len_(nlen) {
  }

  const std::string& InfoElement::name() const {
    return name_;
  }

  uint32_t InfoElement::pen() const {
    return pen_;
  }

  uint16_t InfoElement::number() const {
    return number_;
  }
  
  const IEType* InfoElement::ietype() const {
    return ietype_;
  }
  
  uint16_t InfoElement::len() const {
    return len_;
  }

  std::string InfoElement::toIESpec() const {
    std::ostringstream os;

    os << name_ << "(";
    if (pen_) os << pen_ << "/";
    os << number_ << ")" << "<" << ietype_->name() << ">" << "[" << len_ << "]";
  
    return os.str();
  };

  const InfoElement* InfoElement::forLen(uint16_t len) const {
    if (len_ == len || len == 0) return this;
    
    if (!rle_[len]) {
      rle_[len] = std::shared_ptr<const InfoElement>(new InfoElement(*this, len));
    }
    
    return rle_[len].get();
  }

  bool InfoElement::matches(const InfoElement& rhs) const { 
    return (rhs.pen() == pen_) && (rhs.number() == number_);
  }

}
