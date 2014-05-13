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
 * @section DESCRIPTION
 * 
 * Defines the abstract template interface.
 *
 * A template is an ordered list of Information Elements describing a
 * record format. Client code will use the subclass WireTemplate,
 * which represents a template for encoding/decoding IPFIX messages on
 * the wire.
 */

#ifndef _LIBFC_IETEMPLATE_H_ // idem
#  define _LIBFC_IETEMPLATE_H_ // hack

#  include <cassert>
#  include <map>
#  include <vector>

#  include "InfoElement.h"

namespace IPFIX {

  typedef std::vector<const InfoElement *>::const_iterator IETemplateIter;
  typedef std::map<const InfoElement *, size_t> IEIndexMap;

  class IETemplate {
  public:
  
    /** Determines whether this template contains an instance of the
     * given IE.
     *
     * @param ie information element to search for; must be a canonical
     *           IE retrieved from InfoModel::instance().
     * @return true if this template contains the given IE in any size
     */
    bool contains(const InfoElement* ie) const;
  
    /** Determines whether this template contains all the IEs in a
     * given template.
     *
     * @param rhs template to compare to
     * @return true if this template contains the given IE in any size
     */
    bool containsAll(const IETemplate* rhs) const;
  
    /** Gets the offset of a given IE in this template. 
     *
     * Throws std::out_of_range if the template does not contain the IE.
     * 
     * @param ie information element to search for; must be a canonical
     *           IE retrieved from InfoModel::instance().
     * @return offset to IE in this template
     */
    size_t offset(const InfoElement* ie) const;
    
    /** Gets the length of a given IE in this template. 
     *
     * Throws std::out_of_range if the template does not contain the IE.
     * 
     * @param ie information element to search for; must be a canonical
     *           IE retrieved from InfoModel::instance().
     * @return length of IE in this template
     */
    size_t length(const InfoElement* ie) const;
    
    /** Get the IE matching a given IE in the template.
     *
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
    const InfoElement* ieFor(const InfoElement* ie) const;
    
    /** Gets the minimum length of a record represented by this template.
     * 
     * @return minimum record length
     */
    size_t minlen() const;
    
    /** Retrieves the starting iterator for the IEs in this template.
     * 
     * @return begin iterator
     */
    IETemplateIter begin() const;
    
    /** Retrieves the ending iterating on the IEs in this template.
     * 
     * @return end iterator
     */
    IETemplateIter end() const;
    
    /** Finds a certain IE.
     *
     * @param ie IE to find
     *
     * @return iterator to found element if found, end() if not
     */
    IETemplateIter find(const InfoElement* ie) const;
    
    /** Returns number of IEs in this template.
     *
     * @return number of IEs in template
     */
    size_t size() const;
    
    virtual void dumpIdent(std::ostream &os) const = 0;
    
    void dump(std::ostream& os) const;
    
  protected:
    
    /** Creates an empty template. */
    IETemplate();
    
    /** Creates an empty template with preallocated space.
     *
     * @param n_ies the number of IEs we expect to see in this template
     */
    IETemplate(size_t n_ies);
    
    /** Adds an information element to internal maps; used by subclasses.
     *
     * @param ie IE to add
     */
    void add_inner(const InfoElement* ie);
    
    // pointer to session containing template vector of information elements
    std::vector <const InfoElement *> ies_;

    // vector of information element offsets
    IEIndexMap index_map_;

    // minimum length of record represented by template
    size_t minlen_;
  };

} // namespace IPFIX

#endif
