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

/**
 * @file
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */

#ifndef IPFIX_MBUF_H // idem
#define IPFIX_MBUF_H // hack

#include <cstdint>
#include <cstdio>
#include <list>
#include <memory>

#include "Session.h"

#include "exceptions/FormatError.h"

namespace IPFIX {

  struct SetListEntry {
    uint16_t    id;
    uint16_t    len;
    size_t      off;
  };

  typedef std::list<SetListEntry>::iterator       SetListIter;

/**
 * A message buffer contains a deframed IPFIX message at a receiver.
 * This class is used internally by Collector.
 *
 * FIXME genericize this to handle V9.
 *
 * FIXME genericize this to an interface for handling message export too
 *       (take some message/set framing out of Transcoder and put it here)
 */

  class MBuf {
  public:
    MBuf():
    buf_(NULL), bufsz_(0), len_(0), domain_(0), sequence_(0), export_time_(0) {}
      
    ~MBuf() { 
      delete[] buf_; /* Works correctly if buf_ == NULL */
    }

    /** Clears this MBuf so it's as good as new. */
    void clear();

    /** Gets the domain of the last read message.
     *
     * @return the domain of the most recent message.
     */
    const uint32_t domain() const { return domain_; } 

    /** Gets the sequence number of the last read message. 
     *
     * @return the sequence number of the most recent message.
     */
    const uint32_t sequence() const { return sequence_; } 

    /** Gets the export time of the last read message.
     *
     * @return the export time of the most recent message.
     */
    const uint32_t export_time() const { return export_time_; } 
    
    /** Deframes a message into this message buffer, replacing its
     * previous contents.
     *
     * @param source source to read from. This can be either a file
     *     descriptor (an int), a file pointer, a std::istream, or a
     *     buffer (a uint8_t*)
     * @param session session to store state in
     *
     * @return true if deframing was successful, false if not.
     */
    // g++ isn't smart enough to link this unless it's in the damn header.
    // (Not a g++ problem actually -- templates need to be
    // instantiated and can't be compiled in a straightforward way. --
    // Stephan) 
    template <typename T> bool deframe(T& source, Session& session) {
      Transcoder xc;
  
      // Get the message header from the source
      ensure(kMessageHeaderLen);
      if (!consume(source, kMessageHeaderLen, 0)) {
        // No message header available. Assume EOF or equivalent
        return false;
      }
  
      // Decode it
      xc.setBase(buf_, kMessageHeaderLen); // FIXME are you sure?
      // safe - we made the buffer
      (void)xc.decodeMessageHeader(len_, export_time_, sequence_, domain_);

      // Consume the rest of the message
      ensure(len_);
      if (!consume(source, len_ - kMessageHeaderLen, kMessageHeaderLen)) {
        // Read a header, but no message. Broken. Die.
        throw FormatError("End of stream after message header");
      }
      
      // Prepare transcoder
      xc.setBase(buf_, len_);
      xc.advance(kMessageHeaderLen);
      
      // Deframe the message
      populateSetlist(xc, session);

      // and we're ready
      return true;
    }
    
    /** Returns an iterator for the start of the set list.
     *
     * @return an iterator for the beginning set list.
     */
    SetListIter begin() {return setlist_.begin();}

    /** Return an iterator for the end of the set list.
     *
     * @return an iterator for the end of the set list.
     */
    SetListIter end() {return setlist_.end();}
    
    /** Specifies a transcoder for this message buffer.
     *
     * @param xc transcoder for this message buffer.
     */
    void transcodeBy(Transcoder& xc) { xc.setBase(buf_, len_); }

  private:
    // make me uncopyable
    MBuf(MBuf& rhs) = delete;
    MBuf& operator=(MBuf& rhs) = delete;

    void ensure(size_t length);
    
    /** Consumes bytes from a source into the buffer at a specific
     * offset (file descriptor version).
     *
     * @param fd the file descriptor from which to read bytes
     * @param len the number of bytes to read
     * @param off the offset into this MBuf where to put the read
     * bytes
     *
     * @return true if the read succeeded, false otherwise
     */
    bool consume(int fd, size_t len, size_t off);

    /** Consumes bytes from a source into the buffer at a specific
     * offset (file pointer version).
     *
     * @param fp the file pointer from which to read bytes
     * @param len the number of bytes to read
     * @param off the offset into this MBuf where to put the read
     * bytes
     *
     * @return true if the read succeeded, false otherwise
     */
    bool consume(FILE *fp, size_t len, size_t off);

    /** Consumes bytes from a source into the buffer at a specific
     * offset (istream version).
     *
     * @param is the input stream from which to read bytes
     * @param len the number of bytes to read
     * @param off the offset into this MBuf where to put the read
     * bytes
     *
     * @return true if the read succeeded, false otherwise
     */
    bool consume(std::istream& is, size_t len, size_t off);

    /** Consumes bytes from a source into the buffer at a specific
     * offset (buffer version).
     *
     * @param buf the buffer from which to read bytes
     * @param len the number of bytes to read
     * @param off the offset into this MBuf where to put the read
     * bytes
     *
     * @return true if the read succeeded, false otherwise
     */
    bool consume(const uint8_t* buf, size_t len, size_t off);

    /** Replaces the contents of this buffer's setlist with the sets
     * contained in the message.
     *
     * @param xc transcoder positioned after the message header
     * @param session ???
     */
    void populateSetlist(Transcoder& xc, Session& session);
    
    uint8_t                                   *buf_;
    size_t                                    bufsz_;
    uint16_t                                  len_;
    uint32_t                                  domain_;
    uint32_t                                  sequence_;
    uint32_t                                  export_time_;
    std::list<SetListEntry>                   setlist_;
  };

} // namespace IPFIX

#endif // idem hack
