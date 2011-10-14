#include "Collector.h"

namespace IPFIX {
    
bool Collector::receiveMessage(MBuf& mbuf) {
  Session_SP session;
  
  // get the next message and session key from the derived class
  if (!this->_receiveMessage(mbuf, session)) return false;
  
  SetReceiver_SP receiver;
  XCoder xc;
  mbuf.transcodeBy(xc);
  
  // Iterate over sets
  for (SetListIterator sliter = mbuf.begin(); sliter != mbuf.end(); sliter++) {
    const WireTemplate* set_tmpl = 
          session->getTemplate(mbuf.domain(), sliter->id);
    if (!set_tmpl->active()) {
      // Inactive set template means that the template hasn't been 
      // read in yet. Skip. We might want to count these later.
      break;
    }
    
    // Look for a receiver for this set in the receiver cache 
    IETemplateKey tk(mbuf.domain(), sliter->id);
    
    ReceiverCacheIterator rciter = receiver_cache_.find(tk);
    if (rciter != receiver_cache_.end()) {
      // In cache, take the first one
      receiver = rciter->second;
    } else {
      for (ReceiverListIterator rliter = receivers_.begin();
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
      // restrict transcoder to the set and skip the header
      xc.subBuffer(sliter->off, sliter->len);
      xc.advance(kSetHeaderLen);
      // handle set
      receiver->receive(this, xc, set_tmpl);
    }
  }
  
  return true;
}

void Collector::registerReceiver(IETemplate* mintmpl, SetReceiver* receiver) {
  receivers_[IETemplate_SP(mintmpl)] = SetReceiver_SP(receiver);
}
    
Session_SP Collector::getSession(int sk) {
  Session_SP ssp = sessions_[sk];
  if (!ssp.get()) {
    ssp = Session_SP(new Session(model_));
    sessions_[sk] = ssp;
  }
  return ssp;
}

void Collector::endSession(int sk) {
  sessions_.erase(sk);
}

} /* namespace IPFIX */