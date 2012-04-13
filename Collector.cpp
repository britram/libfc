#include "Collector.h"

namespace IPFIX {

typedef std::map<const IETemplate*, SetReceiver* >::const_iterator ReceiverListIter;
typedef std::map<WireTemplateKey, SetReceiver* >::const_iterator ReceiverCacheIter;    
  
SetReceiver* Collector::receiverForTemplate(const WireTemplate* wt) {
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
    if (wt->containsAll(rliter->first)) {
      receiver_cache_[wtk] = rliter->second;
      return rliter->second;
    }
  }
  
  // If we're here, we missed completely
  return NULL;
}
  
bool Collector::receiveMessage(MBuf& mbuf) {
  std::tr1::shared_ptr<Session> session;
  
  // get the next message and session from the derived class
  if (!this->_receiveMessage(mbuf, session)) return false;
  
  SetReceiver* receiver;
  Transcoder xc;
  mbuf.transcodeBy(xc);
  
  // Iterate over sets
  for (SetListIter sliter = mbuf.begin(); sliter != mbuf.end(); sliter++) {
    const WireTemplate* set_tmpl = 
          session->getTemplate(mbuf.domain(), sliter->id);

    if (!set_tmpl->isActive()) {
      // Inactive set template means that the template hasn't been 
      // received yet. Skip. We might want to count these later.
      std::cerr << "No template for set " << sliter->id << "; skipping." << std::endl;
      break;
    }
    
    receiver = receiverForTemplate(set_tmpl);
    if (receiver == NULL) {
      // No receiver, none registered. Skip it.
      std::cerr << "No receiver for set " << sliter->id << "; skipping." << std::endl;
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

void Collector::registerReceiver(const IETemplate* mintmpl, 
                                 SetReceiver* receiver) {
  receivers_[mintmpl] = receiver;
  std::cerr << "Registered a receiver for template:" << std::endl;
  mintmpl->dump(std::cerr);
}

} /* namespace IPFIX */