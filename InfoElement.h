/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 *
 * This file defines InfoElement, which represents an IPFIX Information Element
 */

#ifndef IPFIX_INFOELEMENT_H // idem
#define IPFIX_INFOELEMENT_H // hack

#include <string>
#include <tr1/memory>
#include <stdint.h>
#include "IEType.h"

namespace IPFIX {

/**
 * Immutable class representing an IPFIX Information Element. InfoElements 
 * have a name, number, size, and type. To support reduced-length encoding, an
 * InfoElement may keep a reference to different-sized versions of itself.
 * Canonical InfoElements are stored within an InfoModel, and only 
 * canonical InfoElements should be used in IETemplates; use the lookupIE() 
 * method on InfoModel to get one for an example (constructed or parsed)
 * InfoElement.
 */
class InfoElement {

public:
  
  /**
   * Create a new, empty InfoElement. Creates a nonsensical, 
   * nameless IE of size 0; used primarily 
   * for sticking InfoElements in STL containers before assignment.
   */
  InfoElement(): 
    name_(std::string()),
    pen_(0), 
    number_(0), 
    ietype_(IEType::octetArray()),
    len_(0) {}

  /**
   * Create a new InfoElement by copying an existing one.
   *
   * @param rhs the IE to copy
   */
  InfoElement(const InfoElement &rhs):
    name_(rhs.name()),
    pen_(rhs.pen()),
    number_(rhs.number()),
    ietype_(rhs.ietype()),
    len_(rhs.len()) {}   

  /**
   * Create a new InfoElement given values for its fields.
   *
   * @param name the IE name as a std::string
   * @param pen the private enterprise number (0 for IANA IEs)
   * @param number the IE number (with the enterprise bit set to 0)
   * @param ietype pointer to the IEType representing the IE's type; get from
   *               InfoModel::lookupIEType() or one of the IEType statics.
   * @param len the length of the Information Element
   */
  InfoElement(const std::string &name, 
              uint32_t pen, 
              uint16_t number, 
              const IEType* ietype, 
              uint16_t len):
    name_(name),
    pen_(pen),
    number_(number),
    ietype_(ietype),
    len_(len) {}
    
  /**
   * Create a new InfoElement by copying an existing one and changing its size
   *
   * @param rhs the IE to copy
   * @param nlen the new length
   */
  InfoElement(const InfoElement &rhs, uint16_t nlen):
    name_(rhs.name()),
    pen_(rhs.pen()),
    number_(rhs.number()),
    ietype_(rhs.ietype()),
    len_(nlen) {}   
    
  /** 
   * Get the IE's name
   *
   * @return the IE's name
   */
  const std::string& name() const { return name_; }

  /** 
   * Get the IE's private enterprise number.
   *
   * @return the IE's PEN. Returns 0 if the IE is in the IANA registry.
   */
  uint32_t pen() const { return pen_; }

  /** 
   * Get the IE's number.
   *
   * @return the IE's number. The enterprise bit is cleared, even if the IE 
   *         is enterprise-specific.
   */
  uint16_t number() const { return number_; }
  
  /**
   * Get a pointer to the IE's type
   *
   * @return a pointer to the IE's type
   */
  const IEType* ietype() const { return ietype_; }
  
  /** 
   * Get the IE's length.
   *
   * @return the IE's length.
   */  
  uint16_t len() const { return len_; }

  /**
   * Get an IE derived from or identical to this IE for a given length
   *
   * @return a pointer to the IE's type
   */
  const InfoElement* forLen(uint16_t len) const;

  /**
   * Determine whether two IE's match each other for purposes of template 
   * compatibility, based on number and PEN only.
   *
   * @param rhs the InfoElement to compare to
   * @return true if the number and PEN match
   */
  bool matches(const InfoElement& rhs) const { 
    return (rhs.pen() == pen_) && (rhs.number() == number_);
  }

  /**
   * Get a complete IESpec for this InfoElement.
   *
   * @return an iespec
   */ 
  std::string toIESpec() const;

  /**
   * Helper class for hashmapping IE by pointer based on number and PEN.
   */
  class ptrIdHash {
  public:
    std::size_t operator() (const InfoElement* ie) const { 
      return ie->pen() | ie->number(); 
    } 
  };

  /**
   * Helper class for comparing IE by pointer based on number and PEN.
   */
  class ptrIdEqual {
  public:
    bool operator() (const InfoElement* lhs, const InfoElement* rhs) const { 
      return lhs->matches(*rhs); 
    }  
  };
  
private:
  std::string   name_;
  uint32_t      pen_;
  uint16_t      number_;
  const IEType* ietype_;
  uint16_t      len_;
  mutable std::map<uint16_t, std::tr1::shared_ptr<InfoElement> > rle_;
};

}


#endif // idem hack
