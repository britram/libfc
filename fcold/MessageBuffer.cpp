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
#include <cassert>
#include <cstring>
#include <sstream>

#include "MessageBuffer.h"

namespace FColD {

    MessageBuffer::MessageBuffer()
        : buf(new uint8_t[kMBufSz]),
          buf_sz(kMBufSz),
          buf_len(0),
          buf_owned(true),
          stream_pos(0),
          recv_ms(0),
          msgclk_ms(0),
          name(nullptr),
          off(0) {}
    
    MessageBuffer::MessageBuffer(size_t sz)
        : buf(new uint8_t[sz]),
          buf_sz(sz),
          buf_len(0),
          buf_owned(true),
          stream_pos(0),
          recv_ms(0),
          msgclk_ms(0),
          name(nullptr),
          off(0) {}
    
    MessageBuffer(uint8_t *ibuf, size_t sz, bool own, bool copy)
          : buf(ibuf),
            buf_sz(sz),
            buf_len(sz),
            buf_owned(own||copy),
            stream_pos(0),
            recv_ms(0),
            msgclk_ms(0),
            name(nullptr),
            off(0)
    {
        if (copy) {
            buf = new uint8_t[sz]
            memcpy(buf, ibuf, sz)
        }
    }

  MessageBuffer::~MessageBuffer() {
      if (buf_owned) {
          delete[] buf;
      }
      if (name) {
          delete[] const_cast<char*>(name);
      }
  }

  ssize_t MessageBuffer::read(uint8_t* result_buf, uint16_t result_len) {
    ssize_t ret = peek(result_buf, result_len);

    if (ret >= 0) {
      off += ret;
    }

    return ret;
  }

  ssize_t MessageBuffer::peek(uint8_t* result_buf, uint16_t result_len) {
    assert(off <= len);

    size_t bytes_to_copy = off + result_len > len ? len - off : result_len;
    
    /* This assert is to make sure that bytes_to_copy can fit into a
     * 16-bit unsigned integer. The reasoning is as follows.  If off +
     * result_len > len, then bytes_to_copy is len - off, which is
     * less than result_len (a 16-bit unsigned integer), but greater
     * than or equal to zero (since len >= off).  Otherwise, it's
     * result_len, a 16-bit unsigned integer. */
    assert(bytes_to_copy <= UINT16_MAX);

    memcpy(result_buf, buf + off, bytes_to_copy);

    return static_cast<ssize_t>(bytes_to_copy);
  }

  bool MessageBuffer::resync() {
    // TODO
    return true;
  }

  size_t MessageBuffer::get_message_offset() const {
      return 0;
  }

  void MessageBuffer::advance_message_offset() {}

  const char* MessageBuffer::get_name() const {
    if (name == 0) {
      std::ostringstream sstr;

      sstr << "Buffer(address=0x" << static_cast<const void*>(buf) 
	   << ",length=" << len << ')';
      std::string s = sstr.str();

      name = new char[s.length() + 1];
      std::strcpy(const_cast<char*>(name), s.c_str());
    }
    
    return name;
  }

  bool MessageBuffer::can_peek() const {
    return true;
  }

} // namespace LIBFC
