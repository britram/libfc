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
#include <climits>
#include <ctime>

#include <unistd.h>

#include "PlacementExporter.h"

namespace IPFIX {

  PlacementExporter::PlacementExporter(ExportDestination& _os,
                                       uint32_t _observation_domain)
    : os(_os),
      current_template(0),
      sequence_number(0),
      observation_domain(_observation_domain), 
      n_message_octets(kMessageHeaderLen),
      template_set_index(-1) {
    /* Push an empty iovec into the iovec vector, to be filled later
     * with the message header by flush(). */
    iovecs.at(0) = ::iovec();
  }

  PlacementExporter::~PlacementExporter() {
    flush();
  }

  static void encode16(uint16_t val, uint8_t** buf,
                       const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint16_t) <= buf_end);
    *(*buf)++ = (val >> 8) & 0xff;
    *(*buf)++ = (val >> 0) & 0xff;
    assert(*buf <= buf_end);
  }

  static void encode32(uint32_t val, uint8_t** buf,
                       const uint8_t* buf_end) {
    assert(*buf < buf_end);
    assert(*buf + sizeof(uint32_t) <= buf_end);
    *(*buf)++ = (val >> 24) & 0xff;
    *(*buf)++ = (val >> 16) & 0xff;
    *(*buf)++ = (val >>  8) & 0xff;
    *(*buf)++ = (val >>  0) & 0xff;
    assert(*buf <= buf_end);
  }

  bool PlacementExporter::flush() {
    /** This message header.
     *
     * This variable is dynamically allocated so as to facilitate
     * its deletion later as part of the iovecs vector. */
    uint8_t* message_header = new uint8_t[kMessageHeaderLen];

    /** Points to the end of this message.
     *
     * Used for range checks. */
    const uint8_t* message_end = message_header + kMessageHeaderLen;

    /** Moves through the message header. */
    uint8_t* p = message_header;

    /** Return value. */
    int ret = 0;

    /* Only write something if we have anything nontrivial to write. */
    if (n_message_octets > kMessageHeaderLen) {
      time_t now = time(0);
      if (now == static_cast<time_t>(-1))
        return false;
      
      encode16(kIpfixVersion, &p, message_end);
      encode16(static_cast<uint16_t>(n_message_octets), &p, message_end);
      encode32(static_cast<uint32_t>(now), &p, message_end);
      encode32(sequence_number++, &p, message_end);
      encode32(observation_domain, &p, message_end);
      
      ::iovec& header = iovecs.at(0);
      
      header.iov_base = message_header;
      header.iov_len = kMessageHeaderLen;
      
      ret = os.writev(iovecs);
      
      for (auto i = iovecs.begin(); i != iovecs.end(); ++i)
        delete[] static_cast<uint8_t*>(i->iov_base);
      iovecs.clear();

      /* Space for next message header. */
      iovecs.at(0) = ::iovec();

      /* On connectionless transports, we forget all templates and
       * reissue them---if needed---in a new message. */
      if (os.is_connectionless())
        used_templates.clear();
      template_set_index = -1;
      
      n_message_octets = kMessageHeaderLen;
    }
    return ret;
  }

  void PlacementExporter::place_values(const PlacementTemplate* tmpl) {
    /** The number of bytes added to the current message as a result
     * of issuing this new data record.  It might be as small as the
     * number of bytes in the representation of this data record, and
     * it might be as large as that number, plus the size of a new
     * template set containing the wire template for the template
     * used. */
    size_t new_bytes; // = tmpl.data_record_size();

    /** Will contain tmpl if this template is hitherto unknown. */
    const PlacementTemplate* unknown_template = 0;

    if (tmpl != current_template) {
      /* We need to insert a new template and start a new data set if
       *
       *  - the underlying transport is connection-oriented and we
       *    have never seen the template; or
       *  - the underlying transport is connectionless and we haven't
       *    seen the template in this message so far.
       */
      if (used_templates.find(tmpl) == used_templates.end()) {
        unknown_template = tmpl;

        /* Need to add a new template to the template record section */
        size_t template_bytes = 0;
        //tmpl->wire_template(new_template_id(), 0, &template_bytes);
        new_bytes += template_bytes;

        /* Need to create template set? */
        if (template_set_index == -1)
          new_bytes += kSetHeaderLen;
      }
      /* Need to open a new data set. */
      new_bytes += kSetHeaderLen;

      /* Switch to another template for this new data set. */
      current_template = tmpl;
    }

    if (n_message_octets + new_bytes > kMaxMessageLen) {
      flush();
      n_message_octets += new_bytes;
      unknown_template = tmpl;
    } else {
      // FIXME: Assemble data record and add to current_set
    }

    if (template_set_index == -1) {
      template_set_index = iovecs.size();
      iovecs.at(template_set_index) = ::iovec();
    }

    if (unknown_template != 0)
      used_templates.insert(unknown_template);
  }

} // namespace IPFIX
