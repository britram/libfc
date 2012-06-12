#include <cstdlib>

#include "Constants.h"
#include "MBuf.h"
#include "Transcoder.h"

#include "exceptions/IOError.h"

namespace IPFIX {

  void MBuf::clear() {
    delete[] buf_;
    buf_ = 0;
    bufsz_ = 0;
    len_ = 0;
    domain_ = 0;
    sequence_ = 0;
    export_time_ = 0;
    setlist_.clear();
  }

bool MBuf::consume(int fd, size_t len, size_t off) {

  ssize_t rc = read(fd, buf_ + off, len);

  if (rc == 0) {
    return false;
  } else if (rc == -1) {
    throw IOError(strerror(errno));
  } else if (static_cast<size_t>(rc) < len) {
    throw FormatError("Short read: end of stream in message body");    
  } else {
    return true;
  }
}
  
bool MBuf::consume(FILE *fp, size_t len, size_t off) {

  size_t rc = fread(buf_ + off, len, 1, fp);
  
  if (rc == 1) {
    return true;
  } else if (feof(fp)) {
    return false;
  } else {
    throw IOError(strerror(errno));
  }

}

bool MBuf::consume(std::istream& is, size_t len, size_t off) {

  is.read(reinterpret_cast<char*>(buf_ + off), len);

  if (static_cast<size_t>(is.gcount()) == len) {
    return true;
  } else if (is.eof()) {
      return false;
  } else {
      throw IOError("short read");   
  }

}


void MBuf::ensure(size_t length) {
  if (bufsz_ >= length) return;

  if (buf_) {
    uint8_t* new_buf = new uint8_t[length];
    memcpy(new_buf, buf_, bufsz_);
    memset(new_buf + bufsz_, '\0', length - bufsz_);
    delete[] buf_;
    buf_ = new_buf;
  } else {
    buf_ = new uint8_t[length];
    memset(buf_, '\0', length);
  }
  
  bufsz_ = length;
}

void MBuf::populateSetlist(Transcoder& xc, Session& session) {
  // Clear the set list
  setlist_.clear();

  while(xc.avail()) {
    SetListEntry sle;

    // read a set header into a set list entry
    sle.off = xc.len();
    xc.decodeSetHeader(sle.id, sle.len);

    // handle templates, skip future magic sets
    if (sle.id == kTemplateSetID) {
      // find end of set and complete this comment FIXME
      const uint8_t* xcend = xc.cur() + (sle.len - kSetHeaderLen);
  
      while ((xcend - xc.cur()) >= kTemplateHeaderLen) {
        if (!session.decodeTemplateRecord(xc, domain_)) {
          // FIXME this is a little weird, change this to follow
          // consistent error handling on decode, once we know what 
          // consistent means in this context. :)
          throw FormatError("Malformed template");
        }
      }
    
    } else if (sle.id >= kMinSetID) {
      // this is a data set
      setlist_.push_back(sle);
      xc.advance(sle.len - kSetHeaderLen);
    } else {
      // this is a new/illegal template set; just skip it and keep going
      std::cerr << "skipping unsupported set ID " << sle.id << std::endl;
      xc.advance(sle.len - kSetHeaderLen);
    } 
  }
}

}
