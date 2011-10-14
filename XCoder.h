#ifndef IPFIX_XCODER_H // idem
#define IPFIX_XCODER_H // hack

#include <cstring>
#include <stdint.h>
#include "InfoElement.h"

namespace IPFIX {

/**
 * Use to represent variable-length IEs in structures represented by
 * VarlenTemplates. Content will be copied to message on encode. Pointer
 * will point inside the set buffer on decode, and must be copied or
 * processed before set processing is completed.
 */

struct VarlenField {
  // length of content
  size_t    len;
  // content pointer
  uint8_t*  cp;
};

class IOError : public std::runtime_error {
public:
  explicit IOError(const std::string& what_arg): 
    std::runtime_error(what_arg) {}
};

class FormatError : public std::runtime_error {
public:
  explicit FormatError(const std::string& what_arg): 
    std::runtime_error(what_arg) {}
};

class XCoder {
  
  public:
        
    XCoder():
      base_(NULL),
      cur_(NULL),
      check_(NULL),
      max_(NULL),
      msg_base_(NULL),
      set_base_(NULL)
    {}
    
    void zero() { memset(base_, 0, max_ - base_); }

    void setBase(uint8_t *buf, size_t sz) { 
      base_ = buf; 
      cur_ = buf; 
      max_ = buf + sz;
    }
    
    size_t len() const { return cur_ - base_; }
    size_t avail() const { return max_ - cur_; }

    const uint8_t* base() const { return base_; }
    const uint8_t* cur() const { return cur_; }
    
    void checkpoint() { check_ = cur_; }
    void rollback() { if (check_) cur_ = check_; }

    void subBuffer(size_t off, size_t len) {
      checkpoint();
      savemax_ = max_;
      if (max_ > cur_ + len) max_ = cur_ + len;
    }
    
    void desubBuffer() { 
      rollback();
      max_ = savemax_;
    }

    // Primitive for skipping forward in the buffer; also used for reading zeroes.
    bool advance(size_t size) { 
      if (size > avail()) return false; 
      cur_ += size; return true;
    }
    bool advance(const InfoElement *ie) { 
      return advance(ie->len()); 
    } 

    // Primitives for encoding
    bool encode(uint8_t* val, size_t len, const InfoElement* ie);
    bool encode(const VarlenField* vf, const InfoElement *ie);
    bool encode(uint16_t val);
    bool encode(uint32_t val);
    bool encodeZero(const InfoElement* ie);

    // Primitives for IPFIX message framing
    bool encodeMessageStart();
    void encodeMessageEnd(uint32_t export_time, uint32_t sequence, uint32_t domain);
    bool encodeSetStart(uint16_t set_id);
    void encodeSetEnd();

    // Primitives for decoding
    bool decode(uint8_t* val, size_t len, const InfoElement *ie);
    bool decode(VarlenField *vf, const InfoElement *ie);
    bool decode(uint16_t& val);
    bool decode(uint32_t& val);
    
    // Primitives for IPFIX message deframing
    bool decodeMessageHeader(uint16_t& len, 
                             uint32_t& export_time, 
                             uint32_t& sequence, 
                             uint32_t& domain);
    bool decodeSetHeader(uint16_t& len, uint16_t& sid);
      
  private:
    /* Buffer */
    uint8_t*    base_;    // buffer base pointer
    /* Cursors */
    uint8_t*    cur_;     // current read/write pointer
    uint8_t*    check_;   // checkpoint for rollback
    uint8_t*    max_;     // buffer end pointer
    uint8_t*    savemax_; // saved buffer end pointer for restriction
    /* Higher-level cursors */
    uint8_t*    msg_base_;  // currently open message header
    uint8_t*    set_base_;  // currently open set header
  };

} // namespace IPFIX

#endif // idem hack