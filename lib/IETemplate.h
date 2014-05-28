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

#ifndef _libfc_IETEMPLATE_H_ // idem
#  define _libfc_IETEMPLATE_H_ // hack

#  include <cassert>
#  include <map>
#  include <vector>

#  if defined(_libfc_HAVE_LOG4CPLUS_)
#    include <log4cplus/logger.h>
#  endif /* defined(_libfc_HAVE_LOG4CPLUS_) */

#  include "InfoElement.h"

namespace libfc {

  class IETemplate {
  public:
    
    /** Creates an empty template. */
    IETemplate();
        
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
  
    /** Gets the minimum length of a record represented by this template.
     * 
     * @return minimum record length
     */
    size_t minlen() const;
    
    /** Retrieves the starting iterator for the IEs in this template.
     * 
     * @return begin iterator
     */
    std::vector<const InfoElement *>::const_iterator begin() const;
    
    /** Retrieves the ending iterating on the IEs in this template.
     * 
     * @return end iterator
     */
    std::vector<const InfoElement *>::const_iterator end() const;
    
    /** Finds a certain IE.
     *
     * @param ie IE to find
     *
     * @return iterator to found element if found, end() if not
     */
    std::vector<const InfoElement *>::const_iterator
      find(const InfoElement* ie) const;
    
    /** Returns number of IEs in this template.
     *
     * @return number of IEs in template
     */
    size_t size() const;

    /** Adds the Information Element to the template.
     *
     * @param ie the IE to add
     */
    void add(const InfoElement* ie);

    /** Compares two IE templates for equality.
     *
     * Two templates are equal iff they contain the same IEs in the
     * same sequence.
     * 
     * @param rhs the right-hand side of the comparison
     *
     * @return true if the two templates are equal, false otherwise
     */
    bool operator==(const IETemplate& rhs) const;

    /** Compares two IE templates for equality.
     *
     * Two templates are equal iff they contain the same IEs in the
     * same sequence.  This method is the exact opposite of
     * operator==.
     * 
     * @param rhs the right-hand side of the comparison
     *
     * @return false if the two templates are equal, true otherwise
     */
    bool operator!=(const IETemplate& rhs) const;

  private:
    /** Adds an information element to internal maps.
     *
     * @param ie IE to add
     */
    void add_inner(const InfoElement* ie);
    
    // pointer to session containing template vector of information elements
    std::vector <const InfoElement *> ies_;

    // minimum length of record represented by template
    size_t minlen_;

#  if defined(_libfc_HAVE_LOG4CPLUS_)
    log4cplus::Logger logger;
#  endif /* defined(_libfc_HAVE_LOG4CPLUS_) */

  };

} // namespace libfc

#endif
