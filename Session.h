/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * Defines Session, a container for per session state. 
 * It is used by Collector and Exporter, which expose 
 * any necessary functionality to client code. 
 *
 * Session is the owner for all WireTemplate instances in libfc; 
 * therefore, WireTemplates should only be handled by pointer as 
 * retrieved from Session::getTemplate() 
 */
#ifndef IPFIX_SESSION_H // idem
#define IPFIX_SESSION_H // hack

#include <map>
#include <list>
#include <tr1/memory>
#include <stdint.h>
#include "InfoModel.h"
#include "WireTemplate.h"

namespace IPFIX {

typedef std::map<WireTemplateKey, std::tr1::shared_ptr<WireTemplate> >::iterator    TibIter;

class Session {

public:
  Session() {}

  uint32_t nextSequence(uint32_t domain) { return next_seq_[domain]; }

  uint32_t incrementSequence(uint32_t domain, uint32_t increment);
  uint32_t checkSequence(uint32_t domain, uint32_t sequence);

  WireTemplate *getTemplate(uint32_t domain, uint16_t tid);
  void deleteTemplate(uint32_t domain, uint16_t tid);
  
  std::list<const WireTemplate *>activeTemplates(uint32_t domain);

  bool decodeTemplateRecord(Transcoder& xc, uint32_t domain);

private:
  std::map<uint32_t, uint32_t>                                    next_seq_;
  std::map<WireTemplateKey, std::tr1::shared_ptr<WireTemplate> >  tib_;
};

}

#endif
