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
 
#include "Collector.h"

namespace IPFIX {

typedef std::map<const IETemplate*, SetReceiver* >::const_iterator ReceiverListIter;
typedef std::map<WireTemplateKey, SetReceiver* >::const_iterator ReceiverCacheIter;    
  
SetReceiver* Collector::receiverForTemplate(const WireTemplate* wt) {
    WireTemplateKey wtk = wt->key();

    // std::cerr << "looking for receiver for template:" << std::endl;
    // wt->dump(std::cerr);

  // first look in the receiver cache
    ReceiverCacheIter rciter = receiver_cache_.find(wtk);
    if (rciter != receiver_cache_.end()) {
        // std::cerr << "   found in cache." << std::endl;
        return rciter->second;
    }

  // nope, look in the receiver list
    for (ReceiverListIter rliter = receivers_.begin();
    rliter != receivers_.end();
    rliter++) {
        if (wt->containsAll(rliter->first)) {
            receiver_cache_[wtk] = rliter->second;
            // std::cerr << "   found in reciever list." << std::endl;
            return rliter->second;
        }
    }

  // If we're here, we missed completely
    // std::cerr << "    not found" << std::endl;
    return NULL;
}

bool Collector::receiveMessage(MBuf& mbuf) {
  std::shared_ptr<Session> session;
  
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
    receiver->receiveSet(this, xc, set_tmpl);
    // and defocus
    xc.defocus();
  }
  
  return true;
}

void Collector::registerReceiver(const IETemplate* mintmpl, 
                                 SetReceiver* receiver) {
  receivers_[mintmpl] = receiver;
  // std::cerr << "Registered receiver " << reinterpret_cast<uint64_t>(receiver) << " for template:" << std::endl;
  // mintmpl->dump(std::cerr);
}

} /* namespace IPFIX */
