#ifndef IPFIX_WIRETEMPLATE_H // idem
#define IPFIX_WIRETEMPLATE_H // hack

#include "IETemplate.h"
#include "StructTemplate.h"

namespace IPFIX {

class WireTemplate : public IETemplate {
  
public:

  /** 
   * Create a new WireTemplate. 
   *
   * Client code should use Session::getTemplate() instead, 
   * which automatically creates a new template when necessary.
   *
   * @param session Session which owns the template
   * @param domain observation domain ID in which the template lives
   * @param tid template identifier
   */
  WireTemplate(const Session* session, uint32_t domain, uint16_t tid):
    IETemplate(session, domain, tid) {}
    
  /**
   * Clear and deactivate this WireTemplate.
   *
   * FIXME should interact with template lifetime mechanism
   */
   void clear();
  
  /**
   * Clear this WireTemplate, replace its contents with those of
   * another given Template, and activate it.
   *
   * @param rhs template to mimic
   */
  void mimic(const IETemplate& rhs);

  /**
   * Add an InformationElement to this Template.
   *
   * @param ie pointer to Information Element to add; must be canonical
               (retrieved from InfoModel::instance())
   * @param offset unused, MUST be zero.
   */
  virtual void add(const InfoElement* ie, size_t offset = 0);

  /**
   * Use this Template to encode a structure described by a template to 
   * a given transcoder. Client code should use Exporter::exportRecord().
   *
   * @param encoder transcoder cued to the location to encode the record
   * @param struct_tmpl template describing the structure in struct_vp
   * @param struct_vp void pointer to struct to encode
   * @return true if encode succeeded, false if not enough space
   */
  bool encode(Transcoder& encoder, const StructTemplate &struct_tmpl, void *struct_vp) const;
  
  /**
   * Encode this Template as a template record to a given transcoder.
   * Used internally by Exporter.
   *
   * @param xcoder transcoder cued to the location to encode the template record
   * @return true if encode succeeded, false if not enough space
   */
  bool encodeTemplateRecord(Transcoder& xcoder) const;
  
  /**
   * Use this Template to decode a structure described by a template from 
   * a given transcoder. 
   *
   * Client code should use this call within SetReceiver::receive().
   *
   * @param encoder transcoder cued to the location of the record to decode
   * @param struct_tmpl template describing the structure in struct_vp
   * @param struct_vp void pointer to struct into which to decode
   * @return true if decode succeeded, false if not enough content
   */  
  bool decode(Transcoder& decoder, const StructTemplate &struct_tmpl, void *struct_vp) const;

  /* a template should be able to provide a transcode plan from another template. */
  // FIXME doesn't build
  // std::list<IPFIX::TCEntry> planTranscode(const IETemplate &struct_tmpl) const;
  // bool encode(Transcoder& encoder, const std::list<TCEntry>& plan, void *struct_vp) const;
  
private:
    WireTemplate():
    IETemplate() {}
  
    // FIXME make noncopyable
  
  };

} // namespace IPFIX 

#endif /* idem */