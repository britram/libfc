#ifndef IPFIX_STRUCTTEMPLATE_H // idem
#define IPFIX_STRUCTTEMPLATE_H // hack

#include "IETemplate.h"

namespace IPFIX {

class StructTemplate : public IETemplate {
  
public:
  
  StructTemplate():
    IETemplate() {}

  virtual void add(const InfoElement* ie, size_t offset = 0) = 0;

  };

} // namespace IPFIX

#endif /* idem */