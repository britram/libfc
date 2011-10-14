#ifndef IPFIX_COLLECTOR_H // idem
#define IPFIX_COLLECTOR_H // hack

#include "Session.h"
#include "MBuf.h"
#include "IETemplate.h"
#include "SetReceiver.h"

namespace IPFIX {

  typedef std::map<IETemplate_SP, SetReceiver_SP>::const_iterator ReceiverListIterator;
  typedef std::map<IETemplateKey, SetReceiver_SP>::const_iterator ReceiverCacheIterator;
  
  class Collector {
    
  public:

    Collector(const InfoModel* model) : model_(model) {}
    virtual ~Collector() {}
    
    // register a set receiver for an exemplar template
    void registerReceiver(IETemplate* mintmpl, SetReceiver* receiver);
        
    // process the next message from this collector
    // this is the entry point to the collector runloop, the collector must 
    // check for new peers here, and so on. returns true if a message was
    // collected, false if none was available
    bool receiveMessage(MBuf& mbuf);
    
    // receive a message with a throwaway message buffer
    bool receiveMessage() {
      MBuf transient_mbuf;
      return this->receiveMessage(transient_mbuf);
    }
    
  protected:
    
    /**
     * Receive and deframe the next message into the given message buffer;
     * return the session on which it was received by reference.
     *
     * @param mbuf buffer to deframe message into
     * @param session (returned by reference) shared pointer to session 
                      from which message was received
     * @return true if message available, false otherwise
     */
    virtual bool _receiveMessage(MBuf& mbuf, Session_SP& session) = 0;
    
    Session_SP getSession(const int sessionKey);
    void endSession(const int sessionKey);
    
  private:
    
    const InfoModel*                                        model_;
    MBuf                                                    buf_;
    std::map<int, Session_SP>                               sessions_;
    std::map<IETemplate_SP, SetReceiver_SP>                 receivers_;
    std::map<IETemplateKey, SetReceiver_SP>                 receiver_cache_;
  };
  
} // namespace IPFIX

#endif // idem hack
