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

  typedef std::list<SetListEntry>::iterator       SetListIterator;

/**
 * Represents a deframed IPFIX Message received at a collector
 */

  class MBuf {
  public:
    MBuf():
      buf_(NULL), bufsz_(0) {}
      
    ~MBuf() { if (buf_) free(buf_); }
    
    const uint32_t domain() const { return domain_; } 
    const uint32_t sequence() const { return sequence_; } 
    const uint32_t export_time() const { return export_time_; } 
    
    template <typename T> bool deframe(T source, Session session);
    
    SetListIterator begin() {return setlist_.begin();}
    SetListIterator end() {return setlist_.end();}
    
    void transcodeBy(XCoder& xc) { xc.setBase(buf_, len_); }

  private:
    // make me uncopyable
    MBuf(MBuf& rhs) {};
    MBuf& operator=(MBuf& rhs) {};

    void ensure(size_t length);
    bool consume(int fd, size_t len, size_t off);
    bool consume(FILE *fp, size_t len, size_t off);
    
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