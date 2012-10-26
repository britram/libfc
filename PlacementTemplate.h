/* Hi Emacs, please use -*- mode: C++; -*- */
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
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */

#ifndef IPFIX_PLACEMENTTEMPLATE_H
#  define IPFIX_PLACEMENTTEMPLATE_H

#  include <map>

#  include <log4cplus/logger.h>

#  include "InfoElement.h"
#  include "MatchTemplate.h"

namespace IPFIX {

  /** Association between IEs and memory locations.
   *
   * An IPFIX collection process will ultimately need to decide where
   * an information element as it appears in an IPFIX data record will
   * have to be stored in memory, just as an IPFIX exporter will have
   * to decide where an information element that is to be exported
   * will take its data from. This class provides the association
   * between a set of information elements and corresponding memory
   * locations.
   */
  class PlacementTemplate {
  public:
    PlacementTemplate();

    /** Registers an association between an IE and a memory location.
     *
     * @param ie the information element
     * @param p the memory location to be associated with the IE
     */
    void register_placement(const InfoElement* ie, void* p);

    /** Retrieves the memory location given an IE.
     *
     * @param ie the information element to look for
     *
     * @return the memory location associated with this IE, or NULL if
     *     the IE hasn't been registered previously.
     */
    void* lookup_placement(const InfoElement* ie) const;

    /** Tells whether a given template matches this template.
     *
     * A template T matches this template iff T's set of IEs is a
     * subset of this template's set of IEs.
     *
     * @return true if the templates match, false otherwise
     */
    bool is_match(const MatchTemplate* t) const;

  private:
    std::map<const InfoElement*, void*> placements;
    log4cplus::Logger logger;
  };

} // namespace IPFIX
#endif // IPFIX_PLACEMENTTEMPLATE_H
