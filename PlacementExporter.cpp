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
#include <cstdarg>
#include <sstream>

#include <unistd.h>

#include <boost/detail/endian.hpp>

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
      /** Value for an uninitialised decision type. */
      encode_none,

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
      encode_double_as_float,
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
    = Decision::encode_double_as_float;
#elif defined(BOOST_LITTLE_ENDIAN)
  Decision::decision_type_t encode_fixlen_maybe_endianness
    = Decision::encode_fixlen_endianness;
  Decision::decision_type_t encode_float_into_double_maybe_endianness
    = Decision::encode_double_as_float_endianness;
#else
#  error libfc does not compile on weird-endian machines.
#endif

  for (auto ie = placement_template->begin();
       ie != placement_template->end();
       ++ie) {
    assert(*ie != 0);
    assert((*ie)->ietype() != 0);

    Decision d;
    void* location;
    size_t size;

    /* Either g++ is too stupid to figure out that the relevant fields
     * will all be set in the various cases below (not even with -O3),
     * or I really have forgotten to set them.  Unfortunately, all the
     * error message says is that "warning:
     * ‘d.EncodePlan::Decision::xxx’ may be used uninitialized in this
     * function", and then pointing to the *declaration* of d, and not
     * at the places where it thinks that the variable might be used
     * uninitialised.  I'm therefore forced to initialise (possibly
     * redundantly) the members of the struct, just to shut the
     * compiler up. Not helpful. */
    d.type = Decision::encode_none;
    d.unencoded_length = 0;
    d.encoded_length = 0;

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
      assert(size <= sizeof(uint8_t));

      d.type = Decision::encode_fixlen;
      d.unencoded_length = sizeof(uint8_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kUnsigned16:
      assert(size <= sizeof(uint16_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint16_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kUnsigned32:
      assert(size <= sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint32_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kUnsigned64:
      assert(size <= sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint64_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kSigned8:
      assert(size <= sizeof(int8_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int8_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kSigned16:
      assert(size <= sizeof(int16_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int16_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kSigned32:
      assert(size <= sizeof(int32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int32_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kSigned64:
      assert(size <= sizeof(int64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(int64_t);
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kFloat32:
      /* Can't use reduced-length encoding on float; see RFC 5101,
       * Chapter 6, Verse 2. */
      assert(size == sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = sizeof(uint32_t);
      d.encoded_length = sizeof(uint32_t);
      break;

    case IPFIX::IEType::kFloat64:
      assert(size == sizeof(uint32_t)
             || size == sizeof(uint64_t));

      d.unencoded_length = sizeof(uint64_t);
      d.encoded_length = size;
      if (d.unencoded_length == sizeof(uint32_t))
        d.type = encode_float_into_double_maybe_endianness;
      else
        d.type = encode_fixlen_maybe_endianness;
      break;

    case IPFIX::IEType::kBoolean:
      assert(size == sizeof(uint8_t));

      d.type = Decision::encode_boolean;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;

    case IPFIX::IEType::kMacAddress:
      /* RFC 5101 says to treat MAC addresses as 6-byte integers,
       * but Brian Trammell says that this is wrong and that the
       * RFC will be changed.  If for some reason this does not
       * come about, replace "encode_fixlen" with
       * "encode_fixlen_maybe_endianness". */
      assert(size == 6*sizeof(uint8_t));
             
      d.type = Decision::encode_fixlen;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case IPFIX::IEType::kString:
      if (size == IPFIX::kVarlen) {
        d.type = Decision::encode_varlen;
      } else {
        d.type = Decision::encode_fixlen_octets;
        d.encoded_length = size;
      }
      break;

    case IPFIX::IEType::kDateTimeSeconds:
      /* Must be encoded as a "32-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.7.
       *
       * The standard doesn't say whether the integer in question is
       * signed or unsigned, but since there is additional information
       * saying that "[t]he 32-bit integer allows the time encoding up
       * to 136 years", this makes sense only if the integer in
       * question is unsigned (signed integers give 68 years, in
       * either direction from the epoch). */
      assert(size == sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case IPFIX::IEType::kDateTimeMilliseconds:
      /* Must be encoded as a "64-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.8.
       *
       * The standard doesn't say whether the integer in question is
       * signed or unsigned, but in analogy with dateTimeSeconds, we
       * will assume the unsigned variant. */
      assert(size == sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case IPFIX::IEType::kDateTimeMicroseconds:
      /* Must be encoded as a "64-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.9.
       *
       * The standard doesn't say whether the integer in question is
       * signed or unsigned, but in analogy with dateTimeSeconds, we
       * will assume the unsigned variant. */
      assert(size == sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case IPFIX::IEType::kDateTimeNanoseconds:
      /* Must be encoded as a "64-bit integer"; see RFC 5101, Chapter
       * 6, Verse 1.10.  See dateTimeMicroseconds above. */
      assert(size == sizeof(uint64_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
                
    case IPFIX::IEType::kIpv4Address:
      /* RFC 5101 says to treat all addresses as integers. This
       * would mean endianness conversion for all of these address
       * types, including MAC addresses and IPv6 addresses. But the
       * only reasonable address type with endianness conversion is
       * the IPv4 address.  If for some reason this is not correct
       * replace "encode_fixlen_maybe_endianness" with
       * "encode_fixlen".
       *
       * Also, treating addresses as integers would subject them to
       * reduced-length encoding, a concept that is quite bizarre
       * since you can't do arithmetic on addresses.  We will
       * therefore not accept reduced-length encoding on addresses.
       */
      assert(size == sizeof(uint32_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    case IPFIX::IEType::kIpv6Address:
      /* See comment on kIpv4Address. */
      assert(size == 16*sizeof(uint8_t));

      d.type = encode_fixlen_maybe_endianness;
      d.unencoded_length = size;
      d.encoded_length = size;
      break;
        
    default: 
      report_error("Unknown IE type");
      break;
    }
    
    if ((d.type == Decision::encode_fixlen 
         || d.type == Decision::encode_fixlen_endianness)
        && d.encoded_length > d.unencoded_length)
      report_error("IE %s encoded length %zu greater than native size %zu",
                   (*ie)->toIESpec().c_str(), d.encoded_length,
                   d.unencoded_length);

  }
}

std::string EncodePlan::Decision::to_string() const {
  std::stringstream sstr;

  sstr << "[";

  switch (type) {
  case encode_none: sstr << "encode_none"; break;
  case encode_boolean: sstr << "encode_boolean"; break;
  case encode_fixlen: sstr << "encode_fixlen"; break;
  case encode_fixlen_endianness: sstr << "encode_fixlen_endianness"; break;
  case encode_fixlen_octets: sstr << "encode_fixlen_octets"; break;
  case encode_varlen: sstr << "encode_varlen"; break;
  case encode_double_as_float_endianness:
    sstr << "encode_double_as_float_endianness";
    break;
  case encode_double_as_float:
    sstr << "encode_double_as_float";
    break;
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
