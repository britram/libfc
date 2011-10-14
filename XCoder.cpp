#include "XCoder.h"
#include "Constants.h" 
#include "IETemplate.h" 

#include <boost/detail/endian.hpp>

namespace IPFIX {

static uint8_t *xcode_raw_left(uint8_t *src, size_t s_len, 
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

#if defined(BOOST_BIG_ENDIAN)
static uint8_t *xcode_raw_right(uint8_t *src, size_t s_len, 
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

static uint8_t *decode_varlen_length(uint8_t *src, size_t& varlen) {
  varlen = *(src++);
  if (varlen == 255) {
    varlen = ((*(src++)) << 8) + (*(src++));
  }
  return src;
}
  
static void xcode_swap(uint8_t *buf, size_t len) {
  for (size_t i = 0 ; i < len/2; i++) {
    uint8_t t = buf[i];
    buf[i] = buf[(len-1)-i];
    buf[(len-1)-i] = t;
  }
}

bool XCoder::encode(uint8_t* val, size_t len, const InfoElement* ie) {
  const IEType *iet = ie->ietype();
  size_t ielen = ie->len();
  
  // Verify length
  if (ielen == kVarlen) {
    ielen = len;
    if (ielen + (ielen > 254 ? 3 : 1) > avail()) {
      return false;
    }
    cur_ = encode_varlen_length(cur_, ielen);
  } else {
    if (ielen > avail()) {
      return false;
    }
  }
  if (!iet->permitsLength(ielen)) {
    throw IETypeError("IE type size mismatch");
  }
  
  if (iet->isEndian()) {
#if defined(BOOST_BIG_ENDIAN)
    cur_ = xcode_raw_right(val, len, cur_, ielen);
#elif defined(BOOST_LITTLE_ENDIAN)
    uint8_t* next = xcode_raw_left(val, len, cur_, ielen);
    xcode_swap(cur_, ielen);
    cur_ = next;
#else
#error libfc does not compile on weird-endian machines.
#endif
  } else {
    cur_ = xcode_raw_left(val, len, cur_, ielen);
  }
  
  return true;
}

bool XCoder::encode(const VarlenField* vf, const InfoElement *ie) {
  return this->encode(vf->cp, vf->len, ie);
}

bool XCoder::encodeZero(const InfoElement* ie) {
  const IEType *iet = ie->ietype();
  size_t ielen = ie->len();
  
  if (!iet->permitsLength(ielen)) {
    throw IETypeError("IE type size mismatch");
  }
  
  if (ielen > avail()) {
    return false;
  }
  
  memset(cur_, 0, ielen);
  cur_ += ielen;
  
  return true;
}

bool XCoder::encode(uint16_t val) {
  static const InfoElement u16ie("",0,0,IEType::unsigned16(),sizeof(uint16_t));
  return encode(reinterpret_cast<uint8_t *>(&val), sizeof(uint16_t), &u16ie);
}

bool XCoder::encode(uint32_t val) {
  static const InfoElement u32ie("",0,0,IEType::unsigned32(),sizeof(uint32_t));
  return encode(reinterpret_cast<uint8_t *>(&val), sizeof(uint32_t), &u32ie);
}

bool XCoder::encodeMessageStart() {
  if (msg_base_) {
    throw std::logic_error("out-of-sequence call to encodeMessageStart()");
  }

  if (kMessageHeaderLen > avail()) {
    return false;
  }
  msg_base_ = cur_;

  // skip it, we'll encode everything later
  return advance(kMessageHeaderLen);
}

void XCoder::encodeMessageEnd(uint32_t export_time, uint32_t sequence, uint32_t domain) {
  if (!msg_base_) {
    throw std::logic_error("out-of-sequence call to encodeMessageEnd()");
  }

  // grab length and save current
  size_t msg_len = cur_ - msg_base_;
  uint8_t *save = cur_;

  // encode message header
  cur_ = msg_base_;
  if (!encode(static_cast<uint16_t>(kIpfixVersion)) ||
      !encode(static_cast<uint16_t>(msg_len)) ||
      !encode(export_time) || !encode(sequence) || !encode(domain)) {
    throw std::logic_error("unexpected failure in encodeMessageEnd()");
  }

  // restore current pointer, mark no current message
  msg_base_ = NULL;
  cur_ = save;
}

bool XCoder::encodeSetStart(uint16_t set_id) {
  if (set_base_) {
    throw std::logic_error("out-of-sequence call to encodeSetStart()");
  }
  
  // precheck buffer for room for set header
  if (kSetHeaderLen > avail()) {
    return false;
  }
  set_base_ = cur_;
  
  // encode set ID, but skip length for later
  if (!encode(set_id) || !encode(static_cast<uint16_t>(0))) { 
    // we already checked for room, so we can only fail if the code is wrong.
    throw std::logic_error("unexpected failure in encodeSetStart()");
  }
  
  return true;
}

void XCoder::encodeSetEnd() {
  if (!set_base_) {
    throw std::logic_error("out-of-sequence call to encodeSetEnd()");
  }

  // grab length and save current
  size_t set_len = cur_ - set_base_;
  uint8_t *save = cur_;
  
  // encode set length
  cur_ = set_base_ + kSetLenOffset;
  if (!encode(static_cast<uint16_t>(set_len))) {
    throw std::logic_error("unexpected failure in encodeSetEnd()");
  }
  
  // restore current pointer, mark no current set
  set_base_ = NULL;
  cur_ = save;
}

bool XCoder::decode(uint8_t* val, size_t len, const InfoElement *ie) {
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
  if (!iet->permitsLength(ielen)) {
    throw IETypeError("IE type size mismatch");
  }
  
  if (iet->isEndian()) {
#if defined(BOOST_BIG_ENDIAN)
    (void)xcode_raw_right(cur_, ielen, val, len);
#elif defined(BOOST_LITTLE_ENDIAN)
    (void)xcode_raw_left(cur_, ielen, val, len);
    xcode_swap(val, len);
#else
#error libfc does not compile on weird-endian machines.
#endif
  } else {
    (void)xcode_raw_left(cur_, ielen, val, len);
  }
  
  cur_ += ielen;
  return true;
}

bool XCoder::decode(VarlenField *vf, const InfoElement *ie) {
  // FIXME need to write this
  // what does this need to do?
  // get the length of the information element from the IE
  // get the base pointer to the content
  // fill in the length and the base pointer to the content
  // advance the transcoder to the end of the content
  throw std::runtime_error("Varlen decode not implemented");
}

bool XCoder::decode(uint16_t& val) {
  static const InfoElement u16ie("",0,0,IEType::unsigned16(),sizeof(uint16_t));
  uint16_t deval;
  if (decode(reinterpret_cast<uint8_t *>(&deval), sizeof(uint16_t), &u16ie)) {
    val = deval;
    return true;
  } else {
    return false;
  }
}

bool XCoder::decode(uint32_t& val) {
  static const InfoElement u32ie("",0,0,IEType::unsigned32(),sizeof(uint32_t));
  uint32_t deval;
  if (decode(reinterpret_cast<uint8_t *>(&deval), sizeof(uint32_t), &u32ie)) {
    val = deval;
    return true;
  } else {
    return false;
  }
}


bool XCoder::decodeMessageHeader(uint16_t& len, 
                                 uint32_t& export_time, 
                                 uint32_t& sequence, 
                                 uint32_t& domain) {

  if (kMessageHeaderLen > avail()) {
    return false;
  }

  uint16_t version;
  if (!decode(version)) {
    // we already checked for bytes, so we can only fail if the code is wrong.
    throw std::logic_error("unexpected failure in decodeMessageHeader()");
  }
  if (version != kIpfixVersion) {
    throw FormatError("wrong IPFIX version; trying to decode non-IPFIX data?");
  }
  
  if (!decode(len)) {
    throw std::logic_error("unexpected failure in decodeMessageHeader()");
  }
  if (len < kMessageHeaderLen) {
    throw FormatError("nonsensical message length; trying to decode non-IPFIX data?");
  }
  
  if (!decode(export_time)) {
    throw std::logic_error("unexpected failure in decodeMessageHeader()");
  }
  if (!decode(sequence)) {
    throw std::logic_error("unexpected failure in decodeMessageHeader()");
  }
  if (!decode(domain)) {
    throw std::logic_error("unexpected failure in decodeMessageHeader()");
  }
  
  return true;
}

bool XCoder::decodeSetHeader(uint16_t& len, uint16_t& sid) {                                   

  if (kSetHeaderLen > avail()) {
    return false;
  }
  
  if (!decode(len)) {
    throw std::logic_error("unexpected failure in decodeMessageHeader()");
  }
  if (len < kSetHeaderLen || len > kMaxSetLen) {
    throw FormatError("nonsensical set length; trying to decode non-IPFIX data?");
  }
  
  if (!decode(sid)) {
    throw std::logic_error("unexpected failure in decodeMessageHeader()");
  }
  if (sid < kMinSetID && 
      sid != kTemplateSetID && 
      sid != kOptionTemplateSetID)
  {
    throw FormatError("nonsensical set ID; trying to decode non-IPFIX data?");
  }
  
  return true;
}

}