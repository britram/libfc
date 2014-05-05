/* Hi Emacs, please use -*- mode: C++; -*- */

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


/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * Defines the InfoModel class
 */
#ifndef _LIBFC_INFOMODEL_H_ // idem
#define _LIBFC_INFOMODEL_H_ // hack

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include "IEType.h"

// FIXME enforce thread safety on changes to the infomodel

namespace IPFIX {

  class InfoElement;

  /**
   * Represents an IPFIX Information Model, a collection of canonical 
   * Information Elements. 
   */
  class InfoModel {
  public:
    InfoModel(const InfoModel&) = delete;
    InfoModel& operator=(const InfoModel&) = delete;

    /** Returns the information model instance.
     *
     * InfoModel is a singleton; this class accessor ensures only one 
     * InfoModel exists at once. 
     *
     * At startup time, your application should initialize this instance
     * using one of the three initialization methods, defaultV9(),
     * defaultIPFIX(), or default5103().
     */
    static InfoModel& instance();
  
    /** Initialises the info model with NetFlow v9 information elements.
     *
     * Initializes this InfoModel for NetFlowV9 only. NetFlowV9 applications
     * should call this method once on the default instance.
     */
    void defaultV9();
    
    /** Initialises the info model with IPFIX information elements.
     *
     * Initializes this InfoModel for IPFIX. IPFIX applications not using 
     * bidirectional flows as per RFC5103 should call this method once on 
     * the default instance returned by instance() at startup.
     */
    void defaultIPFIX();

    /** Initialises the info model with IPFIX information elements,
     * including RFC 5103 support.
     *
     * Initializes this InfoModel for IPFIX with RFC5103 support. IPFIX 
     * applications using bidirectional flows as per RFC5103 should call 
     * this method once on the default instance returned by instance() at 
     * startup.
     */
    void default5103();
  
    /** Creates a new (non-canonical) Information Element.
     *
     * Parses a complete IESpec. The return is suitable for
     * canonicalization via lookupIE() or addition to the InfoModel
     * via add().
     *
     * @param iespec the IESpec to parse
     * @return a new Information Element
     */
    const InfoElement parseIESpec(const std::string& iespec) const;

    /** Adds a copy of an Information Element to the Information Model.
     *
     * Idempotent; will only add the Information Element once.
     *
     * @param specie the Information Element to add
     *
     * @return the added information element
     */
    const InfoElement* add(const InfoElement& specie);
  
    /** Adds an Information Element described by a complete IESpec to the
     * Information Model.
     *
     * @param iespec the Information Element to add as an IESpec
     */
    void add(const std::string& iespec);
  
    /** Adds an unknown Information Element to the Information Model. 
     *
     * Sometimes, templates arrive that contain unknown IEs. This
     * method stores the IE as an octetArray with a generated name.
     *
     * @param pen the private enterprise number to add, or 0 for an IANA IE
     * @param number the IE number (low-order 15 bits without enterprise bit)
     * @param size the IE length
     *
     * @return the information element that was added
     */
    const InfoElement* add_unknown(uint32_t pen, uint16_t number,
				   uint16_t size);

    /** Looks up and returns a pointer to the canonical Information
     * Element.
     *
     * given a private enterprise number, information element number,
     * and size, this method returns a canonical representation of the
     * information element thus referenced.  This is intended for use
     * when parsing templates received by an IPFIX collecting process.
     *
     * @param pen the private enterprise number to lookup, or 0 for an IANA IE
     * @param number the IE number (low-order 15 bits without enterprise bit)
     * @param size the IE length
     * @return a pointer to a canonical IE or NULL if no canonical IE exists
     */
    const InfoElement* lookupIE(uint32_t pen, uint16_t number,
				uint16_t size) const;
  
    /** Looks up and return a pointer to the canonical Information
     * Element.
     *
     * Given  an example IE (as returned, for example, by
     * parseIESpec()), this method returns a canonical representation of the
     * information element thus referenced.
     *
     * @param specie the example IE
     * @return a pointer to a canonical IE or NULL if no canonical IE exists
     */  
    const InfoElement* lookupIE(const InfoElement& specie) const;

    /** Looks up and return a pointer to the canonical Information
     * Element.
     *
     * Given an IE specification, returns a canonical representation of the
     * information element thus referenced.
     *
     * @param iespec the IESpec to look up
     * @return a pointer to a canonical IE or NULL if no canonical IE exists
     */  
    const InfoElement* lookupIE(const std::string& iespec) const;

    /** Gets a type for a name from this model's type lookup table.
     *
     * See RFC5610 for type names.
     *
     * @param name name of the IEType to look up
     * @return pointer to the IEType or NULL if no type for the given name
     */
    const IEType* lookupIEType(const std::string &name) const;
    
    /** Gets a type for a number from this model's type lookup table.
     *
     * See RFC5610 for type numbers.
     *
     * @param number number of the IEType to look up
     * @return pointer to the IEType or NULL if no type for the given number
     */
    const IEType* lookupIEType(const unsigned int number) const;

    /** Dumps this InfoModel as a bunch of IESpect to a given output stream.
     *
     * This method is used only for debugging.
     *
     * @param os the output stream to dump the InfoModel to
     */
    void dump(std::ostream& os) const;
  
  private:
    /** Creates a new InfoModel with a type lookup table.
     *
     * Initially, this info model will have no canonical
     * InfoElements. Use this constructor when initializing the
     * Information Model from a file using add()
     */
    InfoModel();
  
    // Type initialization; internal
    void registerIEType(const IEType* iet);
    void initTypes();

    // Information element registry. Keep canonical IEs by autopointer.
    std::map<uint16_t, std::shared_ptr<InfoElement> > iana_registry_;
    std::map<uint32_t, std::map<uint16_t,
				std::shared_ptr<InfoElement> > >  pen_registry_;
    std::vector<std::shared_ptr<InfoElement> > rle_registry_;
    
    // Information element name lookup. 
    std::map<std::string, std::shared_ptr<InfoElement> >  name_registry_;
    
    // Information element type lookup.
    std::map<std::string, const IEType*> ietypes_byname_;
    std::vector<const IEType*>  ietypes_bynum_;

    /** Makes sure that accesses to instance variables happen
     * under mutual exclusion.
     *
     * Needs to be mutable so that logically const methods like
     * lookupIE() can still acquire the lock.
     */
    mutable std::recursive_mutex lock;
  };

} // namespace IPFIX

#endif // idem hack
