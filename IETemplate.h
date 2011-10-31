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
  
  /**
   * Make this template active. Templates are created inactive.
   * A template can be added to only
   * before it is activated, and used only afterward.
   */
  void activate() {
    if (active_) {
      throw std::logic_error("Cannot activate active template");
    }

    //std::cerr << "activate template (" << domain_ << "," << tid_ << "), " << ies_.size() << " IEs" << std::endl;
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
   * @param rhs template to 
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
      
//  void dump(std::ostream& os) const {
//    os << "# Template "<< domain_ << "/" << tid_ << std::endl; 
//    for (IETemplateIter iter = ies_.begin();
//         iter != ies_.end();
//         iter++) {    
//      os << (*iter)->toIESpec() << std::endl;
//    }
//  }
  
protected:
  
  IETemplate() : 
    minlen_(0),
    active_(false) {}
  
  /**
   * Add an information element to internal maps; used by subclasses
   */
  void add_inner(const InfoElement* ie) {
    // Can't add to an active template
    if (active_) {
      throw std::logic_error("Cannot add IEs to an active template");
    }
    
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
  IndexMap                                 index_map_;
  // minimum length of record represented by template
  size_t                                   minlen_;
  // flag to indicate whether template is active. active templates
  // can be used to encode and decode, inactive templates can be
  // added to.
  bool                                     active_;
};

} // namespace IPFIX

#endif