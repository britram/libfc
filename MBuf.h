#ifndef IPFIX_MBUF_H // idem
#define IPFIX_MBUF_H // hack

#include "Session.h"

#include <stdint.h>
#include <memory>
#include <cstdio>
#include <list>

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
 */

  class MBuf {
  public:
    MBuf():
      buf_(NULL), bufsz_(0) {}
      
    ~MBuf() { if (buf_) free(buf_); }
    
    /** Get the domain of the last read message */
    const uint32_t domain() const { return domain_; } 

    /** Get the sequence number of the last read message */
    const uint32_t sequence() const { return sequence_; } 

    /** Get the export time of the last read message */
    const uint32_t export_time() const { return export_time_; } 
    
    
    /** 
     * Given a source containing a message and a session for state management,
     * deframe a message into this message buffer, replacing its previous 
     * contents.
     *
     * @param source source to read from
     * @param session session to store state in
     */
    // g++ isn't smart enough to link this unless it's in the damn header.
    template <typename T> bool deframe(T source, Session& session) {
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
    
    /**
     * Return an iterator for the start of the set list
     */
    SetListIter begin() {return setlist_.begin();}

    /**
     * Return an iterator for the end of the set list
     */
    SetListIter end() {return setlist_.end();}
    
    /**
     * Use a specified transcoder to transcode this message buffer
     *
     * FIXME determine whether this is really the right interface.
     */
    
    void transcodeBy(Transcoder& xc) { xc.setBase(buf_, len_); }

  private:
    // make me uncopyable
    MBuf(MBuf& rhs);
    MBuf& operator=(MBuf& rhs);

    void ensure(size_t length);
    
    /**
     * Low-level interface to consume bytes from a source into the buffer
     * into a specified offset in the buffer; works with file descriptors
     */
    bool consume(int fd, size_t len, size_t off);

    /**
     * Low-level interface to consume bytes from a source into the buffer
     * into a specified offset in the buffer; works with file pointers
     */
    bool consume(FILE *fp, size_t len, size_t off);
    
    /**
     * Given a transcoder positioned after the message header, replace the
     * contents of this buffer's setlist with the sets contained in the message.
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