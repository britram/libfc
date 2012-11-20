/* Hi Emacs, please use -*- mode: C++; -*- */
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
#include <array>

#include <climits>
#include <ctime>

#include <unistd.h>

#include "PlacementExporter.h"

namespace IPFIX {

  PlacementExporter::PlacementExporter(ExportDestination& _os,
                                       uint32_t _observation_domain)
    : os(_os), current_template(0), sequence_number(0),
      observation_domain(_observation_domain), 
      n_message_octets(kMessageHeaderLen) {
  }

  PlacementExporter::~PlacementExporter() {
    flush();
  }

  static void encode16(uint16_t val, uint8_t** buf, const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint16_t) <= buf_end);
    *(*buf)++ = (val >> 8) & 0xff;
    *(*buf)++ = (val >> 0) & 0xff;
    assert(*buf <= buf_end);
  }

  static void encode32(uint32_t val, uint8_t** buf, const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint32_t) <= buf_end);
    *(*buf)++ = (val >> 24) & 0xff;
    *(*buf)++ = (val >> 16) & 0xff;
    *(*buf)++ = (val >>  8) & 0xff;
    *(*buf)++ = (val >>  0) & 0xff;
    assert(*buf <= buf_end);
  }

  bool PlacementExporter::flush() {
    uint8_t message_header[kMessageHeaderLen];
    const uint8_t* message_end = message_header + kMessageHeaderLen;
    uint8_t* p = message_header;

    time_t now = time(0);
    if (now == static_cast<time_t>(-1))
      return false;

    encode16(kIpfixVersion, &p, message_end);
    encode16(static_cast<uint16_t>(n_message_octets), &p, message_end);
    encode32(static_cast<uint32_t>(now), &p, message_end);
    encode32(sequence_number++, &p, message_end);
    encode32(observation_domain, &p, message_end);

    return os.flush();
  }

  void PlacementExporter::place_values(const PlacementTemplate* tmpl) {
    if (tmpl != current_template) {
      if (used_templates.find(tmpl) == used_templates.end()) {
        used_templates.insert(tmpl);
        templates_in_this_message.insert(tmpl);
      }
      current_template = tmpl;
    }
  }


} // namespace IPFIX
