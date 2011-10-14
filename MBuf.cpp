#include "MBuf.h"
#include "XCoder.h"
#include "Constants.h"

#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace IPFIX {

bool MBuf::consume(int fd, size_t len, size_t off) {

  int rc = read(fd, buf_ + off, len);

  if (rc == 0) {
    return false;
  } else if (rc == -1) {
    throw IOError(strerror(errno));
  } else if (rc < len) {
    throw FormatError("Short read: end of stream in message body");    
  } else {
    return true;
  }
}
  
bool MBuf::consume(FILE *fp, size_t len, size_t off) {

  int rc = fread(buf_ + off, len, 1, fp);
  
  if (rc == 1) {
    return true;
  } else if (feof(fp)) {
    return false;
  } else {
    throw IOError(strerror(errno));
  }

}

template <typename T>
bool MBuf::deframe(T source, Session session) {
  XCoder xc;
  
  // Get the message header from the source
  ensure(kMessageHeaderLen);
  if (!consume(source, kMessageHeaderLen, 0)) {
    // No message header available. Assume EOF or equivalent
    return false;
  }
  
  // Decode it
  xc.setBase(buf_, kMessageHeaderLen);
  // safe - we made the buffer
  (void)xc.decodeMessageHeader(len_, export_time_, sequence_, domain_);

  // Consume the rest of the message
  ensure(len_);
  if (!consume(source, len_ - kMessageHeaderLen, kMessageHeaderLen)) {
    // Read a header, but no message. Broken. Die.
    throw FormatError("End of stream after message header");
  }

  // Clear the set list
  setlist_.empty();

  // Deframe it
  xc.setBase(buf_, len_);
  xc.advance(kMessageHeaderLen);

  while(xc.avail()) {
    SetListEntry sle;
    
    // read a set header into a set list entry
    sle.off = xc.len() - kSetHeaderLen;
    xc.decodeSetHeader(sle.id, sle.len);
    
    // handle templates, skip future magic sets
    if (sle.id == kTemplateSetID) {
      // find end of set an
      const uint8_t* xcend = xc.cur() + (sle.len - kSetHeaderLen);
      
      while ((xcend - xc.cur()) >= kTemplateHeaderLen) {
        if (!session.decodeTemplateRecord(xc, domain_)) {
          // FIXME this is a little weird, change this to follow
          // consistent error handling on decode, once we know what 
          // consistent means in this context. :)
          throw FormatError("Malformed template");
        }
      }
        
    } else if (sle.id < kMinSetID) {
      // FIXME log skip
      std::cerr << "skipping unsupported set ID" << sle.id << std::endl;
    } else {
      setlist_.push_back(sle);
      xc.advance(sle.len - kSetHeaderLen);
    }
  }
}

void MBuf::ensure(size_t length) {
  if (bufsz_ > length) return;
  
  if (buf_) {
    buf_ = reinterpret_cast<uint8_t *>(realloc(buf_, length));
  } else {
    buf_ = reinterpret_cast<uint8_t *>(malloc(length));
  }
  
  if (buf_) {
    bufsz_ = length;
  } else {
    throw std::bad_alloc();
  }
}

}