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

#include <vector>
#include <cstring>
#include <iostream>
#include <stdint.h>
#include <boost/unordered_map.hpp>
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
  
typedef boost::unordered_map<const InfoElement *, size_t, InfoElement::ptrIdHash, InfoElement::ptrIdEqual> 
                                                 IndexMap;

class IETemplate {
  
public:
    
  size_t minlen() const { return minlen_; } 

  void activate() {
    if (active_) {
      throw std::logic_error("Cannot activate active template");
    }

    //std::cerr << "activate template (" << domain_ << "," << tid_ << "), " << ies_.size() << " IEs" << std::endl;
    active_ = true;
  }

  bool active() const { return active_; }

  bool contains(const InfoElement* ie) const {
    return index_map_.count(ie);
  }
  
  bool containsAll(const IETemplate* rhs) const {
    for (IETemplateIter iter = rhs->begin(); iter != rhs->end(); iter++) {
      if (!contains(*iter)) return false;
    }
    return true;
  }
  
  size_t offset(const InfoElement* ie) const {
    return offsets_.at(index_map_.at(ie));
  }
  
  size_t length(const InfoElement* ie) const {
    return ies_.at(index_map_.at(ie))->len();
  }

  IETemplateIter begin() const { return ies_.begin(); } 
  
  IETemplateIter end() const { return ies_.end(); }
      
//  void dump(std::ostream& os) const {
//    os << "# Template "<< domain_ << "/" << tid_ << std::endl; 
//    for (IETemplateIter iter = ies_.begin();
//         iter != ies_.end();
//         iter++) {    
//      os << (*iter)->toIESpec() << std::endl;
//    }
//  }
  
  virtual void add(const InfoElement* ie, size_t offset = 0) = 0;
    
protected:
  
  IETemplate() : 
    minlen_(0),
    trlen_(4),
    active_(false) {}
    
  // pointer to session containing template
  // vector of information elements
  std::vector <const InfoElement *>        ies_;
  // vector of information element offsets
  std::vector <size_t>                     offsets_;
  // map of information elements back to vector indices
  IndexMap                                 index_map_;
  // minimum length of record represented by template
  size_t                                   minlen_;
  // length of template record
  size_t                                   trlen_;
  // flag to indicate whether template is active. active templates
  // can be used to encode and decode, inactive templates can be
  // added to.
  bool                                     active_;
};

} // namespace IPFIX

#endif