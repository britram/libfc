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

#include <unistd.h>

#include "WandioInputSource.h"

namespace libfc {

  WandioInputSource::WandioInputSource(io_t* io, std::string name)
    : io(io),
      message_offset(0),
      current_offset(0),
      name(name),
      io_belongs_to_me(false) {
  }

  WandioInputSource::WandioInputSource(std::string name)
    : io(0),
      message_offset(0),
      current_offset(0),
      name(name),
      io_belongs_to_me(true) {
    io = wandio_create(name.c_str());
  }

  WandioInputSource::~WandioInputSource() {
    /* Do not destroy io if it doesn't belong to me! */
    if (io_belongs_to_me)
      wandio_destroy(io);
  }

  ssize_t WandioInputSource::read(uint8_t* buf, uint16_t len) {
    off_t ret = wandio_read(io, buf, len);
    if (ret > 0)
      current_offset += ret;
    return static_cast<ssize_t>(ret);
  }

  ssize_t WandioInputSource::peek(uint8_t* buf, uint16_t len) {
    off_t ret = wandio_peek(io, buf, len);
    return static_cast<ssize_t>(ret);
  }

  bool WandioInputSource::resync() {
    // TODO
    return true;
  }

  size_t WandioInputSource::get_message_offset() const {
    return message_offset;
  }

  void WandioInputSource::advance_message_offset() {
    message_offset += current_offset;
    current_offset = 0;
  }

  const char* WandioInputSource::get_name() const {
    return name.c_str();
  }

  bool WandioInputSource::can_peek() const {
    return true;
  }

} // namespace libfc
