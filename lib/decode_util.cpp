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
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "decode_util.h"

#include "exceptions/FormatError.h"

namespace LIBFC {
  uint16_t decode_uint16(const uint8_t* buf) {
    return (static_cast<uint16_t>(buf[0]) << 8) 
      | (static_cast<uint16_t>(buf[1]) << 0);
  }

  uint32_t decode_uint32(const uint8_t* buf) {
    return (static_cast<uint32_t>(buf[0]) << 24) 
      | (static_cast<uint32_t>(buf[1]) << 16)
      | (static_cast<uint32_t>(buf[2]) <<  8) 
      | (static_cast<uint32_t>(buf[3]) <<  0);
  }

  void report_error(const std::string message, ...) {
    static const size_t buf_size = 10240;
    static char buf[buf_size];
    va_list args;
  
    va_start(args, message);
    int nchars = vsnprintf(buf, buf_size, message.c_str(), args);
    va_end(args);

    if (nchars < 0)
      strcpy(buf, "Error while formatting error message");
    else if (static_cast<unsigned int>(nchars) > buf_size - 1 - 3) {
      buf[buf_size - 4] = '.';
      buf[buf_size - 3] = '.';
      buf[buf_size - 2] = '.';
      buf[buf_size - 1] = '\0';   // Shouldn't be necessary
    }

    throw LIBFC::FormatError(buf);
  }
} // namespace LIBFC
