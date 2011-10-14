#ifndef IPFIX_SESSION_H // idem
#define IPFIX_SESSION_H // hack

#include <map>
#include <list>
#include <tr1/memory>
#include <stdint.h>
#include "InfoModel.h"
#include "WireTemplate.h"

namespace IPFIX {

typedef std::tr1::shared_ptr<class Session>                   Session_SP;

typedef std::map<IETemplateKey, WireTemplate_SP>::iterator    TibIterator;

class Session {
public:
  Session(const InfoModel *model):
   model_(model) 
  {}

  const InfoModel *model() const { return model_; }
  uint32_t nextSequence(uint32_t domain) { return next_seq_[domain]; }

  uint32_t incrementSequence(uint32_t domain, uint32_t increment);
  uint32_t checkSequence(uint32_t domain, uint32_t sequence);

  WireTemplate *getTemplate(uint32_t domain, uint16_t tid);
  void deleteTemplate(uint32_t domain, uint16_t tid);
  
  std::list<const WireTemplate *>activeTemplates(uint32_t domain);

  bool decodeTemplateRecord(XCoder& xc, uint32_t domain);

private:
  const InfoModel*                                          model_;
  std::map<uint32_t, uint32_t>                              next_seq_;
  std::map<IETemplateKey, WireTemplate_SP>                  tib_;
};

}

#endif
