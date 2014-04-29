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

#ifndef IPFIX_IPFIXCONTENTHANDLER_H
#  define IPFIX_IPFIXCONTENTHANDLER_H

#  include <list>
#  include <map>
#  include <vector>

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
#    include <log4cplus/logger.h>
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#  include "ContentHandler.h"
#  include "ErrorHandler.h"
#  include "InfoElement.h"
#  include "InfoModel.h"
#  include "InputSource.h"
#  include "MatchTemplate.h"
#  include "PlacementTemplate.h"

namespace IPFIX {

  class PlacementCollector;

  /** This class decodes data sets, and is the main go-to point when
   * using the "Placement Interface"-type of IPFIX collection.  For a
   * more in-depth treatment, see the documentation on
   * PlacementTemplate.
   */
  class IPFIXContentHandler : public ContentHandler, public ErrorHandler {
  public:
    IPFIXContentHandler();
    ~IPFIXContentHandler();

    /* From ContentHandler */
    void start_session();
    void end_session();

    void start_message(uint16_t version,
                       uint16_t length,
                       uint32_t export_time,
                       uint32_t sequence_number,
                       uint32_t observation_domain,
		       uint64_t base_time);
    void end_message();
    void start_template_set(uint16_t set_id,
			    uint16_t set_length,
			    const uint8_t* buf);
    void end_template_set();
    void start_options_template_set(uint16_t set_id,
				   uint16_t set_length,
				   const uint8_t* buf);
    void end_options_template_set();
    void start_data_set(uint16_t id, uint16_t length, const uint8_t* buf);
    void end_data_set();

    /* From ErrorHandler */
    void error(Error error, const char* message);
    void fatal(Error error, const char* message);
    void warning(Error error, const char* message);

    /** Registers a placement template.
     *
     * Calling this function with a given placement template and a
     * given callback means that if there is a data record whose
     * template fits the placement template, you want the callback's
     * callbacks to be called.
     *
     * @param placement_template the placement template
     * @param callback which functions to call on a matching record.
     */
    void register_placement_template(
        const PlacementTemplate* placement_template,
        PlacementCollector* callback);

  private:
    /** Observation domain for this message. */
    uint32_t observation_domain;

    /** The cached InfoModel instance. */
    InfoModel& info_model;

    /** Looks up a template in the set of wire templates.
     *
     * This function returns the wire template belonging to this
     * template ID (and observation domain), or NULL if it is not
     * present. The reason to do it in this rather roundabout way is
     * because std::map does not have a contains_key() function.  Like
     * Java.
     *
     * @param id the template ID to look up
     *
     * @return a pointer to the template belonging to this template
     *     ID, or NULL if the template isn't present.
     */
    const MatchTemplate* find_wire_template(uint16_t id) const;

    /** Given a wire template, finds a matching placement template.
     *
     * A given wire template may match more than one stored placement
     * template.  For example, if there is one placement template for
     * IEs A and B, one for IEs A, B, and C, and a third for IEs A, B,
     * and D, then, when a data set comes along that contains IEs A,
     * B, C, and D, all three placement templates are potential
     * candidates.
     *
     * We resolve this problem as follows. We store the placement
     * templates @em{in sequence} and take the first one that
     * matches. That means if you have the three templates in the
     * above example, putting the one with A and B before the others
     * will pre-empt both of the two other ones.
     *
     * This means that if you are in a situation where you have
     * placement templates that might conceivably match several wire
     * templates, you should:
     *
     * - add the templates from most specific to least specific;
     * - add more common ones before less common ones; and
     * - resolve remaining conflicts through addition order, where the
     *   earlier template wins.
     *
     * @param wire_template the wire template to match
     *
     * @return the matching placement template, or NULL if no
     *     placement template matches
     */
    const PlacementTemplate*
    match_placement_template(const MatchTemplate* wire_template) const;

    /** Makes unique template key from template ID and observation domain. 
     *
     * @param tid template id
     *
     * @return unique template id
     */
    uint64_t make_template_key(uint16_t tid) const;

    /** Computes the minimal length of a template.
     *
     * IPFIX messages may have padding in their data sets, but that
     * padding must not be larger than the smallest data set that
     * could conceivably been sent. Therefore, this method computes
     * the smallest data set that could be sent using a certain
     * template. This must take into account reduced-length encodings
     * of data fields. (This complicated computation could have been
     * avoided if the data set header also carried the number of
     * records in the data set.)
     *
     * @param t the template
     *
     * @return the minimum length of the template on the wire
     */
    uint16_t wire_template_min_length(const MatchTemplate* t);

    void start_template_record(uint16_t template_id, uint16_t field_count);
    void end_template_record();
    void start_options_template_record(uint16_t template_id,
                                      uint16_t field_count,
                                      uint16_t scope_field_count);
    void end_options_template_record();
    void field_specifier(bool enterprise,
                         uint16_t ie_id,
                         uint16_t ie_length,
                         uint32_t enterprise_number);
    void scope_field_specifier(bool enterprise,
			       uint16_t ie_id,
			       uint16_t ie_length,
			       uint32_t enterprise_number);
    void options_field_specifier(bool enterprise,
				 uint16_t ie_id,
				 uint16_t ie_length,
				 uint32_t enterprise_number);

    /** Wire templates as they're read from template sets.
     *
     * This map is kept between messages.
     */
    std::map<uint64_t, const MatchTemplate*> wire_templates;

    /** Placement templates.
     *
     * This list is kept between messages, and new templates are added
     * at the end.
     */
    std::list<const PlacementTemplate*> placement_templates;

    /** Association between placement template and callback. */
    std::map<const PlacementTemplate*, PlacementCollector*> callbacks;

    /** Says whether to use the matched template cache. 
     *
     * At the moment, this is statically set in the constructor (to
     * false), but it could be made into a constructor parameter.
     */
    bool use_matched_template_cache;

    /** Association between wire template and placement template.
     *
     * We are using the strategy that we choose the @em{first}
     * placement template that matches as @em{the} matching
     * template. This means that a match, once found, cannot later be
     * replaced by a better match.  Therefore, once we've found a
     * matching template, we record it in this cache and look it up,
     * potentially saving long matching operations.
     *
     * Profiling shows that for simple cases, this caching slows the
     * collection process down, therefore this member might not be
     * used.
     */
    mutable std::map<const MatchTemplate*, const PlacementTemplate*>
      matched_templates;

    /** The current wire template that is being assembled. 
     *
     * This pointer is deleted after every template record.
     */
    MatchTemplate* current_wire_template;

    /** Id of current wire template */
    uint16_t current_template_id;

    /** Number of fields in current wire template. */
    uint16_t current_field_count;

    /** Wire template field currently being assembled.
     *
     * This number must be less than current_field_count.
     */
    uint16_t current_field_no;

    /** Tells whether the parse is good so far.
     *
     * This will be set to false when an unrecoverable parse error has
     * been detected. Then the destructor can decide *not* to check
     * assertions (because those assume that the parse has gone
     * well).
     */
    bool parse_is_good;

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger;
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */
 };

} // namespace IPFIX

#endif // IPFIX_IPFIXCONTENTHANDLER_H
