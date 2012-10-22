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
 *   - if the data's endianness must be converted;
 *   - if the data needs to be transformed in any other way (for
 *     example, boolean values are encoded with 1 meaning true and 2
 *     meaning false(!!), or reduced-length encoding of floating-point
 *     values means that doubles are really transferred as floats);
 *   - for variable-length data, what the length of the encoded value
 *     is; and
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
  DecodePlan(std::vector<std::pair<const IPFIX::InfoElement*, void*> > minimal_template, std::vector<const IPFIX::InfoElement*> wire_template);
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
   * @param length length of the remaining data set
   *
   * @return number of bytes decoded
   */
  uint16_t execute(const uint8_t* buf, uint16_t length);

private:
  struct Decision {
    /** The decision type. */
    enum decision_type_t {
      /** Skip a fixed amount. */
      skip_fixlen,

      /** Skip a variable amount. */
      skip_varlen,

      /** Transfer a fixed amount, with no endianness conversion, no
       * booleans, and no octet string. */
      transfer_fixlen,

      /** Transfer a boolean. Someone found it amusing in RFC 2579 to
       * specify boolean values true and false as 1 and 2,
       * respectively [sic!].  And someone else found it amusing to
       * standardise this behaviour in RFC 5101 too.  This is of
       * course wrong, since it disallows entirely sensible operations
       * like + for "or", * for "and" and "<=" for implication (which
       * is what you get when you make false less than true). */
      transfer_boolean,

      /** Transfer a fixed amount, with endianness conversion. */
      transfer_fixlen_endianness,

      /** Transfer an octet string with fixed length. */
      transfer_fixlen_octets,

      /** Reduced-length float64, no endianness conversion. */
      transfer_float_into_double,

      /** Reduced-length float64, with endianness conversion. */
      transfer_float_into_double_endianness,

      /** Transfer a variable amount. */
      transfer_varlen,
    } type;

    /** How much data is affected in the data set?  This field makes
     * sense only in fixlen decisions. */
    uint16_t length;

    /** Destination type size in bytes.  This field makes sense only in
     * transfer_fixlen decisions. */
    uint16_t destination_size;

    /** Transfer target. This field makes sense only in transfer
     * decisions.  The caller must make sure that these pointers are
     * suitably aligned for the result data type (for fixlen
     * transfers), or that they point to a BasicOctetArray object (for
     * varlen transfers). */
    uint8_t* p;
  };
  
  std::vector<Decision> plan;
};

/** Checks whether an IE from a minimal template is a match for an IE
 * from a wire template.
 *
 * Two information elements match if they:
 *
 *   - either the same private enterprise number or ar both in the
 *     IANA registry; and
 *   - have the same IE number.
 *
 * @param minimal IE from a wire template
 * @param wire IE from a wire template
 *
 * @return true if they match, false otherwise
 */
static bool
match_ie(const InfoElement* minimal, const InfoElement* wire) {
  return minimal->pen() == wire->pen()
    && minimal->number() == wire->number();
}

/** Dummy return value used to shut the compiler up.
 *
 * The function match() must return something, even when a matching
 * (ie, pointer) pair has not been found in the minimal template (see
 * the documentation there). We pre-empt the return statement with an
 * appropriate assert() so that the return statement is never
 * executed. However, that presumes that the compiler is smart enough
 * to figure that out, and not all compilers might be.  So we simply
 * write a return statement where we return this dummy pair. */
static std::pair<const IPFIX::InfoElement*, void*> __x(0,0);

/** Finds the matching IE from a minimal template.
 *
 * It is a fatal error if there is no such matching IE.
 *
 * @param minimal_template a minimal template
 * @param wt an IE from a wire template
 *
 * @return the (ie, pointer) pair from the minimal template whose
 *     information element matches the IE from the wire template.
 */
static const std::pair<const IPFIX::InfoElement*, void*>&
match(const std::vector<std::pair<const IPFIX::InfoElement*, void*>>& minimal_template
      const IPFIX::InfoElement* wt) {
  for (auto i = minimal_template.begin();
       i != minimal_template.end();
       i++) {
    if (match_ie(i->first, wt))
      return *i;
  }

  assert(0 == "Internal error: IE not found in supposedly "
              "matching template");
  /* NOTREACHED */
  return __x;                   // Shut compiler up
}

DecisionPlan::DecisionPlan(std::vector<std::pair<const IPFIX::InfoElement*, void*> > minimal_template, std::vector<const IPFIX::InfoElement*> wire_template) 
  : plan(wire_template.size()) {

#if defined(BOOST_BIG_ENDIAN)
  Decision::decision_type_t transfer_fixlen_maybe_endianness
    = Decision::transfer_fixlen;
  Decision::decision_type_t transfer_float_into_double_maybe_endianness
    = Decision::transfer_float_into_double;
#elif defined(BOOST_LITTLE_ENDIAN)
  Decision::decision_type_t transfer_fixlen_maybe_endianness
    = Decision::transfer_fixlen_endiananness;
  Decision::decision_type_t transfer_float_into_double_maybe_endianness
    = Decision::transfer_float_into_double_endianness;
#else
#  error libfc does not compile on weird-endian machines.
#endif

  for (auto wt = wire_template.begin(); wt != wire_template.end(); wt++) {
    if (minimal_template contains *i) { /* Encode transfer decision */
      Decision d;
      const std::pair<const IPFIX::InfoElement*, void*>& min_match
        = match(wire_template, *wt);

      d.p = min_match.second;
      if (wt->ietype() == 0)
        report_error("IE has NULL ietype");

      switch (wt->ietype()->number()) {
      case kOctetArray: 
        if (wi->len() == IPFIX::kVarlen) {
          d.type = Decision::transfer_varlen;
        } else {
          d.type = Decision::transfer_fixlen_octets;
          d.length = wt->len();
        }
        break;

      case IPFIX::IEType::kUnsigned8:
        d.type = Decision::transfer_fixlen;
        d.length = wt->len();
        d.destination_size = sizeof(uint8_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kUnsigned16:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(uint16_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kUnsigned32:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(uint32_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kUnsigned64:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(uint64_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kSigned8:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(int8_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kSigned16:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(int16_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kSigned32:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(int32_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kSigned64:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(int64_t);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kFloat32:
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(float);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kFloat64:
        assert(wt->length == sizeof(float)
               || wt->length() == sizeof(double));
        d.length = wt->len();
        if (d.length == sizeof(float))
          d.type = transfer_float_into_double_maybe_endianness;
        else
          d.type = transfer_fixlen_maybe_endianness;
        d.destination_size = sizeof(double);
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kBoolean:
        d.type = Decision::transfer_bool;
        d.length = wt->len();
        d.destination_size = sizeof(uint8_t); 
        if (wt->len() > i->destination_size)
          report_error("IE length greater than native size");
        break;

      case IPFIX::IEType::kMacAddress:
        /* RFC 5101 says to treat MAC addresses as 6-byte integers,
         * but Brian Trammell says that this is wrong and that the
         * RFC will be changed.  If for some reason this does not
         * come about, replace "transfer_fixlen" with
         * "transfer_fixlen_maybe_endianness". */
        d.type = transfer_fixlen;
        d.length = wt->len();
        d.destination_size = 6*sizeof(uint8_t);
        if (wt->len() != 6)
          report_error("MAC IE not 6 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
        break;
        
      case IPFIX::IEType::kString:
        if (wi->len() == IPFIX::kVarlen) {
          d.type = Decision::transfer_varlen;
        } else {
          d.type = Decision::transfer_fixlen_octets;
          d.length = wt->len();
        }
        break;

      case IPFIX::IEType::kDateTimeSeconds: break;
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(uint32_t);
        break;
        
      case IPFIX::IEType::kDateTimeMilliseconds: break;
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(uint64_t);
        break;
        
      case IPFIX::IEType::kDateTimeMicroseconds: break;
        d.type = transfer_fixlen_maybe_endianness;
        // RFC 5101, Chapter 6, Verse 2
        assert(wt->len() == sizeof(uint64_t));
        d.length = wt->len();
        d.destination_size = sizeof(uint64_t);
        break;
        
      case IPFIX::IEType::kDateTimeNanoseconds: break;
        d.type = transfer_fixlen_maybe_endianness;
        // RFC 5101, Chapter 6, Verse 2
        assert(wt->len() == sizeof(uint64_t));
        d.length = wt->len();
        d.destination_size = sizeof(uint64_t);
        break;
        
      case IPFIX::IEType::kIpv4Address: break;
        /* RFC 5101 says to treat all addresses as integers. This
         * would mean endianness conversion for all of these address
         * types, including MAC addresses and IPv6 addresses. But the
         * only reasonable address type with endianness conversion is
         * the IPv4 address.  If for some reason this is not correct
         * replace "transfer_fixlen_maybe_endianness" with
         * "transfer_fixlen". */
        d.type = transfer_fixlen_maybe_endianness;
        d.length = wt->len();
        d.destination_size = sizeof(uint32_t);
        if (wt->len() != 4)
          report_error("IPv4 Address IE not 4 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
        break;
        
      case IPFIX::IEType::kIpv6Address: break;
        /* RFC 5101 says to treat IPv6 addresses as 16-byte integers,
         * but Brian Trammell says that this is wrong and that the
         * RFC will be changed.  If for some reason this does not
         * come about, replace "transfer_fixlen" with
         * "transfer_fixlen_maybe_endianness". */
        d.type = transfer_fixlen;
        d.length = wt->len();
        d.destination_size = 16*sizeof(uint8_t);
        if (wt->len() != 16)
          report_error("IPv6 Address IE not 16 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
        break;
        
      default: 
        report_error("Unknown IE type");
        break;
      }
    } else {                    /* Encode skip decision */
      if (wi->len() == IPFIX::kVarlen) {
        d.type = Decision::skip_varlen;
      } else {
        d.type = Decision::skip_fixlen;
        d.length = wt->len();
      }
      break;
    }
  }
}

static uint16_t decode_varlen_length(const uint8_t** cur,
                                     const uint8_t* buf_end) {
  uint16_t ret = 0;

  if (*cur >= buf_end) 
    report_error("first octet of varlen length encoding beyond buffer");
  ret = *cur;

  if (ret < UCHAR_MAX)
    (*cur)++;
  else {
    if (*cur + 3 > buf_end) 
      report_error("three-byte varlen length encoding beyond buffer");
    (*cur)++;
    /* Assume that the two length-carrying octets are in network byte
     * order */
    ret = (*cur << 8) + (*cur + 1);
    *cur += 2;

    /* If it turns out that the three-byte encoding must not be used
     * for values < 255, then uncomment the following two lines: */
    //if (ret < UCHAR_MAX)
    //  report_error("three-byte varlen encoding used for value < 255");
  }

  if (*cur + ret > buf_end)
    report_error("varlen IE goes beyond buffer");

  return ret;
}

uint16_t DecisionPlan::execute(const uint8_t* buf, uint16_t length) {
  const uint8_t* cur = buf;
  const uint8_t* buf_end = buf + length;

  for (auto i = plan.begin(); i != plan.end(); ++i) {
    assert(cur < buf_end);

    switch (i->type) {
    case Decision::skip_fixlen:
      assert (cur + i->length <= buf_end);
      cur += i->length;
      break;

    case Decision::skip_varlen:
      {
        uint16_t varlen_length = decode_varlen_length(&cur, buf_end);
        assert(cur + varlen_length <= buf_end);
        cur += varlen_length;
      }
      break;

    case Decision::transfer_boolean:
      // Undo RFC 2579 madness
      assert(cur + 1 <= buf_end);
      if (*cur == 1)
        *(i->p) = 1;
      else if (*cur == 2)
        *(i->p) = 0;
      else
        report_error("bool encoding wrong");
      cur++;
      break;

    case Decision::transfer_fixlen:
      if (cur + i->length > buf_end)
        report_error("IE length beyond buffer");

      if (i->length > i->destination_size)
        report_error("IE length greater than native size");

      /* Assume all-zero bit pattern is zero, null, 0.0 etc. */
      // FIXME: Check if transferring native data types is faster
      // (e.g., short when i->length == 2, long when i->length == 4
      // etc).
      memset(i->p, '\0', i->destination_size);
      // Intention: right-justify value at cur in field at i->p
      memcpy(i->p + i->destination_size - i->length, cur, i->length);
      cur += i->length;
      break;

    case Decision::transfer_fixlen_endianness:
      if (cur + i->length > buf_end)
        report_error("IE length beyond buffer");

      if (i->length > i->destination_size)
        report_error("IE length greater than native size");

      /* Assume all-zero bit pattern is zero, null, 0.0 etc. */
      // FIXME: Check if transferring native data types is faster
      // (e.g., short when i->length == 2, long when i->length == 4
      // etc).
      memset(i->p, '\0', i->destination_size);
      // Intention: left-justify value at cur in field at i->p
      for (uint16_t k = 0; k < i->length; k++)
        i->p[k] = cur[i->length - (k + 1)];
      cur += i->length;
      break;

    case transfer_fixlen_octets:
      assert(cur + i->length <= buf_end);
      
      IPFIX::BasicOctetArray* p
        = reinterpret_cast<IPFIX::BasicOctetArray*>(i->p);
      p->copy_content(cur, i->length);

      cur += i->length;
      break;

    case transfer_float_into_double:
      assert(cur + sizeof(float) <= buf_end);
      {
        float f;
        memcpy(&f, cur, sizeof(float));
        *static_cast<double*>(i->p) = f;
      }
      cur += sizeof(float);
      break;

    case transfer_float_into_double_endianness:
      assert(cur + sizeof(float) <= buf_end);
      {
        union {
          uint8_t b[sizeof(float)];
          float f;
        } val;
        val.b[0] = cur[3];
        val.b[1] = cur[2];
        val.b[2] = cur[1];
        val.b[3] = cur[0];
        *static_cast<double*>(i->p) = val.f;
      }
      cur += sizeof(float);
      break;

    case Decision::transfer_varlen:
      {
        uint16_t varlen_length = decode_varlen_length(&cur, buf_end);
        assert(cur + varlen_length <= buf_end);
      
        IPFIX::BasicOctetArray* p
          = reinterpret_cast<IPFIX::BasicOctetArray*>(i->p);
        p->copy_content(cur, varlen_length);

        cur += varlen_length;
      }
      break;
    }
  }

  assert ((cur - buf) <= USHRT_MAX);
  return static_cast<uint16_t>(cur - buf);
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

    if (1 /* wire_templates contains template_id as key */) {
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
      wire_templates[current_template_id] = *current_wire_template;
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
    (*current_wire_template)[current_field_count++] = ie;
  }

  void DataSetDecoder::start_data_set(uint16_t id,
                                      uint16_t length,
                                      const uint8_t* buf) {
    if (1 /* wire_templates does not contain "id" as key */)
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
