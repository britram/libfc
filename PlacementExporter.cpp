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
#include <climits>
#include <ctime>
#include <sstream>

#include <unistd.h>

#include "PlacementExporter.h"

#include "exceptions/ExportError.h"

/** Encode plans describe how a data record is to be encoded.
 *
 * Decoding a data record means determining, for each data field, 
 *
 *   - if the data's endianness must be converted;
 *   - if the data needs to be transformed in any other way (for
 *     example, boolean values are encoded with 1 meaning true and 2
 *     meaning false(!!), or reduced-length encoding of floating-point
 *     values means that doubles are really transferred as floats); and
 *   - for variable-length data, what the length of the encoded value
 *     is.
 *
 * See also the documentation for DecodePlan in DataSetDecoder.cpp
 */
class EncodePlan {
public:
  /** Creates an encoding plan from a placement template.
   *
   * @param placement_template a placement template from which we
   *   encode a data record.
   */
  EncodePlan(const IPFIX::PlacementTemplate* placementTemplate);

  /** Executes this plan.
   *
   * @param buf the buffer where to store the encoded values
   * @param offset the offset at which to store the values
   * @param length the total length of the buffer
   *
   * @return the number of encoded octets
   */
  uint16_t execute(uint8_t* buf, uint16_t offset, uint16_t length);
  
private:
  struct Decision {
    /** The decision type. */
    enum decision_type_t {
      /** Encode a boolean.  I'm repeating here the comment I made in
       * the corresponding declaration for transfer_boolean in
       * DataSetDecoder.cpp, because it still gets my blood up:
       *
       * Someone found it amusing in RFC 2579 to encode the boolean
       * values true and false as 1 and 2, respectively [sic!].  And
       * someone else found it amusing to standardise this behaviour
       * in RFC 5101 too.  This is of course wrong, since it disallows
       * entirely sensible operations like `plus' for "or", `times'
       * for "and" and `less than' for implication (which is what you
       * get when you make false less than true).
       *
       * This is why we can't subsume the encoding of booleans (which
       * are fixlen-encoded values of length 1) under
       * encode_basic_no_endianness below. */
      encode_boolean,

      /** Encode a basic type (fixlen) with no endianness conversion. */
      encode_fixlen,

      /** Encode a basic type (fixlen) with endianness conversion. */
      encode_fixlen_endianness,

      /** Encode a BasicOctetArray as fixlen. */
      encode_fixlen_octets,

      /** Encode a BasicOctetArray as varlen. Varlen encoding is
       * supported only for BasicOctetArray and derived classes.  In
       * all other instances, I'll do what Brian Trammell recommended
       * I do and tell the user to eff off. */
      encode_varlen,

      /** Encode double as float with endianness conversion. */
      encode_double_as_float_endianness,

      /** Encode double as float, no endianness conversion. */
      encode_double_as_float_no_endianness,
    } type;

    /** Address where original value is to be found. */
    const void* address;

    /** Size of original (unencoded) data. 
     *
     * If type is encode_varlen or encode_double_as_float or
     * encode_fixlen_octets, this field is implied and may not contain
     * a valid value.
     */
    size_t unencoded_length;

    /** Requested size of encoded data. 
     *
     * If type is encode_varlen or encode_double_as_float, this field
     * is implied and may not contain a valid value.
     */
    size_t encoded_length;

    /** Creates a printable version of this encoding decision. 
     *
     * @return a printable version of this encoding decision
     */
    std::string to_string() const;
  };

  std::vector<Decision> plan;
};

static void report_error(const char* message, ...) {
  static const size_t buf_size = 10240;
  static char buf[buf_size];
  va_list args;
  
  va_start(args, message);
  int nchars = vsnprintf(buf, buf_size, message, args);
  va_end(args);

  if (nchars < 0)
    strcpy(buf, "Error while formatting error message");
  else if (static_cast<unsigned int>(nchars) > buf_size - 1 - 3) {
    buf[buf_size - 4] = '.';
    buf[buf_size - 3] = '.';
    buf[buf_size - 2] = '.';
    buf[buf_size - 1] = '\0';   // Shouldn't be necessary
  }

  throw IPFIX::ExportError(buf);
}

/* See DataSetDecoder::DecodePlan::DecodePlan. */
EncodePlan::EncodePlan(const IPFIX::PlacementTemplate* placement_template) {
#if defined(BOOST_BIG_ENDIAN)
  Decision::decision_type_t encode_fixlen_maybe_endianness
    = Decision::encode_fixlen;
  Decision::decision_type_t encode_double_as_float_maybe_endianness
    = Decision::encode_double_as_float_no_endianness;
#elif defined(BOOST_LITTLE_ENDIAN)
  Decision::decision_type_t transfer_fixlen_maybe_endianness
    = Decision::encode_fixlen_endianness;
  Decision::decision_type_t transfer_float_into_double_maybe_endianness
    = Decision::encode_double_as_float_endianness;
#else
#  error libfc does not compile on weird-endian machines.
#endif

  for (auto ie = placement_template.begin();
       i != placement_template.end();
       ++i) {
    assert(*ie != 0);
    assert((*ie)->ietype != 0);

    Decision d;
    void* location;
    size_t size;
    
    /* The IE *must* be present in the placement template. If not,
     * there is something very wrong in the PlacementTemplate
     * implementation. */
    assert(placement_template->lookup_placement(*ie, &location, &size));

    switch ((*ie)->ietype()->number()) {
    case IPFIX::IEType::kOctetArray: 
      if (size == IPFIX::kVarlen) {
        d.type = Decision::encode_varlen;
      } else {
        d.type = Decision::encode_fixlen_octets;
        d.encoded_length = size;
      }
      break;

    case IPFIX::IEType::kUnsigned8:
      assert((*ie)->len() == 1);

      d.type = Decision::encode_fixlen;
      d.unencoded_length = sizeof(uint8_t);
      d.encoded_length = (*ie)->len();
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kUnsigned16:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint16_t);
      d.encoded_length = (*ie)->len();
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kUnsigned32:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(uint32_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("enc IE %s length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kUnsigned64:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(uint64_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kSigned8:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(int8_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kSigned16:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(int16_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kSigned32:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(int32_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kSigned64:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(int64_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kFloat32:
      d.type = transfer_fixlen_maybe_endianness;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(uint32_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kFloat64:
      assert((*ie)->len() == sizeof(float)
             || d.length == sizeof(double));
      d.length = (*ie)->len();
      if (d.length == sizeof(float))
        d.type = transfer_float_into_double_maybe_endianness;
      else
        d.type = transfer_fixlen_maybe_endianness;
      d.destination_size = sizeof(double);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kBoolean:
      d.type = Decision::transfer_boolean;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = sizeof(uint8_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;

    case IPFIX::IEType::kMacAddress:
      /* RFC 5101 says to treat MAC addresses as 6-byte integers,
       * but Brian Trammell says that this is wrong and that the
       * RFC will be changed.  If for some reason this does not
       * come about, replace "transfer_fixlen" with
       * "transfer_fixlen_maybe_endianness". */
      d.type = Decision::transfer_fixlen;
      d.unencoded_length = (*ie)->len();
      d.encoded_length = 6*sizeof(uint8_t);
      if (d.length != 6)
        report_error("MAC IE not 6 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
      break;
        
    case IPFIX::IEType::kString:
      if ((*ie)->len() == IPFIX::kVarlen) {
        d.type = Decision::transfer_varlen;
      } else {
        d.type = Decision::transfer_fixlen_octets;
        d.length = (*ie)->len();
      }
      break;

    case IPFIX::IEType::kDateTimeSeconds:
      d.type = transfer_fixlen_maybe_endianness;
      d.length = (*ie)->len();
      d.destination_size = sizeof(uint32_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;
        
    case IPFIX::IEType::kDateTimeMilliseconds:
      d.type = transfer_fixlen_maybe_endianness;
      d.length = (*ie)->len();
      d.destination_size = sizeof(uint64_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;
        
    case IPFIX::IEType::kDateTimeMicroseconds:
      d.type = transfer_fixlen_maybe_endianness;
      // RFC 5101, Chapter 6, Verse 2
      assert((*ie)->len() == sizeof(uint64_t));
      d.length = (*ie)->len();
      d.destination_size = sizeof(uint64_t);
      if (d.length > d.destination_size)
        report_error("IE %s length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.length, d.destination_size);
      break;
        
    case IPFIX::IEType::kDateTimeNanoseconds:
      d.type = transfer_fixlen_maybe_endianness;
      // RFC 5101, Chapter 6, Verse 2
      assert((*ie)->len() == sizeof(uint64_t));
      d.length = (*ie)->len();
      d.destination_size = sizeof(uint64_t);
      if (d.encoded_length > d.unencoded_length)
        report_error("IE %s enc length %zu greater than native size %zu",
                     (*ie)->toIESpec().c_str(), d.encoded_length,
                     d.destination_size);
      break;
        
    case IPFIX::IEType::kIpv4Address:
      /* RFC 5101 says to treat all addresses as integers. This
       * would mean endianness conversion for all of these address
       * types, including MAC addresses and IPv6 addresses. But the
       * only reasonable address type with endianness conversion is
       * the IPv4 address.  If for some reason this is not correct
       * replace "transfer_fixlen_maybe_endianness" with
       * "transfer_fixlen". */
      d.type = transfer_fixlen_maybe_endianness;
      d.length = (*ie)->len();
      d.destination_size = sizeof(uint32_t);
      if (d.length != 4)
        report_error("IPv4 Address IE not 4 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
      break;
        
    case IPFIX::IEType::kIpv6Address:
      /* RFC 5101 says to treat IPv6 addresses as 16-byte integers,
       * but Brian Trammell says that this is wrong and that the
       * RFC will be changed.  If for some reason this does not
       * come about, replace "transfer_fixlen" with
       * "transfer_fixlen_maybe_endianness". */
      d.type = Decision::transfer_fixlen;
      d.length = (*ie)->len();
      d.destination_size = 16*sizeof(uint8_t);
      if (d.encoded_length != 16)
        report_error("IPv6 Address IE not 16 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
      break;
        
    default: 
      report_error("Unknown IE type");
      break;
    }
  }
}

std::string EncodePlan::Decision::to_string() const {
  std::stringstream sstr;

  sstr << "[";

  switch (type) {
  case encode_boolean: sstr << "encode_boolean"; break;
  case encode_basic_no_endianness: sstr << "encode_basic_no_endianness"; break;
  case encode_basic_endianness: sstr << "encode_basic_endianness"; break;
  case encode_fixlen_octet_array: sstr << "encode_fixlen_octet_array"; break;
  case encode_varlen: sstr << "encode_varlen"; break;
  case encode_double_as_float: sstr << "encode_double_as_float"; break;
  }

  sstr << "@" << address << "[" << encoded_length << "]";
  return sstr.str();
}


namespace IPFIX {

  PlacementExporter::PlacementExporter(ExportDestination& _os,
                                       uint32_t _observation_domain)
    : os(_os),
      current_template(0),
      sequence_number(0),
      observation_domain(_observation_domain), 
      n_message_octets(kMessageHeaderLen),
      template_set_index(-1) {
    /* Push an empty iovec into the iovec vector, to be filled later
     * with the message header by flush(). */
    iovecs.push_back(::iovec());
  }

  PlacementExporter::~PlacementExporter() {
    flush();

    for (auto i = iovecs.begin(); i != iovecs.end(); ++i)
      delete[] static_cast<uint8_t*>(i->iov_base);
  }

  static void encode16(uint16_t val, uint8_t** buf,
                       const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint16_t) <= buf_end);
    *(*buf)++ = (val >> 8) & 0xff;
    *(*buf)++ = (val >> 0) & 0xff;
    assert(*buf <= buf_end);
  }

  static void encode32(uint32_t val, uint8_t** buf,
                       const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint32_t) <= buf_end);
    *(*buf)++ = (val >> 24) & 0xff;
    *(*buf)++ = (val >> 16) & 0xff;
    *(*buf)++ = (val >>  8) & 0xff;
    *(*buf)++ = (val >>  0) & 0xff;
    assert(*buf <= buf_end);
  }

  bool PlacementExporter::flush() {
    /** This message header.
     *
     * This variable is dynamically allocated so as to facilitate
     * its deletion later as part of the iovecs vector. */
    uint8_t* message_header = new uint8_t[kMessageHeaderLen];

    /** Points to the end of this message.
     *
     * Used for range checks. */
    const uint8_t* message_end = message_header + kMessageHeaderLen;

    /** Moves through the message header. */
    uint8_t* p = message_header;

    /** Return value. */
    int ret = 0;

    /* Only write something if we have anything nontrivial to write. */
    if (n_message_octets > kMessageHeaderLen) {
      time_t now = time(0);
      if (now == static_cast<time_t>(-1))
        return false;
      
      encode16(kIpfixVersion, &p, message_end);
      encode16(static_cast<uint16_t>(n_message_octets), &p, message_end);
      encode32(static_cast<uint32_t>(now), &p, message_end);
      encode32(sequence_number++, &p, message_end);
      encode32(observation_domain, &p, message_end);
      
      ::iovec& header = iovecs.at(0);
      
      header.iov_base = message_header;
      header.iov_len = kMessageHeaderLen;
      
      ret = os.writev(iovecs);
      
      for (auto i = iovecs.begin(); i != iovecs.end(); ++i)
        delete[] static_cast<uint8_t*>(i->iov_base);
      iovecs.clear();

      /* Space for next message header. */
      iovecs.push_back(::iovec());

      template_set_index = -1;
      
      n_message_octets = kMessageHeaderLen;
    }
    return ret;
  }

  void PlacementExporter::place_values(const PlacementTemplate* tmpl) {
    /** The number of bytes added to the current message as a result
     * of issuing this new data record.  It might be as small as the
     * number of bytes in the representation of this data record, and
     * it might be as large as that number, plus the size of a new
     * template set containing the wire template for the template
     * used. */
    size_t new_bytes = tmpl->data_record_size();

    /** Will contain tmpl if this template is hitherto unknown. */
    const PlacementTemplate* unknown_template = 0;

    if (tmpl != current_template) {
      /* We need to insert a new template and start a new data set if
       *
       *  - the underlying transport is connection-oriented and we
       *    have never seen the template; or
       *  - the underlying transport is connectionless and we haven't
       *    seen the template in this message so far.
       */
      if (used_templates.find(tmpl) == used_templates.end()) {
        unknown_template = tmpl;

        /* Need to add a new template to the template record section */
        size_t template_bytes = 0;
        // FIXME: should be somethign like this:
        //   tmpl->wire_template(new_template_id(), 0,
        //   &template_bytes);
        // but is something like this instead
        tmpl->wire_template(1234, 0, &template_bytes);
        new_bytes += template_bytes;

        /* Need to create template set? */
        if (template_set_index == -1)
          new_bytes += kSetHeaderLen;
      }

      /* Finish current data set. */
      // FIXME

      /* Open a new data set. */
      new_bytes += kSetHeaderLen;

      iovecs.push_back(::iovec());
      iovec& l = iovecs.back();

      l.iov_base = new uint8_t[kMaxMessageLen];
      l.iov_len = kSetHeaderLen;

      /* Switch to another template for this new data set. */
      current_template = tmpl;
    }

    if (n_message_octets + new_bytes > os.preferred_maximum_message_size()) {
      flush();
      unknown_template = tmpl;
    } else {
    }

    n_message_octets += new_bytes;

    if (template_set_index == -1) {
      template_set_index = iovecs.size();
      iovecs.push_back(::iovec());
    }

    if (unknown_template != 0)
      used_templates.insert(unknown_template);
  }

} // namespace IPFIX
