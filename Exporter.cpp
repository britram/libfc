#include "Exporter.h"
#include "Constants.h"

#include "exceptions/MTUError.h"

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

  /* allocate buffer; will throw std::bad_alloc on failure */
  buf_ = new uint8_t[mtu];
  
  //std::cerr << "allocated buffer " << static_cast<void*> (buf_) << " length " << mtu << std::endl;
	  
  /* start a message */
  startMessage();
}

Exporter::~Exporter() {
  //std::cerr << "freed buffer " << static_cast<void*> (buf_) << std::endl;
  delete buf_;
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
    throw std::logic_error("template ID is reserved");
  }

  // Make sure template is already active
  WireTemplate *new_tmpl = session_.getTemplate(domain_, tid);
  if (!new_tmpl->isActive()) {
    throw std::logic_error("template is inactive");
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
  for (auto iter = tmpl_list.begin(); iter != tmpl_list.end(); iter++) {
    if (!(*iter)->encodeTemplateRecord(xcoder_)) {
      flush();
      ensureSet();
      if (!(*iter)->encodeTemplateRecord(xcoder_)) {
        // If it doesn't work now, it never will. Throw.
        throw MTUError("template record");
      }
    }
  }
  
  msg_empty_ = false;
}

void Exporter::exportStruct(const StructTemplate& struct_tmpl, uint8_t* struct_cp) {
  ensureSet();
  
  if (!tmpl_->encode(xcoder_, struct_tmpl, struct_cp)) {
    // Not enough room to encode this record. Flush and try again.
    flush();
    ensureSet();
    if (!tmpl_->encode(xcoder_, struct_tmpl, struct_cp)) {
      // If it doesn't work now, it never will. Throw.
      throw MTUError("record");
    }
  }
  
  msg_empty_ = false;
}

void Exporter::beginRecord() {

// bool WireTemplate::encode(Transcoder& xc, const StructTemplate& struct_tmpl, uint8_t* struct_cp) const
// {  
//   // Refuse to encode an with inactive template
//   if (!active_) {
//     throw TemplateInactiveError("encode");
//   }
//   
//   // Refuse to encode unless we have at _least_ minimum length avail.
//   if (xc.avail() < minlen_) {
//     //std::cerr << "encoder overrun (" << xc.avail() << " avail, " << minlen_ << " required.)" << std::endl;
//     return false;
//   }
//   
//   // Note: Void casts here are safe, since we checked minlen. 
//   //       When we move to varlen encoding, we'll need to checkpoint and rollback.
//   // FIXME this should be sped up by specifically building a transcode plan.
//   xc.checkpoint();
//   for (IETemplateIter iter = ies_.begin();
//        iter != ies_.end();
//        iter++) {
//     if (struct_tmpl.contains(*iter)) {
//       size_t off = struct_tmpl.offset(*iter);
//       size_t len = struct_tmpl.length(*iter);
//       if (len == kVarlen) {
//         VarlenField *vf = reinterpret_cast<VarlenField *>(struct_cp + off);
//         if (!xc.encode(vf, *iter)) goto err;
//       } else {
//         if (!xc.encode(struct_cp + off, len, *iter)) goto err;
//       }
//     } else {
//       if (!xc.encodeZero(*iter)) goto err;
//     }
//   }
//   
//   return true;
// 
// err:
//   xc.rollback();
//   return false;
// }

}

void Exporter::exportRecord() {

}

void Exporter::rollbackRecord() {

}

void Exporter::startMessage() {
  xcoder_.setBase(buf_,mtu_);
  if (!xcoder_.encodeMessageStart()) {
    throw MTUError("message");
  }
  msg_empty_ = true;
}

void Exporter::ensureSet() {
  if (!set_active_) {
    if (!xcoder_.encodeSetStart(set_id_)) {
      flush();
      if (!xcoder_.encodeSetStart(set_id_)) {
        throw MTUError("initial set header");
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
  //std::cerr << "flush(" << export_time << ")" << std::endl;
  
  if (set_active_) {
    endSet();
  }
  
  if (xcoder_.len() > kMessageHeaderLen) {
    xcoder_.encodeMessageEnd(export_time, 0, domain_);
    _sendMessage(buf_, xcoder_.len());
    startMessage();
  }
}

} // namespace IPFIX
