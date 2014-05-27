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

#ifndef _LIBFC_PLACEMENTEXPORTER_H_
#  define _LIBFC_PLACEMENTEXPORTER_H_

#  include <cstdint>
#  include <list>
#  include <set>
#  include <vector>

#  include <sys/uio.h>

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
#    include <log4cplus/logger.h>
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#  include "Constants.h"
#  include "ExportDestination.h"
#  include "PlacementTemplate.h"

class EncodePlan;

namespace LIBFC {

  /** Interface for exporter with the placement interface.
   *
   * A simple example of how to use the placement interface for export
   * is this (with error checking omitted):
   *
   * @code
   * FileExportDestination d(some_file_descriptor);
   * PlacementExporter e(d, my_observation_domain);
   *
   * uint64_t flow_start_milliseconds = 0;
   * uint32_t source_ip_v4_address = 0;
   *
   * PlacementTemplate* my_flow_template = new PlacementTemplate();
   * 
   * my_flow_template->register_placement(
   *   InfoModel::instance().lookupIE("flowStartMilliseconds"),
   *   &flow_start_milliseconds, 0);
   * my_flow_template->register_placement(
   *   InfoModel::instance().lookupIE("sourceIPv4Address"),
   *   &source_ip_v4_address, 0);
   *
   * // Assign values to source_ip_v4_address and flow_start_milliseconds
   *
   * e.place_values(my_flow_template);
   * e.flush();
   *
   * delete my_flow_template
   *
   * close(some_file_descriptor);
   * @endcode
   *
   * See the documentation for ExportDestination,
   * FileExportDestination, and PlacementTemplate for more
   * information.
   */
  class PlacementExporter {
  public:
    /** Creates an exporter.
     *
     * @param os the output stream to use
     * @param observation_domain the observation domain; see RFC5101
     */
    PlacementExporter(ExportDestination& os, uint32_t observation_domain);

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
    void place_values(const PlacementTemplate* tmpl);

  private:

    ExportDestination& os;
    /* The expression of cont-ness for the PlacementTemplates pointed
     * to by the pointers below is very tricky.  From the point of
     * view of this object, they are const: we promise never to change
     * any data member or to call any mutator in the PlacementTemplate
     * objects. On the other hand, the caller also holds copies of
     * these pointers, and in fact the caller is *expected* to change
     * data members, so for the caller the pointer is not const.
     *
     * I don't know what kinds of things C++ compilers can
     * legitimately assume about const pointers.  If they can assume
     * that data values will not change in objects pointed to by const
     * pointers, then this could be very bad and the const would have
     * to be removed throughout.
     */

    /** The template currently in use.
     *
     * As long as this doesn't change, we don't need to open another
     * data set or another template set. */
    const PlacementTemplate* current_template;

    /** All templates used so far in this session or message.
     *
     * In this set, we capture all templates used so far in this
     * session.  When a data record comes along that belongs to a
     * hitherto unknown template, that template is inserted here, and
     * a new template is issued. */
    std::set<const PlacementTemplate*> used_templates;

    /** Templates that need to go into this message's template record. */
    std::set<const PlacementTemplate*> new_templates;

    /** Most recently assigned template id. */
    uint16_t current_template_id;

    /** Sequence number for messages; see RFC 5101. */
    uint32_t sequence_number;

    /** Observation domain for messages; see RFC 5101.
     *
     * For the moment, we support only one observation domain. This
     * may change in the future. */
    uint32_t observation_domain;

    /** Number of octets in this message so far. This includes message
     * and set headers, template sets and data sets. */
    size_t n_message_octets;

    /** Number of octets in template set, or 0 if no template set. */
    uint16_t template_set_size;

    /** The number of octets in the currently assembled set.
     *
     * The space between `<' and `::' is mandatory because of the
     * trigraph `<::', which stands for `['.  Who came up with this
     * crap? */
    std::vector< ::iovec> iovecs;

    EncodePlan* plan;

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger;
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */

    /** Finishes the current data set by putting the template ID and
     * set length into the set header. 
     *
     * This function is idempotent.
     */
    void finish_current_data_set();
  };

} // namespace LIBFC

#endif // _LIBFC_PLACEMENTEXPORTER_H_
