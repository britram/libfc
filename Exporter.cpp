#include "Exporter.h"
#include "Constants.h"

namespace IPFIX {

Exporter::Exporter(uint32_t domain, size_t mtu): 
  buf_(NULL),
  set_id_(0),
  msg_empty_(true),
  set_active_(false),
  session_(),
  tmpl_(NULL),
  domain_(domain),
  mtu_(mtu) {

  /* allocate buffer */
  if (!(buf_ = reinterpret_cast<uint8_t *>(malloc(mtu)))) throw std::bad_alloc();
  
  std::cerr << "allocated buffer " << reinterpret_cast<unsigned long> (buf_) << " length " << mtu << std::endl;
	  
  /* start a message */
  startMessage();
}

Exporter::~Exporter() {
  std::cerr << "freed buffer " << reinterpret_cast<unsigned long> (buf_) << std::endl;
  free(buf_);
}

void Exporter::setDomain(uint32_t domain)
{
  // Flush message if necessary
  if (domain_ != domain && !msg_empty_) {
    flush();
  }
  
  domain_ = domain;
}

void Exporter::setTemplate(uint16_t tid)
{
  // short circuit on already set
  if (set_id_ == tid) {
    return;
  }

  // die on bad template ID
  if (tid < kMinSetID) {
    throw std::logic_error("Cannot set template with reserved ID");
  }

  // Make sure template is already active
  WireTemplate *new_tmpl = session_.getTemplate(domain_, tid);
  if (!new_tmpl->isActive()) {
    throw std::logic_error("Cannot set inactive template");
  }
  
  // Finalize existing set if necessary
  if (set_id_) endSet();

  // Set new template and set ID
  tmpl_ = new_tmpl;
  set_id_ = new_tmpl->tid();
}

void Exporter::exportTemplatesForDomain() {
  ensureTemplateSet();

  std::list<const WireTemplate *> tmpl_list = session_.activeTemplates(domain_);  
  for (std::list<const WireTemplate *>::iterator iter = tmpl_list.begin();
                                               iter != tmpl_list.end();
                                               iter++) {
    if (!(*iter)->encodeTemplateRecord(xcoder_)) {
      flush();
      ensureSet();
      if (!(*iter)->encodeTemplateRecord(xcoder_)) {
        // If it doesn't work now, it never will. Throw.
        throw MTUError("MTU too small for template record");
      }
    }
  }
  
  msg_empty_ = false;
}

void Exporter::exportRecord(const StructTemplate& struct_tmpl, uint8_t* struct_cp) {
  ensureSet();
  
  if (!tmpl_->encode(xcoder_, struct_tmpl, struct_cp)) {
    // Not enough room to encode this record. Flush and try again.
    flush();
    ensureSet();
    if (!tmpl_->encode(xcoder_, struct_tmpl, struct_cp)) {
      // If it doesn't work now, it never will. Throw.
      throw MTUError("MTU too small for record");
    }
  }
  
  msg_empty_ = false;
}


void Exporter::startMessage() {
  xcoder_.setBase(buf_,mtu_);
  if (!xcoder_.encodeMessageStart()) {
    throw MTUError("MTU too small for message");
  }
  msg_empty_ = true;
}

void Exporter::ensureSet() {
  if (!set_active_) {
    if (!xcoder_.encodeSetStart(set_id_)) {
      flush();
      if (!xcoder_.encodeSetStart(set_id_)) {
        throw MTUError("MTU too small for initial set header");
      }
    }
    set_active_ = true;
  }
}

void Exporter::ensureTemplateSet() {
  if (set_id_ != kTemplateSetID) {
    if (set_active_) {
      endSet();
    }
    tmpl_ = NULL;
    set_id_ = kTemplateSetID;
  }
  
  ensureSet();
}

void Exporter::flush(time_t export_time) {
  std::cerr << "flush(" << export_time << ")" << std::endl;
  
  if (set_active_) {
    endSet();
  }
  
  if (xcoder_.len() > kMessageHeaderLen) {
    xcoder_.encodeMessageEnd(export_time, 0, domain_);
    _sendMessage(buf_, xcoder_.len());
    startMessage();
  }
}

}