/* Copyright (c) 2011, NEC Europe Ltd, Consorzio Nazionale 
 * Interuniversitario per le Telecomunicazioni, Institut 
 * Telecom/Telecom Bretagne, ETH Zürich, INVEA-TECH a.s. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of NEC Europe Ltd, Consorzio Nazionale 
 *      Interuniversitario per le Telecomunicazioni, Institut Telecom/Telecom 
 *      Bretagne, ETH Zürich, INVEA-TECH a.s. nor the names of its contributors 
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
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 */
#ifndef IPFIX_INFOMODEL_H // idem
#define IPFIX_INFOMODEL_H // hack

#include <stdint.h>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include <memory>

#include "IEType.h"
#include "InfoElement.h"

// FIXME enforce thread safety on changes to the infomodel

namespace IPFIX {

/**
 * Represents an IPFIX Information Model, a collection of canonical 
 * Information Elements. 
 */
class InfoModel {
  
public:
  
  /**
   * InfoModel instance accessor.
   *
   * InfoModel is a singleton; this class accessor ensures only one 
   * InfoModel exists at once. 
   *
   * At startup time, your application should initialize this instance
   * using one of the three initialization methods, defaultV9(),
   * defaultIPFIX(), or default5103().
   */
  static InfoModel& instance() {
    static InfoModel instance_;
    return instance_;
  }
  
  /**
   * Default initializer for InfoModel for NetFlowV9 Information Elements.
   *
   * Initializes this InfoModel for NetFlowV9 only. NetFlowV9 applications
   * should call this method once on the default instance.
   */
  void defaultV9();

  /**
   * Default initializer for InfoModel for IPFIX Information Elements.
   *
   * Initializes this InfoModel for IPFIX. IPFIX applications not using 
   * bidirectional flows as per RFC5103 should call this method once on 
   * the default instance returned by instance() at startup.
   */
  void defaultIPFIX();

  
  /**
   * Default initializer for InfoModel for IPFIX Information Elements.
   *
   * Initializes this InfoModel for IPFIX with RFC5103 support. IPFIX 
   * applications using bidirectional flows as per RFC5103 should call 
   * this method once on the default instance returned by instance() at 
   * startup.
   */
  void default5103();
  
  /**
   * Create a new (non-canonical) Information Element by parsing a complete
   * IESpec. The return is suitable for canonicalization via lookupIE() or 
   * addition to the InfoModel via add().
   *
   * @param iespec the IESpec to parse
   * @return a new Information Element
   */
  const InfoElement parseIESpec(const std::string& iespec) const;

  /**
   * Create a new (non-canonical) Information Element by parsing a complete
   * IESpec. The return is suitable for canonicalization via lookupIE() or 
   * addition to the InfoModel via add().
   *
   * @param iespec the IESpec to parse
   * @return a new Information Element
   */
  const InfoElement parseIESpec(const char* iespec) const;
  
  /**
   * Add a copy of an Information Element to the Information Model
   *
   * Idempotent; will only add the Information Element once.
   *
   * @param specie the Information Element to add
   */
  void add(const InfoElement& specie);
  
  /**
   * Add an Information Element described by a complete IESpec to the
   * Information Model
   *
   * @param iespec the Information Element to add as an IESpec
   */
  void add(const std::string& iespec);
  
  /**
   * Add an unknown Information Element with no type information 
   * (as received in a template) to the Information Model. Stores the IE
   * as an octetArray with a generated name.
   *
   * @param pen the private enterprise number to add, or 0 for an IANA IE
   * @param number the IE number (low-order 15 bits without enterprise bit)
   * @param size the IE length
   */
  void add_unknown(uint32_t pen, uint16_t number, uint16_t size);

  /**
   * Look up and return a pointer to the canonical Information Element given 
   * a private enterprise number, information element number, and size. 
   * This is intended for use when parsing templates received by an IPFIX
   * collecting process.
   *
   * @param pen the private enterprise number to lookup, or 0 for an IANA IE
   * @param number the IE number (low-order 15 bits without enterprise bit)
   * @param size the IE length
   * @return a pointer to a canonical IE or NULL if no canonical IE exists
   */
  const InfoElement* lookupIE(uint32_t pen, uint16_t number, uint16_t size) const;
  
  /**
   * Look up and return a pointer to the canonical Information Element given 
   * an example IE (as returned, for example, by parseIESpec())
   *
   * @param specie the example IE
   * @return a pointer to a canonical IE or NULL if no canonical IE exists
   */  
  const InfoElement* lookupIE(const InfoElement& specie) const;

  /**
   * Look up and return a pointer to the canonical Information Element given 
   * an IESpec.
   *
   * @param iespec the IESpec to look up
   * @return a pointer to a canonical IE or NULL if no canonical IE exists
   */  
  const InfoElement* lookupIE(const std::string& iespec) const;

  /**
   * Look up and return a pointer to the canonical Information Element given 
   * an IESpec.
   *
   * @param iespec the IESpec to look up
   * @return a pointer to a canonical IE or NULL if no canonical IE exists
   */  
  const InfoElement* lookupIE(const char* iespec) const;

  /**
   * Get a type for a name from this model's type lookup table. See RFC5610 for type names.
   *
   * @param name name of the IEType to look up
   * @return pointer to the IEType or NULL if no type for the given name
   */
  const IEType* lookupIEType(const std::string &name) const {
    std::map<std::string, const IEType*>::const_iterator iter;
    
    if ((iter = ietypes_byname_.find(name)) == ietypes_byname_.end()) {
      return NULL;
    } else { 
      return iter->second;
    }
  }

  /**
   * Get a type for a number from this model's type lookup table. See RFC5610 for type numbers.
   *
   * @param number number of the IEType to look up
   * @return pointer to the IEType or NULL if no type for the given number
   */
  const IEType* lookupIEType(const unsigned int number) const { 
    return ietypes_bynum_.at(number); 
  }

  /**
   * Debugging. Dump this InfoModel as a bunch of IESpect to a given output stream.
   *
   * @param os the output stream to dump the InfoModel to
   */
  void dump(std::ostream& os) const;
  
private:
  /**
   * Create a new InfoModel with a type lookup table, but no
   * canonical InfoElements. Use this constructor when initializing the
   * Information Model from a file using add()
   */
  InfoModel() {
    initTypes();
  }
  
  // FIXME make InfoModel uncopyable
  
  // Type initialization; internal
  void registerIEType(const IEType* iet);
  void initTypes();

  // Information element registry. Keep canonical IEs by autopointer.
  std::map<uint16_t, std::shared_ptr<InfoElement> >
                                iana_registry_;
  std::map<uint32_t, std::map<uint16_t, std::shared_ptr<InfoElement> > >
                                pen_registry_;
  std::vector<std::shared_ptr<InfoElement> >
                                rle_registry_;

  // Information element name lookup. 
  std::map<std::string, std::shared_ptr<InfoElement> >
                                name_registry_;
  
  // Information element type lookup.
  std::map<std::string, const IEType*>
                                ietypes_byname_;
  std::vector<const IEType*>
                                ietypes_bynum_;

};

} // namespace IPFIX

#endif // idem hack
