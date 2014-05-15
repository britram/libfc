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

#ifndef _LIBFC_INPUTSOURCE_H_
#  define _LIBFC_INPUTSOURCE_H_

#  include <cstdint>

#  include <unistd.h>

namespace LIBFC {

  /** Abstract base class for IPFIX input sources.
   *
   * This class defines the interface that all IPFIX input sources
   * must implement.
   */
  class InputSource {
  public:
    /** Reads a number of bytes from the input source into a buffer.
     *
     * @param buf the buffer in which to put the bytes
     * @param len the number of bytes to read
     *
     * @return the number of bytes read (0 indicates end of file), or
     *     -1 on error.
     */
    virtual ssize_t read(uint8_t* buf, uint16_t len) = 0;

    /** Attempts to re-synchronise the stream to the beginning of a valid
     * message.
     *
     * This method has no meaning in message-based input sources, such
     * as UDP. In such cases, the method should simply throw away any
     * buffers that it might have and receive the next message.
     *
     * @return true if resynchronisation worked, false if the stream
     * has ended before resynchronisation.
     */
    virtual bool resync() = 0;

    /** Returns the offset of the current message.
     *
     * For stream-based input sources like files or TCP streams, this
     * is the offset (in bytes) into the stream for the current
     * message.  For message-based input sources, like UDP, this is
     * zero.
     *
     * @return the message offset
     */
    virtual size_t get_message_offset() const = 0;

    /** Advances the offset.
     *
     * For message-based input sources, this member function is a
     * no-op.  For stream-based input sources, this increments the
     * current offset by the total number of bytes read since the last
     * increment.
     */
    virtual void advance_message_offset() = 0;

    /** Returns a name for this input source.
     *
     * In case of a file input source, the best name is of course the
     * file name.  A TCP or UDP source should perhaps return the
     * IP address and port number of the local and/or remote port.
     *
     * @return a name for this input source.
     */
    virtual const char* get_name() const = 0;

    /** Returns whether this input source supports peek().
     *
     * @return true if this input source supports peek(), false if not.
     */
    virtual bool can_peek() const = 0;
  };

} // namespace LIBFC

#endif // _LIBFC_INPUTSOURCE_H_
