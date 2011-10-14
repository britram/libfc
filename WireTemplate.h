#ifndef IPFIX_WIRETEMPLATE_H // idem
#define IPFIX_WIRETEMPLATE_H // hack

#include "IETemplate.h"
#include "StructTemplate.h"

namespace IPFIX {

typedef std::tr1::shared_ptr<class WireTemplate> WireTemplate_SP;

class WireTemplate : public IETemplate {
  
public:

  /** 
   * Create a new WireTemplate.
   * Client code should use Session.getTemplate() instead, 
   * which automatically creates a new template when necessary.
   */
  WireTemplate(const Session* session, uint32_t domain, uint16_t tid):
    IETemplate(session, domain, tid) {}
    
  // FIXME need a way to promote any IETemplate to a WireTemplate

  /* a template should be able to provide a transcode plan from another template. */
  // FIXME doesn't build
  // std::list<IPFIX::TCEntry> planTranscode(const IETemplate &struct_tmpl) const;
  // bool encode(XCoder& encoder, const std::list<TCEntry>& plan, void *struct_vp) const;

  void clear();
  
  void mimic(const IETemplate& rhs);

  virtual void add(const InfoElement* ie, size_t offset = 0);
  
  bool encode(XCoder& encoder, const StructTemplate &struct_tmpl, void *struct_vp) const;
  
  bool encodeTemplateRecord(XCoder& xcoder) const;
  
  bool decode(XCoder& decoder, const StructTemplate &struct_tmpl, void *struct_vp) const;

private:
    WireTemplate():
    IETemplate() {}
  
  };

} // namespace IPFIX 

#endif /* idem */