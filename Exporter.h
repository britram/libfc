#ifndef IPFIX_EXPORTER_H // idem
#define IPFIX_EXPORTER_H // hack

#include <ctime>
#include <stdexcept>
#include "Session.h"
#include "Xcoder.h"

namespace IPFIX {

class MTUError : public std::runtime_error {
public:
  explicit MTUError(const std::string& what_arg): 
    std::runtime_error(what_arg) {}
};
  
class Exporter {
  
public:

  Exporter(const InfoModel* model, uint32_t domain, size_t mtu);
  virtual ~Exporter();
  
  void setDomain(uint32_t domain);
  void setTemplate(uint16_t tid);
  IETemplate *getTemplate(uint16_t tid) { 
    return session_.getTemplate(domain_, tid); 
  }
  
  void exportTemplatesForDomain();
  
  void exportRecord(const StructTemplate &struct_tmpl, void *struct_vp);
  void flush(time_t export_time);
  void flush() { flush(time(NULL)); }

protected:
  virtual void sendMessage(uint8_t *base, size_t len) = 0;

private:
  // make me uncopyable
  Exporter();
  Exporter(Exporter& rhs);
  Exporter& operator=(Exporter& rhs);

  void endSet() { xcoder_.encodeSetEnd(); set_active_ = false; }
  void ensureSet();
  void ensureTemplateSet();
  void startMessage();
  void endMessage(time_t export_time);
  
  
  uint8_t*                  buf_;
  XCoder                    xcoder_;
  uint16_t                  set_id_;
  bool                      msg_empty_;
  bool                      set_active_;
  Session                   session_;
  WireTemplate*             tmpl_;
  uint32_t                  domain_;
  size_t                    mtu_;
};

}

#endif