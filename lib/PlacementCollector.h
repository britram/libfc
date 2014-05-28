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
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */

#ifndef _libfc_PLACEMENTCALLBACK_H_
#  define _libfc_PLACEMENTCALLBACK_H_

#  include "PlacementContentHandler.h"
#  include "MessageStreamParser.h"
#  include "PlacementTemplate.h"

namespace libfc {

  /** Interface for collector with the placement interface. */
  class PlacementCollector {
  public:
    /** The protocol which we want to collect for. */
    enum Protocol {
      /** Expect an IPFIX message stream. */
      ipfix,
      /** Expect a Netflow V9 message stream. */
      netflowv9,
      /** Expect a Netflow V5 message stream. */
      netflowv5,
    };

    /** Creates a callback. */
    PlacementCollector(Protocol protocol);

    virtual ~PlacementCollector() = 0;

    /** Collects information elements from an input stream. 
     *
     * @param is the input stream to parse
     *
     * @return an error context, giving information about potential errors.
     */
    std::shared_ptr<ErrorContext> collect(InputSource& is);

    /** Signals that placement of values will now begin. 
     *
     * @param template placement template for current placements
     */
    virtual std::shared_ptr<ErrorContext>
      start_placement(const PlacementTemplate* tmpl) = 0;

    /** Signals that placement of values has ended. 
     *
     * @param template placement template for current placements
     * @return If <= 0, stop processing.
     */
    virtual std::shared_ptr<ErrorContext>
      end_placement(const PlacementTemplate* tmpl) = 0;

    /** Will be called on unhandled data sets.
     *
     * For the purposes of this discussion, an unhandled data set is a
     * data set for which there exists a template, but for which no
     * placement exists.
     *
     * The default implementation simply returns with no error
     * indication.  Caution: when overriding this method, you might be
     * tempted to return an error just because there is no matching
     * placement template for this data set.  But be aware that @em{this
     * will cause parsing to stop}.  If you want the caller to try
     * again, return Error::again.
     *
     * @param id set id (= template ID) as per RFC 5101, > 255
     * @param length length in bytes of the data records in this set
     *     (excluding the header)
     * @param buf pointer to the beginning of the data records
     *     (excluding the header)
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext>
      unhandled_data_set(uint32_t observation_domain, uint16_t id,
                         uint16_t length, const uint8_t* buf);

    /** Will be called on unknown data sets.
     *
     * For the purposes of this discussion, an unknown data set is a
     * data set for which no template exists.
     *
     * The default implementation simply returns with no error
     * indication.  Caution: when overriding this method, you might be
     * tempted to return an error just because there is no matching
     * placement template for this data set.  But be aware that @em{this
     * will cause parsing to stop}.  If you want the caller to try
     * again, return Error::again.
     *
     * @param id set id (= template ID) as per RFC 5101, > 255
     * @param length length in bytes of the data records in this set
     *     (excluding the header)
     * @param buf pointer to the beginning of the data records
     *     (excluding the header)
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext>
      unknown_data_set(uint32_t observation_domain, uint16_t id,
                       uint16_t length, const uint8_t* buf);

  protected:
    /** Registers a placement template.
     *
     * @param placement_template the placement template to register
     */
    void register_placement_template(const PlacementTemplate*);

    /** Registers this object as the one to call on unhandled/unknown
     * data sets.
     */
    void give_me_unhandled_data_sets();

  private:
    PlacementContentHandler d;
    MessageStreamParser* ir;
  };

} // namespace libfc

#endif // _libfc_PLACEMENTCALLBACK_H_
