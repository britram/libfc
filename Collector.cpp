#include "Collector.h"

namespace IPFIX {
    
bool Collector::receiveMessage(MBuf& mbuf) {
  std::tr1::shared_ptr<Session> session;
  
  // get the next message and session key from the derived class
  if (!this->_receiveMessage(mbuf, session)) return false;
  
  std::tr1::shared_ptr<SetReceiver> receiver;
  XCoder xc;
  mbuf.transcodeBy(xc);
  
  // Iterate over sets
  for (SetListIter sliter = mbuf.begin(); sliter != mbuf.end(); sliter++) {
    const WireTemplate* set_tmpl = 
          session->getTemplate(mbuf.domain(), sliter->id);
    if (!set_tmpl->active()) {
      // Inactive set template means that the template hasn't been 
      // read in yet. Skip. We might want to count these later.
      break;
    }
    
    // Look for a receiver for this set in the receiver cache 
    IETemplateKey tk(mbuf.domain(), sliter->id);
    
    ReceiverCacheIter rciter = receiver_cache_.find(tk);
    if (rciter != receiver_cache_.end()) {
      // In cache, take the first one
      receiver = rciter->second;
    } else {
      for (ReceiverListIter rliter = receivers_.begin();
                                rliter != receivers_.end();
                                rliter++) {
        // Search the receivers
        if (set_tmpl->containsAll(rliter->first.get())) {
          // Found one. Stick it in the cache
          // FIXME how to invalidate this cache?
          receiver = rliter->second;
          receiver_cache_[tk] = receiver;
          break;
        }
      }
    }
    
    if (receiver.get() == NULL) {
      // No receiver, none registered. Skip it.
      break;
    } else {
      // restrict transcoder to the set content
      xc.focus(sliter->off + kSetHeaderLen, sliter->len - kSetHeaderLen);
      // handle set
      receiver->receive(this, xc, set_tmpl);
      // and defocus
      xc.defocus();
    }
  }
  
  return true;
}

void Collector::registerReceiver(const IETemplate* mintmpl, SetReceiver* receiver) {
  // FIXME we really want to make a copy here, no?
  receivers_[std::tr1::shared_ptr<const IETemplate>(mintmpl)] = std::tr1::shared_ptr<SetReceiver>(receiver);
}
    
std::tr1::shared_ptr<Session> Collector::getSession(int sk) {
  std::tr1::shared_ptr<Session> ssp = sessions_[sk];
  if (!ssp.get()) {
    ssp = std::tr1::shared_ptr<Session>(new Session(model_));
    sessions_[sk] = ssp;
  }
  return ssp;
}

void Collector::endSession(int sk) {
  sessions_.erase(sk);
}

} /* namespace IPFIX */