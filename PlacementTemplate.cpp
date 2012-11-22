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

#ifdef _IPFIX_HAVE_LOG4CPLUS_
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_DEBUG(logger, expr)
#endif /* _IPFIX_HAVE_LOG4CPLUS_ */

#include "PlacementTemplate.h"

namespace IPFIX {

  class PlacementInfo {
  public:
    /** Address where to write/read values from/to. */
    void* address;
    
    /** Size of InfoElement on the wire. This is useful only when
     * exporting. */
    size_t size_on_wire;
  };

  PlacementTemplate::PlacementTemplate() 
#ifdef _IPFIX_HAVE_LOG4CPLUS_
    : logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("logger")))
#endif /* _IPFIX_HAVE_LOG4CPLUS_ */
  {
  }

  void PlacementTemplate::register_placement(const InfoElement* ie,
                                             void* p) {
    placements[ie] = p;
  }

  void* PlacementTemplate::lookup_placement(const InfoElement* ie) const {
    std::map<const InfoElement*, void*>::const_iterator it
      = placements.find(ie);
    return it == placements.end() ? 0 : it->second;
  }

  bool PlacementTemplate::is_match(const MatchTemplate* t) const {
    LOG4CPLUS_DEBUG(logger, "ENTER is_match");

    for (auto i = placements.begin(); i != placements.end(); ++i) {
      LOG4CPLUS_DEBUG(logger, "  looking up IE " << i->first->toIESpec());
      auto p = t->find(i->first);
      if (p == t->end()) {
        LOG4CPLUS_DEBUG(logger, "    not found -> false");
        return false;
      }
    }
    LOG4CPLUS_DEBUG(logger, "  all found -> true");
    return true;
  }

} // namespace IPFIX
