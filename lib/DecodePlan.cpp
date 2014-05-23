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
#include <climits>
#include <sstream>

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include "BasicOctetArray.h"
#include "DecodePlan.h"
#include "PlacementTemplate.h"

#include "decode_util.h"
#include "ipfix_endian.h"

#include "exceptions/FormatError.h"


namespace LIBFC {

  std::string DecodePlan::Decision::to_string() const {
    std::stringstream sstr;

    sstr << "[";
    switch (type) {
    case skip_fixlen:
      sstr << "skip_fixlen " << length; break;
    case skip_varlen:
      sstr << "skip_varlen"; break;
    case transfer_fixlen:
      sstr << "transfer_fixlen " << length 
	   << "/" << destination_size; break;
    case transfer_boolean:
      sstr << "transfer_boolean"; break;
    case transfer_fixlen_endianness:
      sstr << "transfer_fixlen_endianness " << length
	   << "/" << destination_size; break;
    case transfer_fixlen_octets:
      sstr << "transfer_fixlen_octets " << length; break;
    case transfer_float_into_double:
      sstr << "transfer_float_into_double"; break;
    case transfer_float_into_double_endianness:
      sstr << "transfer_float_into_double_endianness"; break;
    case transfer_varlen:
      sstr << "transfer_varlen"; break;
    };
    sstr << "]";
  
    return sstr.str();
  }


  DecodePlan::DecodePlan(const LIBFC::PlacementTemplate* placement_template,
			 const LIBFC::MatchTemplate* wire_template) 
    : plan(wire_template->size())
#ifdef _LIBFC_HAVE_LOG4CPLUS_
    ,
      logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DecodePlan")))
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
  {

    LOG4CPLUS_TRACE(logger, "ENTER DecodePlan::DecodePlan (wt with "
		    << wire_template->size() << " entries)");

#if defined(IPFIX_BIG_ENDIAN)
    Decision::decision_type_t transfer_fixlen_maybe_endianness
      = Decision::transfer_fixlen;
    Decision::decision_type_t transfer_float_into_double_maybe_endianness
      = Decision::transfer_float_into_double;
#elif defined(IPFIX_LITTLE_ENDIAN)
    Decision::decision_type_t transfer_fixlen_maybe_endianness
      = Decision::transfer_fixlen_endianness;
    Decision::decision_type_t transfer_float_into_double_maybe_endianness
      = Decision::transfer_float_into_double_endianness;
#else
#  error libfc does not compile on weird-endian machines.
#endif

    unsigned int decision_number = 0;
    for (auto ie = wire_template->begin(); ie != wire_template->end(); ie++) {
      assert(*ie != 0);
      LOG4CPLUS_TRACE(logger, "  decision " << (decision_number + 1)
		      << ": looking up placement");// for " << (*ie)->toIESpec());

      Decision d;

      if (placement_template->lookup_placement(*ie, &d.p, 0)) { /* IE present */
	LOG4CPLUS_TRACE(logger, "    found -> transfer");
	d.wire_ie = *ie;

	/* This object is needed for an interesting reason.  Previous
	 * versions of this code had lines like these:
	 *
	 *   report_error("IE %s has NULL ietype", (*ie)->toIESpec().c_str());
	 *
	 * The problem with this line is that the temporary object
	 * created by toIESpec() may already be destroyed by the time
	 * report_error() is being called, and then c_str() is no
	 * longer valid.
	 */
	std::string ie_spec = (*ie)->toIESpec();

	if ((*ie)->ietype() == 0)
	  report_error("IE %s has NULL ietype", ie_spec.c_str());

	/* There is some code duplication going on here, but unless
	 * someone can demonstrate to me that this leads to higher
	 * maintenance costs, I'd like to keep each IE type separate,
	 * until this code has been fully debugged. */
	switch ((*ie)->ietype()->number()) {
	case LIBFC::IEType::kOctetArray: 
	  if ((*ie)->len() == LIBFC::kIpfixVarlen) {
	    d.type = Decision::transfer_varlen;
	  } else {
	    d.type = Decision::transfer_fixlen_octets;
	    d.length = (*ie)->len();
	  }
	  break;

	case LIBFC::IEType::kUnsigned8:
	  d.type = Decision::transfer_fixlen;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint8_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kUnsigned16:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint16_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kUnsigned32:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint32_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kUnsigned64:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint64_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kSigned8:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(int8_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kSigned16:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(int16_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kSigned32:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(int32_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kSigned64:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(int64_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kFloat32:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(float);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kFloat64:
	  assert((*ie)->len() == sizeof(float)
		 || d.length == sizeof(double));
	  d.length = (*ie)->len();
	  if (d.length == sizeof(float))
	    d.type = transfer_float_into_double_maybe_endianness;
	  else
	    d.type = transfer_fixlen_maybe_endianness;
	  d.destination_size = sizeof(double);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kBoolean:
	  d.type = Decision::transfer_boolean;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint8_t); 
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;

	case LIBFC::IEType::kMacAddress:
	  /* RFC 5101 says to treat MAC addresses as 6-byte integers,
	   * but Brian Trammell says that this is wrong and that the
	   * RFC will be changed.  If for some reason this does not
	   * come about, replace "transfer_fixlen" with
	   * "transfer_fixlen_maybe_endianness". */
	  d.type = Decision::transfer_fixlen;
	  d.length = (*ie)->len();
	  d.destination_size = 6*sizeof(uint8_t);
	  if (d.length != 6)
	    report_error("MAC IE not 6 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
	  break;
        
	case LIBFC::IEType::kString:
	  if ((*ie)->len() == LIBFC::kIpfixVarlen) {
	    d.type = Decision::transfer_varlen;
	  } else {
	    d.type = Decision::transfer_fixlen_octets;
	    d.length = (*ie)->len();
	  }
	  break;

	case LIBFC::IEType::kDateTimeSeconds:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint32_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;
        
	case LIBFC::IEType::kDateTimeMilliseconds:
	  d.type = transfer_fixlen_maybe_endianness;
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint64_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;
        
	case LIBFC::IEType::kDateTimeMicroseconds:
	  d.type = transfer_fixlen_maybe_endianness;
	  // RFC 5101, Chapter 6, Verse 2
	  assert((*ie)->len() == sizeof(uint64_t));
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint64_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;
        
	case LIBFC::IEType::kDateTimeNanoseconds:
	  d.type = transfer_fixlen_maybe_endianness;
	  // RFC 5101, Chapter 6, Verse 2
	  assert((*ie)->len() == sizeof(uint64_t));
	  d.length = (*ie)->len();
	  d.destination_size = sizeof(uint64_t);
	  if (d.length > d.destination_size)
	    report_error("IE %s length %zu greater than native size %zu",
			 ie_spec.c_str(), d.length, d.destination_size);
	  break;
        
	case LIBFC::IEType::kIpv4Address:
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
        
	case LIBFC::IEType::kIpv6Address:
	  /* RFC 5101 says to treat IPv6 addresses as 16-byte integers,
	   * but Brian Trammell says that this is wrong and that the
	   * RFC will be changed.  If for some reason this does not
	   * come about, replace "transfer_fixlen" with
	   * "transfer_fixlen_maybe_endianness". */
	  d.type = Decision::transfer_fixlen;
	  d.length = (*ie)->len();
	  d.destination_size = 16*sizeof(uint8_t);
	  if (d.length != 16)
	    report_error("IPv6 Address IE not 16 octets long (c.f. RFC 5101, Chapter 6, Verse 2");
	  break;
        
	default: 
	  report_error("Unknown IE type");
	  break;
	}
      } else {                    /* Encode skip decision */
	LOG4CPLUS_TRACE(logger, "    not found -> skip");
	if ((*ie)->len() == LIBFC::kIpfixVarlen) {
	  d.type = Decision::skip_varlen;
	} else {
	  d.type = Decision::skip_fixlen;
	  d.length = (*ie)->len();
	}
      }

      plan[decision_number++] = d;
      LOG4CPLUS_TRACE(logger, "  decision " << decision_number
		      << " entered as " << d.to_string());
    }

    /* Coalesce adjacent skip_fixlen decisions. */
    for (auto decision = plan.begin(); decision != plan.end(); ++decision) {
      if (decision->type == Decision::skip_fixlen) {
	auto skips = decision;
	auto next = decision;
	uint16_t length = decision->length;
      
	++next;
	for (++skips;
	     skips != plan.end() && skips->type == Decision::skip_fixlen;
	     ++skips)
	  length += skips->length;
	plan.erase(next, skips);
	decision->length = length;
      }
    }

#ifdef _LIBFC_HAVE_LOG4CPLUS_
    if (logger.getLogLevel() <= log4cplus::DEBUG_LOG_LEVEL) {
      LOG4CPLUS_TRACE(logger, "  plan is: ");
      for (auto d = plan.begin(); d != plan.end(); ++d)
	LOG4CPLUS_TRACE(logger, "    " << d->to_string());
    }
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

    LOG4CPLUS_TRACE(logger, "LEAVE DecodePlan::DecodePlan");
  }

  static uint16_t decode_varlen_length(const uint8_t** cur,
				       const uint8_t* buf_end) {
    uint16_t ret = 0;

    if (*cur >= buf_end) 
      report_error("first octet of varlen length encoding beyond buffer");
    ret = **cur;

    if (ret < UCHAR_MAX)
      (*cur)++;
    else {
      if (*cur + 3 > buf_end) 
	report_error("three-byte varlen length encoding beyond buffer");
      (*cur)++;
      /* Assume that the two length-carrying octets are in network byte
       * order */
      ret = (*(*cur + 0) << 8) + *(*cur + 1);
      *cur += 2;

      /* If it turns out that the three-byte encoding must not be used
       * for values < 255, then uncomment the following two lines: */
      //if (ret < UCHAR_MAX)
      //  report_error("three-byte varlen encoding used for value < 255");
    }

    if (*cur + ret > buf_end) {
      std::stringstream sstr;
      sstr << "varlen length " << ret
	   << " (0x" << std::hex << ret << ")" << std::dec
	   << " goes beyond buffer (only " << (buf_end - *cur)
	   << " bytes left";
      report_error(sstr.str());
    }

    return ret;
  }

#if defined(_LIBFC_HAVE_LOG4CPLUS_) && defined(_LIBFC_DO_HEXDUMP_)
  static void hexdump(log4cplus::Logger& logger, const uint8_t* buf,
		      const uint8_t* buf_end) {
    char out_buf[81];
    const uint8_t* p = buf;
    size_t size = sizeof(out_buf);
    for (; p < buf_end; p += 16) {
      snprintf(out_buf, size, "%08x", static_cast<unsigned int>(p - buf));
      size -= 8;
      for (int i = 0; i < 16 && p < buf_end; i++) {
	snprintf(out_buf + 8 + 3*i, size, " %02x", p[i]);
	size -= 3;
      }
      LOG4CPLUS_TRACE(logger, out_buf);
    }
  }
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */

  uint16_t DecodePlan::execute(const uint8_t* buf, uint16_t length) {
    LOG4CPLUS_TRACE(logger, "ENTER DecodePlan::execute");

    const uint8_t* cur = buf;
    const uint8_t* buf_end = buf + length;

    for (auto i = plan.begin(); i != plan.end(); ++i) {
      assert(cur < buf_end);

      LOG4CPLUS_TRACE(logger, "  decision: " << i->type);

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
	assert(cur + 1 <= buf_end);
	// Undo RFC 2579 madness
	{
	  bool *q = static_cast<bool*>(i->p);
	  if (*cur == 1)
	    *q = 1;
	  else if (*cur == 2)
	    *q = 0;
	  else
	    report_error("bool encoding wrong");
	}
	cur++;
	break;

      case Decision::transfer_fixlen:
	if (cur + i->length > buf_end) {
	  std::string ie_spec = i->wire_ie->toIESpec();
	  report_error("IE %s length beyond buffer: cur=%p, ielen=%zu, end=%p",
		       ie_spec.c_str(), cur, i->length, buf_end);
	}

	assert(i->length <= i->destination_size);

	/* Assume all-zero bit pattern is zero, null, 0.0 etc. */
	// FIXME: Check if transferring native data types is faster
	// (e.g., short when i->length == 2, long when i->length == 4
	// etc).
	{
	  uint8_t* q = static_cast<uint8_t*>(i->p);
	  LOG4CPLUS_TRACE(logger, "  fixlen: q == " << static_cast<void*>(q));

	  memset(q, '\0', i->destination_size);
	  // Intention: right-justify value at cur in field at i->p
	  memcpy(q + i->destination_size - i->length, cur, i->length);
	}
	cur += i->length;
	break;

      case Decision::transfer_fixlen_endianness:
	if (cur + i->length > buf_end) {
	  std::string ie_spec = i->wire_ie->toIESpec();
	  report_error("IE %s length beyond buffer: cur=%p, ielen=%zu, end=%p",
		       ie_spec.c_str(), cur, i->length, buf_end);
	}

	assert(i->length <= i->destination_size);

#if defined(_LIBFC_HAVE_LOG4CPLUS_) && defined(_LIBFC_DO_HEXDUMP_)
        {
          LOG4CPLUS_TRACE(logger, "transfer w/endianness Before");
          hexdump(logger, buf, cur);
          LOG4CPLUS_TRACE(logger, "At and after");
          hexdump(logger, cur, cur + 8);
        }
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */

	/* Assume all-zero bit pattern is zero, null, 0.0 etc. */
	// FIXME: Check if transferring native data types is faster
	// (e.g., short when i->length == 2, long when i->length == 4
	// etc).
	{
	  uint8_t* q = static_cast<uint8_t*>(i->p);        
	  LOG4CPLUS_TRACE(logger, "  fixlen_endianness: q == " << static_cast<void*>(q) << ", size=" << i->destination_size);
	  memset(q, '\0', i->destination_size);
	  LOG4CPLUS_TRACE(logger, "  memset done");
	  // Intention: left-justify value at cur in field at i->p
	  for (uint16_t k = 0; k < i->length; k++)
	    q[k] = cur[i->length - (k + 1)];
	}
	LOG4CPLUS_TRACE(logger, "  transfer done");
	cur += i->length;
	break;

      case Decision::transfer_fixlen_octets:
	assert(cur + i->length <= buf_end);
	{ 
	  LIBFC::BasicOctetArray* p
	    = reinterpret_cast<LIBFC::BasicOctetArray*>(i->p);
	  p->copy_content(cur, i->length);
	}

	cur += i->length;
	break;

      case Decision::transfer_float_into_double:
	assert(cur + sizeof(float) <= buf_end);
	{
	  float f;
	  memcpy(&f, cur, sizeof(float));
	  *static_cast<double*>(i->p) = f;
	}
	cur += sizeof(float);
	break;

      case Decision::transfer_float_into_double_endianness:
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
	cur += sizeof(uint32_t);
	break;

      case Decision::transfer_varlen:
	{
#if defined(_LIBFC_HAVE_LOG4CPLUS_) && defined(_LIBFC_DO_HEXDUMP_)
	  {
	    LOG4CPLUS_TRACE(logger, "Before");
	    hexdump(logger, buf, cur);
	    LOG4CPLUS_TRACE(logger, "At and after");
	    hexdump(logger, cur, std::min(cur + 12, buf_end));
	  }
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */
	  uint16_t varlen_length = decode_varlen_length(&cur, buf_end);
	  LOG4CPLUS_TRACE(logger, "  varlen length " << varlen_length);
	  assert(cur + varlen_length <= buf_end);
      
	  LIBFC::BasicOctetArray* p
	    = reinterpret_cast<LIBFC::BasicOctetArray*>(i->p);
	  p->copy_content(cur, varlen_length);

	  cur += varlen_length;
	}
	break;
      }
    }

    assert ((cur - buf) <= USHRT_MAX);
    return static_cast<uint16_t>(cur - buf);
  }

} /* namespace LIBFC */
