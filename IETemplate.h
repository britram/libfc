/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
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
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * Defines the abstract template interface.
 *
 * A template is an ordered list of Information Elements describing
 * a record format. Client code will use either WireTemplate, which 
 * represents a template for encoding/decoding IPFIX messages on
 * the wire; or StructTemplate, which represents a C struct in memory.
 */

#ifndef IPFIX_IETEMPLATE_H // idem
#define IPFIX_IETEMPLATE_H // hack

#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

#include "InfoElement.h"
#include "Transcoder.h"
#include "Constants.h"

namespace IPFIX {

// struct TCEntry {
//   TCEntry(): 
//           offset(0),
//           length(0),
//           ie(NULL) {}
//           
//   TCEntry(size_t o, size_t l, const InfoElement* i):
//           offset(o), 
//           length(l), 
//           ie(i) {}
//   size_t            offset;
//   size_t            length;
//   const InfoElement *ie;
// };

// forward declarations
class Session;
class IETemplate;

typedef std::vector<const InfoElement *>::const_iterator 
                                                 IETemplateIter;
  
//typedef std::unordererd_map<const InfoElement *, size_t, InfoElement::ptrIdHash, InfoElement::ptrIdEqual> 
typedef std::map<const InfoElement *, size_t> 
                                                 IEIndexMap;

class IETemplate {
  
public:
  
  /**
   * Make this template active. Templates are created inactive.
   * A template can be added to only
   * before it is activated, and used only afterward.
   */
  void activate() {
    assert(!active_);

    active_ = true;
  }

  /**
   * Return true if this template is active.
   */
  bool isActive() const { return active_; }

  /**
   * Determine whether this template contains an instance of the given IE
   *
   * @param ie information element to search for; must be a canonical
   *           IE retrieved from InfoModel::instance().
   * @return true if this template contains the given IE in any size
   */
  bool contains(const InfoElement* ie) const {
    return index_map_.count(ie);
  }
  
  /**
   * Determine whether this template contains all the IEs in a given template.
   *
   * @param rhs template to compare to
   * @return true if this template contains the given IE in any size
   */
  bool containsAll(const IETemplate* rhs) const {
    for (IETemplateIter iter = rhs->begin(); iter != rhs->end(); iter++) {
      if (!contains(*iter)) return false;
    }
    return true;
  }
  
  /**
   * Get the offset of a given IE in this template. 
   * Throws std::out_of_range if the template does not contain the IE.
   * 
   * @param ie information element to search for; must be a canonical
   *           IE retrieved from InfoModel::instance().
   * @return offset to IE in this template
   */
  size_t offset(const InfoElement* ie) const {
    return offsets_.at(index_map_.at(ie));
  }
  
  /**
   * Get the length of a given IE in this template. 
   * Throws std::out_of_range if the template does not contain the IE.
   * 
   * @param ie information element to search for; must be a canonical
   *           IE retrieved from InfoModel::instance().
   * @return length of IE in this template
   */
  size_t length(const InfoElement* ie) const {
    return ies_.at(index_map_.at(ie))->len();
  }
  
  /**
   * Get the IE matching a given IE in the template.
   * Used by Exporter for external encoding.
   * Throws std::out_of_range if the template does not contain the IE.
   * 
   * FIXME kind of breaks encapsulation; consider bringing record 
   * cursor export support into WireTemplate somehow.
   * 
   * @param ie information element to search for; must be a canonical
   *           IE retrieved from InfoModel::instance().
   * @return version of the IE in the template; may differ in length
   */
  const InfoElement* ieFor(const InfoElement* ie) const {
    return ies_.at(index_map_.at(ie));
  }

  /**
   * Get the minimum length of a record represented by this template.
   * 
   * @return minimum record length
   */
  size_t minlen() const { return minlen_; } 
  
  /**
   * Begin iterating on the IEs in this template
   * 
   * @return begin iterator
   */
  IETemplateIter begin() const { return ies_.begin(); } 
  
  /**
   * End iterating on the IEs in this template
   * 
   * @return end iterator
   */
  IETemplateIter end() const { return ies_.end(); }

  /** Returns number of IEs in this template.
   *
   * @return number of IEs in template
   */
  size_t size() const { return ies_.size(); }

  virtual void dumpIdent(std::ostream &os) const = 0;

  void dump(std::ostream& os) const {
    dumpIdent(os);
    os << "  count " << ies_.size() << " minlen " << minlen_ << std::endl;
    for (unsigned i = 0; i < ies_.size(); i ++) {
      os << "    " << ies_[i]->toIESpec() << " off " << offsets_[i] << std::endl;
    }
  }
  
protected:
  
  IETemplate() : 
    minlen_(0),
    active_(false) {}
   
  IETemplate(size_t n_ies) : 
    ies_(n_ies),
    minlen_(0),
    active_(false) {}
  
  /**
   * Add an information element to internal maps; used by subclasses
   */
  void add_inner(const InfoElement* ie) {
    // Can't add to an active template
    assert(!active_);
    
    // Add the IE to the IE vector
    ies_.push_back(ie);
    
    // Add the IE to the index map
    index_map_[ie] = ies_.size()-1;
  }
    
  // pointer to session containing template
  // vector of information elements
  std::vector <const InfoElement *>        ies_;
  // vector of information element offsets
  std::vector <size_t>                     offsets_;
  // map of information elements back to vector indices
  IEIndexMap                               index_map_;
  // minimum length of record represented by template
  size_t                                   minlen_;
  // flag to indicate whether template is active. active templates
  // can be used to encode and decode, inactive templates can be
  // added to.
  bool                                     active_;
};

} // namespace IPFIX

#endif
