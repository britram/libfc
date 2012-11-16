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

#  include <cstdint>
#  include <mutex>

#  include "Constants.h"
#  include "ExportDestination.h"
#  include "PlacementTemplate.h"

namespace IPFIX {

  /** Interface for exporter with the placement interface. */
  class PlacementExporter {
  public:
    /** Creates an exporter.
     *
     * @param os the output stream to use
     */
    PlacementExporter(ExportDestination& os);

    /** Destroys an exporter.
     *
     * Will, as a side effect, flush and close the export
     * destination.
     */
    ~PlacementExporter();
    
    /** Finishes the current message and sends it.
     *
     * @return true if the operation was successful, false otherwise
     */
    bool flush();

    /** Place values in a PlacementTemplate into the message. 
     *
     * @param template placement template for current placement
     */
    virtual void place_values(const PlacementTemplate* tmpl) = 0;

    /** Registers a placement template.
     *
     * @param tmpl the placement template to register
     */
    void register_placement_template(const PlacementTemplate* tmpl);

  private:
    ExportDestination& os;
    std::mutex lock;
  };

} // namespace IPFIX

#endif // IPFIX_PLACEMENTCALLBACK_H
