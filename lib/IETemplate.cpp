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
#include <algorithm>

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include "IETemplate.h"

namespace LIBFC {

  IETemplate::IETemplate()
    : minlen_(0) {
  }
   
  bool IETemplate::contains(const InfoElement* ie) const {
    return std::find(ies_.begin(), ies_.end(), ie) != ies_.end();
  }

  bool IETemplate::containsAll(const IETemplate* rhs) const {
    for (auto iter = rhs->begin(); iter != rhs->end(); iter++) {
      if (!contains(*iter)) 
	return false;
    }
    return true;
  }
  
  size_t IETemplate::minlen() const {
    return minlen_;
  } 
  
  std::vector<const InfoElement *>::const_iterator IETemplate::begin() const {
    return ies_.begin();
  } 
  
  std::vector<const InfoElement *>::const_iterator IETemplate::end() const {
    return ies_.end();
  }

  std::vector<const InfoElement *>::const_iterator 
  IETemplate::find(const InfoElement* ie) const {
    return std::find(ies_.begin(), ies_.end(), ie);
  }

  size_t IETemplate::size() const { 
    return ies_.size();
  }

  void IETemplate::add_inner(const InfoElement* ie) {
    ies_.push_back(ie);
  }

  void IETemplate::add(const InfoElement* ie) {
    add_inner(ie);

    if (ie->len() == kIpfixVarlen)
      minlen_ += 1;
    else
      minlen_ += ie->len();
  }

  bool IETemplate::operator==(const IETemplate& rhs) const {
    auto l = begin();
    auto r = rhs.begin();

    while (l != end() && r != rhs.end()) {
      if (*l != *r)
	return false;
      ++l;
      ++r;
    }

    return l == end() && r == rhs.end();
  }

  bool IETemplate::operator!=(const IETemplate& rhs) const {
    return !(*this == rhs);
  }

} // namespace LIBFC
