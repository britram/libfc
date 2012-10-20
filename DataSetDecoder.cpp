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

#include <cassert>
#include <climits>

#include "BasicOctetArray.h"
#include "DataSetDecoder.h"


/** Decode plans describe how a data record is to be decoded.
 *
 * Decoding a data record means determining, for each data field, 
 *
 *   - if the data's endianness must be converted; and
 *   - where the converted data is to be stored.
 *
 * Basically, clients register sets of pairs of <ie, pointer> with the
 * DataSetDecoder class below.  This is called a Minimal Template.
 * This minimal template will then be used to match incoming data
 * records.  The previously used procedure was to nominate the first
 * minimal template whose set of information elements is a subset of
 * the information elements for the data set. in question. We
 * implement this here as well, but it might be changed easily.  (For
 * example, we might reasonably select that minimal template that is a
 * subset of the data set's template and at the same time matches the
 * most fields.)
 *
 * Now having a template for the data set (called a Wire Template) and
 * a matching minimal template, we create a Decoding Plan.  Basically,
 * a decoding plan is a sequence of decisions, one for each field.
 * There are two types of decisions:
 *
 *   - A SKIP decision causes the corresponding field to be skipped.
 *   - A TRANSFER decision causes the corresponding field to be
 *     endianness-converted if necessary (this can be gleaned form the
 *     corresponding information element's type), and copied to the
 *     pointer that the client gave at the time of registration.
 *
 * For convenience, there exist two variations of each decision,
 * depending on whether the corresponding field is fixed-length field
 * or a variable-length field.
 *
 * It would be nice if we could collapse two adjacent SKIP decisions
 * into one, but that can only be done if the two fields in question
 * are fixed-length fields.  We might do this in a future release if
 * it turns out to be a performance problem.
 */
class DecodePlan {
public:
  /** Creates a decoding plan from a minimal template and a wire
   * template.
   *
   * @param minimal_template a minimal template that must have been
   *   found to match the wire template (all IEs in the minimal
   *   template must also appear in the wire template)
   * @param wire_template the wire template for the data set
   */
  DecodePlan(std::vector<std::pair<const InfoElement*, void*> > minimal_template, std::vector<const InfoElement*> wire_template);
  ~DecodePlan();

  /** Executes the plan.
   *
   * Due to the construction of IPFIX (there exists variable-length
   * data and a data record does not have a header containing its
   * length), we do not know the exact length of that data record.
   * Hence we give the length of the remaining data set and expect
   * this member function to return the number of bytes that it has
   * decoded.
   *
   * @param buf the buffer containing the data record (and the
   *     remaining data set)
   * @param len length of the remaining data set
   *
   * @return number of bytes decoded
   */
  uint16_t execute(const uint8_t* buf, uint16_t len);

private:
  struct Decision {
    // FIXME Could encode endianness conversion into the decision
    // type, would save an if in the inner loop. Profile this.
    /** The decision type. */
    enum Type {
      skip_fixlen,              /** Skip a fixed amount. */
      skip_varlen,              /** Skip a variable amount. */
      transfer_fixlen,          /** Transfer a fixed amount */
      transfer_varlen,          /** Transfer a variable amount */
    } type;

    /** How much data is affected in the data set?  This field makes
     * sense only in fixlen decisions. */
    uint16_t length;

    /** Destination type size in bytes.  This field makes sense only in
     * transfer decisions. */
    uint16_t destination_size;

    /** Endianness conversion?  This field makes sense only in fixlen
     * decisions. */
    bool convert_endianness;

    /** Transfer target. This field makes sense only in transfer
     * decisions.  The caller must make sure that these pointers are
     * suitably aligned for the result data type (for fixlen
     * transfers), or that they point to a BasicOctetArray object (for
     * varlen transfers). */
    void* p;
  };
  
  std::vector<Decision> plan;
};

uint16_t DecodePlan::execute(const uint8_t* buf, uint16_t length) {
  const uint8_t* cur = buf;
  const uint8_t* buf_end = buf + length;

  for (auto i = plan.begin(); i != plan.end(); ++i) {
    assert(cur < buf_end);

    switch (i->type) {
    case Type::skip_fixlen:
      assert (cur + i->length <= buf_end);
      cur += i->length;
      break;

    case Type::skip_varlen:
      uint16_t varlen_length = decode_varlen_length(&cur);
      assert(cur + varlen_length <= buf_end);
      cur += varlen_length;
      break;

    case Type::transfer_fixlen:
      assert(cur + i->length <= buf_end);
      /* Assume all-zero bit pattern is zero, null, 0.0 etc. */
      // FIXME: Check if transferring native data types is faster
      // (e.g., short when i->length == 2, long when i->length == 4
      // etc).
      memset(i->p, '\0', i->destination_size);
      if (i->convert_endianness) {
        // Intention: left-justify value at cur in field at i->p
        for (uint16_t k = 0; k < i->length; k++)
          i->p[k] = cur[i->length - (k + 1)];
      } else
        // Intention: right-justify value at cur in field at i->p
        memcpy(i->p + i->destination_size - i->length, cur, i->length);

      cur += i->length;
      break;

    case Type::transfer_varlen:
      uint16_t varlen_length = decode_varlen_length(&cur);
      assert(cur + varlen_length <= buf_end);
      
      BasicOctetArray* p = reinterpret_cast<BasicOctetArray*>(i->p);
      p->copy_content(cur, varlen_length);

      cur += varlen_length;
      break;
    }

    assert ((cur - buf) <= USHRT_MAX);
    return static_cast<uint16_t>(cur - buf);
  }
}

namespace IPFIX {

  DataSetDecoder::DataSetDecoder()
    : current_wire_template(0) {
  }

  DataSetDecoder::~DataSetDecoder() {
    assert (current_wire_template == 0);
  }

  void DataSetDecoder::start_message(uint16_t version,
                                     uint16_t length,
                                     uint32_t export_time,
                                     uint32_t sequence_number,
                                     uint32_t observation_domain) {
    assert(current_wire_template == 0);
    assert(wire_templates.size() == 0);
  }

  void DataSetDecoder::end_message() {
    wire_templates.clear();
    assert(current_wire_template == 0);
  }

  void DataSetDecoder::start_template_set(uint16_t set_id,
                                          uint16_t set_length) {
  }

  void DataSetDecoder::end_template_set() {
  }

  void DataSetDecoder::start_template_record(
      uint16_t template_id,
      uint16_t field_count) {
    assert(current_wire_template == 0);
    current_template_id = template_id;

    if (wire_templates contains template_id as key) {
      // Duplicate template id. FIXME: Overwrite?
      report_error();
    }

    current_field_count = 0;
    current_field_no = 0;
    current_wire_template 
      = new std::vector<const InfoElement*>(current_field_count);
  }

  void DataSetDecoder::end_template_record() {
    if (current_wire_template->size() > 0)
      wire_templates.map(current_template_id, *current_wire_template);
    delete current_wire_template;
    current_wire_template = 0;
  }

  void DataSetDecoder::start_option_template_set(
      uint16_t set_id,
      uint16_t set_length) {
  }

  void DataSetDecoder::end_option_template_set() {
  }

  void DataSetDecoder::start_option_template_record(
      uint16_t template_id,
      uint16_t field_count,
      uint16_t scope_field_count) {
  }

  void DataSetDecoder::end_option_template_record() {
  }

  void DataSetDecoder::field_specifier(
      bool enterprise,
      uint16_t ie_id,
      uint16_t ie_length,
      uint32_t enterprise_number) {
    if (current_field_no >= current_field_count) {
      // Template contains more field specifiers than were given in
      // the header.
      report_error();
    }

    const InfoElement* ie
      = lookup_ie(ie_id, ie_length, enterprise, enterprise_number);
    assert (ie != 0);
    current_wire_template[current_field_count++] = ie;
  }

  void DataSetDecoder::start_data_set(uint16_t id,
                                      uint16_t length,
                                      const uint8_t* buf) {
    if (wire_templates does not contain "id" as key)
      // No wire template for this data set: skip (but no error)
      return;

    // Find out who is interested in data from this data set

    // Prepare transcoding plan

    const uint8_t* buf_end = buf + length;
    const uint8_t* cur = buf;

    while (cur < buf_end) {
    }

  }

  void DataSetDecoder::end_data_set() {
  }

} // namespace IPFIX
