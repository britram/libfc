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

#include <algorithm>
#include <cerrno>
#include <cstring>

#include "BufWriter.h"

#include "exceptions/IOError.h"

namespace IPFIX {

  BufWriter::BufWriter(uint32_t domain, size_t mtu) 
    : Exporter(domain, mtu),
      _buf(0),
      _cur(0),
      _len(0),
      _capacity(0) {}
  
  const unsigned char* BufWriter::buf() const {
    return this->_buf;
  }

  size_t BufWriter::len() const {
    return this->_len;
  }

  void BufWriter::_sendMessage(uint8_t *base, size_t len) {
    if (_len + len > _capacity) {
      size_t new_capacity = std::max(2*len, _len + len);
      unsigned char* new_buf = new unsigned char[new_capacity];
      memcpy(new_buf, _buf, _len);
      delete[] _buf;
      _buf = new_buf;
      _cur = _buf + _len;
      _capacity = new_capacity;
    }
    
    memcpy(_cur, base, _len);
    _cur += len;
    _len += len;
  }

  BufWriter::~BufWriter() {
    delete[] _buf;
  }

}
