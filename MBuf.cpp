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
  
  bool MBuf::consume(const uint8_t* buf, size_t len, size_t off) {
    memcpy(buf_ + off, buf, len);
    return true;
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
