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

#ifndef IPFIX_PLACEMENTCALLBACK_H
#  define IPFIX_PLACEMENTCALLBACK_H

#  include "DataSetDecoder.h"
#  include "IPFIXReader.h"
#  include "PlacementTemplate.h"

namespace IPFIX {

  /** Interface for collector with the placement interface. */
  class PlacementCollector {
  public:
    /** Creates a callback. */
    PlacementCollector();

    /** Collects information elements from an input stream. 
     *
     * @param is the input stream to parse
     */
    void collect(InputSource& is);

    /** Signals that placement of values will now begin. 
     *
     * @param template placement template for current placements
     */
    virtual void start_placement(const PlacementTemplate* tmpl) = 0;

    /** Signals that placement of values has ended. 
     *
     * @param template placement template for current placements
     */
    virtual void end_placement(const PlacementTemplate* tmpl) = 0;

  protected:
    /** Registers a placement template.
     *
     * @param placement_template the placement template to register
     */
    void register_placement_template(const PlacementTemplate*);

  private:
    DataSetDecoder dsd;
    IPFIXReader ir;
  };

} // namespace IPFIX

#endif // IPFIX_PLACEMENTCALLBACK_H