#include <cassert>

#include "Constants.h" 
#include "IETemplate.h" 
#include "Transcoder.h"

#include "exceptions/EncodingError.h"
#include "exceptions/FormatError.h"
#include "exceptions/IOError.h"

#include <boost/detail/endian.hpp>

namespace IPFIX {

const InfoElement Transcoder::u16ie("_internal_unsigned16",
                                    0, 0, IEType::unsigned16(),
                                    sizeof(uint16_t));
  
const InfoElement Transcoder::u32ie("_internal_unsigned32",
                                    0, 0, IEType::unsigned32(),
                                    sizeof(uint32_t));

  
/* 
* Transcode left-justified. When copying from a smaller field to
* a larger field, zero pads the end of the field. When copying
* from a larger field to a smaller field, cuts bytes from the
* end of the field.
*/
  static uint8_t *xcode_raw_left(const void *src, size_t s_len, 
                                       uint8_t *dst, size_t d_len) {
    if (s_len >= d_len) {
        memcpy(dst, src, d_len);
        dst += d_len;
    } else { // s_len < d_len, need to zero pad on the right
        memcpy(dst, src, s_len);
        memset(dst + s_len, 0, d_len - s_len);
        dst += d_len;
    }
    return dst;
}

/* 
* Transcode right-justified. When copying from a smaller field to
* a larger field, zero pads the beginning of the field. When copying
* from a larger field to a smaller field, cuts bytes from the
* beginning of the field.
*/
#if defined(BOOST_BIG_ENDIAN)
static uint8_t *xcode_raw_right(const void *src, size_t s_len, 
                                      uint8_t *dst, size_t d_len) {
    if (s_len >= d_len) {
        memcpy(dst, src + (s_len - d_len), d_len);
        dst += d_len;
    } else { // s_len < d_len, need to zero pad on the left
        memset(dst, 0, d_len - s_len);
        memcpy(dst + (d_len - s_len), src, s_len);
        dst += d_len;
    }
    return dst;
}
#endif

/* 
* Encode length as a varlen field
*/
static uint8_t *encode_varlen_length(uint8_t *dst, size_t varlen) {
    if (varlen < 255) {
        *dst++ = static_cast<uint8_t>(varlen);
    } else {
        *dst++ = 255;
        *dst++ = static_cast<uint8_t>(varlen >> 8);
        *dst++ = static_cast<uint8_t>(varlen & 0xFF);
    }
    return dst;
}

/* 
* Decode varlen field length
*/
static uint8_t *decode_varlen_length(uint8_t *src, size_t& varlen) {
    varlen = *(src++);
    if (varlen == 255) {
        varlen = *(src++);
        varlen = (varlen << 8) + *(src++);
    }
    return src;
}

/*
 * Swap byte order in place
 */
static void xcode_swap(uint8_t *buf, size_t len) {
    for (size_t i = 0 ; i < len/2; i++) {
        uint8_t t = buf[i];
        buf[i] = buf[(len-1)-i];
        buf[(len-1)-i] = t;
    }
}

size_t Transcoder::encodeAt(const void* val, size_t len, size_t off, const InfoElement* ie) {

    size_t ielen = ie->len();

    // Verify length
    if (ielen == kVarlen) {
        ielen = len;
        if (off + ielen + (ielen > 254 ? 3 : 1) > avail()) {
            return 0;
        }

        off = (encode_varlen_length(cur_ + off, ielen) - cur_);

    } else {
        if (off + ielen > avail()) {
            return 0;
        }
    }

    assert(ie->ietype()->permitsLength(ielen));

    size_t new_off;

    if (ie->ietype()->isEndian()) {
#if defined(BOOST_BIG_ENDIAN)
        new_off = xcode_raw_right(val, len, cur_ + off, ielen) - cur_;
#elif defined(BOOST_LITTLE_ENDIAN)
        new_off = xcode_raw_left(val, len, cur_ + off, ielen) - cur_;
        xcode_swap(cur_ + off, ielen);
#else
#error libfc does not compile on weird-endian machines.
#endif
    } else {
        new_off = xcode_raw_left(val, len, cur_ + off, ielen) - cur_; 
    }

    return new_off;
}

size_t Transcoder::encodeZeroAt(size_t len, size_t off) {  
  if (off + len > avail()) {
    return 0;
  }
  
  memset(cur_ + off, 0, len);
  return off + len;
}

bool Transcoder::encodeZero(const InfoElement* ie) {
  size_t ielen = ie->len();
  
  assert(ie->ietype()->permitsLength(ielen));
  
  if (ielen > avail()) {
    return false;
  }
  
  memset(cur_, 0, ielen);
  cur_ += ielen;
  
  return true;
}


bool Transcoder::encodeMessageStart() {
  if (msg_base_) {
    throw EncodingError("out-of-sequence call to encodeMessageStart()");
  }

  if (kMessageHeaderLen > avail()) {
    return false;
  }
  msg_base_ = cur_;

  // skip it, we'll encode everything later
  return advance(kMessageHeaderLen);
}

void Transcoder::encodeMessageEnd(uint32_t export_time, uint32_t sequence, uint32_t domain) {
  if (!msg_base_) {
    throw EncodingError("out-of-sequence call to encodeMessageEnd()");
  }

  // grab length and save current
  size_t msg_len = cur_ - msg_base_;
  uint8_t *save = cur_;

  // encode message header
  cur_ = msg_base_;
  if (!encode(static_cast<uint16_t>(kIpfixVersion)) ||
      !encode(static_cast<uint16_t>(msg_len)) ||
      !encode(export_time) || !encode(sequence) || !encode(domain)) {
    throw EncodingError("unexpected failure in encodeMessageEnd()");
  }

  //std::cerr << "encodeMessageEnd "<< export_time << " " << sequence << " " << domain << std::endl;
  
  // restore current pointer, mark no current message
  msg_base_ = NULL;
  cur_ = save;
}

bool Transcoder::encodeSetStart(uint16_t set_id) {
  if (set_base_) {
    throw EncodingError("out-of-sequence call to encodeSetStart()");
  }
  
  // precheck buffer for room for set header
  if (kSetHeaderLen > avail()) {
    return false;
  }
  set_base_ = cur_;
  
  // encode set ID, but skip length for later
  if (!encode(set_id) || !encode(static_cast<uint16_t>(0))) { 
    // we already checked for room, so we can only fail if the code is wrong.
    throw EncodingError("unexpected failure in encodeSetStart()");
  }
  
  return true;
}

void Transcoder::encodeSetEnd() {
  if (!set_base_) {
    throw EncodingError("out-of-sequence call to encodeSetEnd()");
  }

  // grab length and save current
  size_t set_len = cur_ - set_base_;
  uint8_t *save = cur_;
  
  // encode set length
  cur_ = set_base_ + kSetLenOffset;
  if (!encode(static_cast<uint16_t>(set_len))) {
    throw EncodingError("unexpected failure in encodeSetEnd()");
  }
  
  // restore current pointer, mark no current set
  set_base_ = NULL;
  cur_ = save;
}

size_t Transcoder::decodeAt(void* val, size_t len, size_t off, const InfoElement *ie) {
  
  size_t      ielen = ie->len();  
  uint8_t*    dp = cur_ + off;
  
  // Decode variable length
  if (ielen == kVarlen) {
    dp = decode_varlen_length(dp, ielen);
  }
  
  // Ensure there are enough bytes available in the buffer
  //if ((dp + ielen) - cur_ > avail()) {
  if (dp + ielen > cur_ + avail()) {
      return 0;
  }
  
  // Ensure the length is permitted for the IE
  assert(ie->ietype()->permitsLength(ielen));
  
  if (ie->ietype()->isEndian()) {
#if defined(BOOST_BIG_ENDIAN)
    (void)xcode_raw_right(dp, ielen, static_cast<uint8_t*>(val), len);
#elif defined(BOOST_LITTLE_ENDIAN)
    (void)xcode_raw_left(dp, ielen, static_cast<uint8_t*>(val), len);
    xcode_swap(static_cast<uint8_t*>(val), len);
#else
#error libfc does not compile on weird-endian machines.
#endif
  } else {
    (void)xcode_raw_left(dp, ielen, static_cast<uint8_t*>(val), len);
  }
  
  // return the offset after the decode
  return dp + ielen - cur_;
}

// bool Transcoder::decode(uint8_t* val, size_t len, const InfoElement *ie) {
//   
// //  fprintf(stderr, "xc 0x%016lx decode %50s at %4lu to 0x%016lx (len %lu)\n",
// //    base_, ie->toIESpec().c_str(), cur_ - base_, val, len);
//   
//   const IEType *iet = ie->ietype();
//   size_t ielen = ie->len();
//   
//   // Get variable length
//   if (ielen == kVarlen) {
//     cur_ = decode_varlen_length(cur_, ielen);
//   }
//   
//   // Ensure there are enough bytes available in the buffer
//   if (ielen > avail()) {
//       return false;
//   }
//   
//   // Ensure the length is permitted for the IE
//   assert(iet->permitsLength(ielen));
//   
//   if (iet->isEndian()) {
// #if defined(BOOST_BIG_ENDIAN)
//     (void)xcode_raw_right(cur_, ielen, val, len);
// #elif defined(BOOST_LITTLE_ENDIAN)
//     (void)xcode_raw_left(cur_, ielen, val, len);
//     xcode_swap(val, len);
// #else
// #error libfc does not compile on weird-endian machines.
// #endif
//   } else {
//     (void)xcode_raw_left(cur_, ielen, val, len);
//   }
//   
//   cur_ += ielen;
//   return true;
// }
  
bool Transcoder::decode(VarlenField *vf, const InfoElement *ie) {
  const IEType *iet = ie->ietype();
  size_t ielen = ie->len();
  
  // Get variable length
  if (ielen == kVarlen) {
    cur_ = decode_varlen_length(cur_, ielen);
  }

  // Ensure there are enough bytes available in the buffer
  if (ielen > avail()) {
    return false;
  }

  // Ensure the length is permitted for the IE
  assert(iet->permitsLength(ielen));
  
  // store pointer to content and length in vf
  vf->cp = cur_;
  vf->len = ielen;
  
  //std::cerr << "** decoding vf len " << ielen << std::endl;
  
  // and skip
  cur_ += ielen;
  return true;
}


bool Transcoder::decodeSkip(const InfoElement *ie) {
  size_t ielen = ie->len();
  
  // Get variable length
  if (ielen == kVarlen) {
    cur_ = decode_varlen_length(cur_, ielen);
  }

  // Ensure there are enough bytes available in the buffer
  if (ielen > avail()) {
    return false;
  }
  
  //std::cerr << "** skipping len " << ielen << std::endl;
  
  // and skip
  cur_ += ielen;
  return true;
}

size_t Transcoder::decodeVarlenLengthAt(size_t off, size_t& varlen) {
    if (off + 1 > avail()) return 0;
    
    varlen = cur_[off];
    if (varlen == 255) {
        if (off + 3 > avail()) return 0;
        varlen = cur_[off+1];
        varlen = (varlen << 8) + cur_[off+2];
        return off + 3;
    }
    return off + 1;
}

bool Transcoder::decodeMessageHeader(uint16_t& len, 
                                 uint32_t& export_time, 
                                 uint32_t& sequence, 
                                 uint32_t& domain) {

  if (kMessageHeaderLen > avail()) {
    return false;
  }

  uint16_t version;
  if (!decode(version)) {
    // we already checked for bytes, so we can only fail if the code is wrong.
    throw EncodingError("unexpected failure in decodeMessageHeader()");
  }
  
  // std::cerr << "decodeMessageHeader() version " << version << std::endl;
  
  if (version != kIpfixVersion) {
    throw FormatError("wrong IPFIX version; trying to decode non-IPFIX data?");
  }
  
  if (!decode(len)) {
    throw EncodingError("unexpected failure in decodeMessageHeader()");
  }
  
  // std::cerr << "decodeMessageHeader() length " << len << std::endl;
  
  if (len < kMessageHeaderLen) {
    throw FormatError("nonsensical message length; trying to decode non-IPFIX data?");
  }
  
  if (!decode(export_time)) {
    throw EncodingError("unexpected failure in decodeMessageHeader()");
  }
  
  // std::cerr << "decodeMessageHeader() export_time " << export_time << std::endl;

  
  if (!decode(sequence)) {
    throw EncodingError("unexpected failure in decodeMessageHeader()");
  }
  
  // std::cerr << "decodeMessageHeader() sequence " << sequence << std::endl;

  if (!decode(domain)) {
    throw EncodingError("unexpected failure in decodeMessageHeader()");
  }
  
  // std::cerr << "decodeMessageHeader() domain " << domain << std::endl;

  return true;
}

bool Transcoder::decodeSetHeader(uint16_t& sid, uint16_t& len) {                                   

  if (kSetHeaderLen > avail()) {
    return false;
  }
  
  if (!decode(sid)) {
    throw EncodingError("unexpected failure in decodeMessageHeader()");
  }
  if (sid < kMinSetID && 
      sid != kTemplateSetID && 
      sid != kOptionTemplateSetID)
  {
    throw FormatError("nonsensical set ID; trying to decode non-IPFIX data?");
  }
  
  if (!decode(len)) {
    throw EncodingError("unexpected failure in decodeMessageHeader()");
  }
  if (len < kSetHeaderLen || len > kMaxSetLen) {
    throw FormatError("nonsensical set length; trying to decode non-IPFIX data?");
  }
  
  return true;
}

void Transcoder::focus(size_t off, size_t len) {
  checkpoint();
  cur_ = base_ + off;

  assert(savemax_ == 0);

  savemax_ = max_;
  if (max_ > cur_ + len) max_ = cur_ + len;
  //      fprintf(stderr, "xc 0x%016lx   focus from %u to %u\n",
  //              base_, cur_ - base_, max_ - base_);
}

void Transcoder::defocus() { 
  assert (savemax_ != 0);

  rollback();
  max_ = savemax_;
  savemax_ = NULL;
  //      fprintf(stderr, "xc 0x%016lx defocus from %u to %u\n",
  //              base_, cur_ - base_, max_ - base_);
}

} // namespace IPFIX
