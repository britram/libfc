#include "Session.h"

#include "exceptions/IEUnknownError.h"

namespace IPFIX {

uint32_t Session::incrementSequence(uint32_t domain, uint32_t increment) {
  // FIXME actually do this -- this is the export interface
  return 0;
}

uint32_t Session::checkSequence(uint32_t domain, uint32_t sequence) {
  // FIXME actually do this -- this is the collector verifier interface
  return 0;
}

WireTemplate* Session::getTemplate(uint32_t domain, uint16_t tid) {
  //std::cerr << "session getTemplate(" << domain << "," << tid << ")" << std::endl;

  if (tid < kMinSetID) {
    throw std::logic_error("template ID reserved");
  }
  
  WireTemplateKey tk(domain, tid);
  std::shared_ptr<WireTemplate> tsp = tib_[tk];
  if (!tsp.get()) {
    //std::cerr << "    miss, create new template" << std::endl;
    tsp = std::shared_ptr<WireTemplate>(new WireTemplate(domain, tid));
    tib_[tk] = tsp;
  }
  return tsp.get();
}

void Session::deleteTemplate(uint32_t domain, uint16_t tid) {
  WireTemplateKey tk(domain, tid);
  tib_.erase(tk);
}

std::list<const WireTemplate *> Session::activeTemplates(uint32_t domain) {
  std::list<const WireTemplate *> out;
  
  for (TibIter iter = tib_.begin();
                   iter != tib_.end();
                   iter++) {
    if (domain == 0 || (*iter).first.first == domain) {
      out.push_back((*iter).second.get());
    }
  }
  
  return out;
}

bool Session::decodeTemplateRecord(Transcoder &xc, uint32_t domain) {
  
  // Ensure template record header is available
  if (xc.avail() < kTemplateHeaderLen) {
    return false;
  }
  
  uint16_t tid = 0, iecount = 0, ienum = 0, ielen = 0;
  uint32_t iepen = 0;
  
  // decode template record header
  (void) xc.decode(tid);
  (void) xc.decode(iecount);

  // short-circuit on template withdrawal
  if (iecount == 0) {
    deleteTemplate(domain, tid);
    return true;
  }
  
  // get the template for this record
  WireTemplate *tmpl = getTemplate(domain, tid);
  
  // FIXME need to handle template withdrawals
  // for now, just overwrite (this is too permissive)
  if (tmpl->isActive()) {
    tmpl->clear();
  }
  
  // iterate over template record fields
  for (int i = 0; i < iecount; i++) {
    
    // decode field
    if (!xc.decode(ienum)) return false;
    if (!xc.decode(ielen)) return false;
    if (ienum & kEnterpriseBit) {
      if (!xc.decode(iepen)) return false;
    } else {
      iepen = 0;
    }
    
    // add IE to template 
    const InfoElement *ie = InfoModel::instance().lookupIE(iepen, ienum, ielen);
    if (ie == NULL) {
        // Add IE to infomodel if we don't know about it yet
        InfoModel::instance().add_unknown(iepen, ienum, ielen);
        ie = InfoModel::instance().lookupIE(iepen, ienum, ielen);
        if (ie == NULL) {
            throw IEUnknownError(iepen, ienum, ielen);
        }
    }
    tmpl->add(ie);
  }

  // activate the template so we can use it
  tmpl->activate();
  
  return true;
}


}
