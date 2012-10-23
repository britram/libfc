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

#ifndef IPFIX_DATASETDECODER_H
#  define IPFIX_DATASETDECODER_H

#  include <map>
#  include <vector>

#  include "ContentHandler.h"
#  include "InfoElement.h"

namespace IPFIX {

  class DataSetDecoder : public ContentHandler {
  public:
    DataSetDecoder();
    virtual ~DataSetDecoder();

    void start_message(uint16_t version,
                       uint16_t length,
                       uint32_t export_time,
                       uint32_t sequence_number,
                       uint32_t observation_domain);
    void end_message();
    void start_template_set(uint16_t set_id, uint16_t set_length);
    void end_template_set();
    void start_template_record(uint16_t template_id, uint16_t field_count);
    void end_template_record();
    void start_option_template_set(uint16_t set_id, uint16_t set_length);
    void end_option_template_set();
    void start_option_template_record(uint16_t template_id,
                                      uint16_t field_count,
                                      uint16_t scope_field_count);
    void end_option_template_record();
    void field_specifier(bool enterprise,
                         uint16_t ie_id,
                         uint16_t ie_length,
                         uint32_t enterprise_number);
    void start_data_set(uint16_t id, uint16_t length, const uint8_t* buf);
    void end_data_set();

    void register_minimal_template(const std::vector<const std::pair<const InfoElement*, void*> >& minimal_template);

  private:
    class WireTemplate;
    class MinimalTemplate;

    uint32_t observation_domain;

    /** Makes unique template key from template ID. 
     *
     * @param tid template id
     *
     * @return unique template id
     */
    uint64_t DataSetDecoder::make_template_key(uint16_t tid);

    // FIXME: What's the right type for the value type? Pointer?
    // Reference? What about const-ness (once the template is
    // registered, it's read-only).
    /** Wire templates as they're read from template sets.
     *
     * This map is kept between messages.
     */
    std::list<const WireTemplate> wire_templates;

    /** Minimal templates.
     *
     * This list is kept between messages, and new templates are added
     * at the end.
     */
    std::list<MinimalTemplate> minimal_templates;

    // Data for reading template sets
    /** The current wire template that is being assembled. 
     *
     * This pointer is deleted after every template record.
     */
    std::vector<const InfoElement*>* current_wire_template;

    /** Id of current wire template */
    uint16_t current_template_id;

    /** Number of fields in current wire template. */
    uint16_t current_field_count;

    /** Wire template field currently being assembled.
     *
     * This number must be less than current_field_count.
     */
    uint16_t current_field_no;

    // Data for decoding data sets
    
  };

} // namespace IPFIX

#endif // IPFIX_DATASETDECODER_H
