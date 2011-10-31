#include "Collector.h"

namespace IPFIX {

typedef std::map<std::tr1::shared_ptr<const IETemplate>, std::tr1::shared_ptr<SetReceiver> >::const_iterator ReceiverListIter;
typedef std::map<WireTemplateKey, std::tr1::shared_ptr<SetReceiver> >::const_iterator ReceiverCacheIter;    
  
const std::tr1::shared_ptr<SetReceiver> Collector::receiverForTemplate(const WireTemplate* wt) {
  WireTemplateKey wtk = wt->key();
  
  // first look in the receiver cache
  ReceiverCacheIter rciter = receiver_cache_.find(wtk);
  if (rciter != receiver_cache_.end()) {
    return rciter->second;
  }
  
  // nope, look in the receiver list
  for (ReceiverListIter rliter = receivers_.begin();
                        rliter != receivers_.end();
                        rliter++) {
    if (wt->containsAll(rliter->first.get())) {
      receiver_cache_[wtk] = rliter->second;
      return rliter->second;
    }
  }
  
  // If we're here, we missed completely
  return std::tr1::shared_ptr<SetReceiver>();
}
  
bool Collector::receiveMessage(MBuf& mbuf) {
  std::tr1::shared_ptr<Session> session;
  
  // get the next message and session key from the derived class
  if (!this->_receiveMessage(mbuf, session)) return false;
  
  std::tr1::shared_ptr<SetReceiver> receiver;
  Transcoder xc;
  mbuf.transcodeBy(xc);
  
  // Iterate over sets
  for (SetListIter sliter = mbuf.begin(); sliter != mbuf.end(); sliter++) {
    const WireTemplate* set_tmpl = 
          session->getTemplate(mbuf.domain(), sliter->id);

    if (!set_tmpl->isActive()) {
      // Inactive set template means that the template hasn't been 
      // received yet. Skip. We might want to count these later.
      break;
    }
    
    receiver = receiverForTemplate(set_tmpl);
    if (receiver.get() == NULL) {
      // No receiver, none registered. Skip it.
      break;
    }
    
    // restrict transcoder to the set content
    xc.focus(sliter->off + kSetHeaderLen, sliter->len - kSetHeaderLen);
    // handle set
    receiver->receive(this, xc, set_tmpl);
    // and defocus
    xc.defocus();
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
    ssp = std::tr1::shared_ptr<Session>(new Session());
    sessions_[sk] = ssp;
  }
  return ssp;
}

void Collector::endSession(int sk) {
  sessions_.erase(sk);
}

} /* namespace IPFIX */