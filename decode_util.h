/* Hi Emacs, please use -*- mode: C++; -*- */
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

/**
 * @file
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */

#ifndef IPFIX_DECODE_UTIL_H
#  define IPFIX_DECODE_UTIL_H

#  include <cstdint>

namespace IPFIX {
    /** Decodes a 16-bit value from a buffer.
   *
   * Values are transported on the wire in network byte order (=
   * big-endian).  This function converts two adjacent bytes to a
   * 16-bit number in host byte order.
   *
   * It is the responsibility of the caller to make sure that buf
   * contains at least two more octets.  It would be tempting to use
   * functions like ntohs(), but we can't do that since we don't know
   * if the buffer is properly aligned.
   *
   * @param buf the buffer from which to decode the octets
   *
   * @return the decoded 16-bit value
   */
  extern uint16_t decode_uint16(const uint8_t* buf);

  /** Decodes a 32-bit value from a buffer.
   *
   * Values are transported on the wire in network byte order (=
   * big-endian).  This function converts four adjacent bytes to a
   * 32-bit number in host byte order.
   *
   * It is the responsibility of the caller to make sure that buf
   * contains at least four more octets.  It would be tempting to use
   * functions like ntohs(), but we can't do that since we don't know
   * if the buffer is properly aligned.
   *
   * @param buf the buffer from which to decode the octets
   *
   * @return the decoded 32-bit value
   */
  extern uint32_t decode_uint32(const uint8_t* buf);

} // namespace IPFIX

#endif /* IPFIX_DECODE_UTIL_H */