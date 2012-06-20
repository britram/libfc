#include <cassert>

#include "Exporter.h"
#include "Constants.h"

#include "exceptions/MTUError.h"

namespace IPFIX {

Exporter::Exporter(uint32_t domain, size_t mtu): 
  buf_(NULL),
  session_(),
  mtu_(mtu),
  domain_(domain),
  set_id_(0),
  msg_empty_(true),
  set_active_(false),
  tmpl_(NULL),
  rec_active_(false)
 {

  /* allocate buffer; will throw std::bad_alloc on failure */
  buf_ = new uint8_t[mtu_];
  
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
  assert (tid >= kMinSetID);

  // Make sure template is already active
  WireTemplate *new_tmpl = session_.getTemplate(domain_, tid);
  assert (new_tmpl->isActive());
  
  // Finalize existing set if necessary
  if (set_id_) endSet();

  // Set new template and set ID
  tmpl_ = new_tmpl;
  set_id_ = new_tmpl->tid();
  
  // create a new cursor
  ec_ = ExportCursor(tmpl_);
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
  
  if (!tmpl_->encodeStruct(xcoder_, struct_tmpl, struct_cp)) {
    // Not enough room to encode this record. Flush and try again.
    flush();
    ensureSet();
    if (!tmpl_->encodeStruct(xcoder_, struct_tmpl, struct_cp)) {
      // If it doesn't work now, it never will. Throw.
      throw MTUError("record");
    }
  }
  
  msg_empty_ = false;
}

void Exporter::beginRecord() {
    assert(!rec_active_);
    
    // Write a set header if we don't have one yet.
    ensureSet();
    
    // Start a new message if we don't have at least enough 
    // space for a record.
    if (xcoder_.avail() < tmpl_->minlen()) {
        flush();
        ensureSet();
        // Check again in case the MTU is simply too small
        if (xcoder_.avail() < tmpl_->minlen()) {
            throw MTUError("record");
        }
    }
    
    // Zero record
    xcoder_.encodeZeroAt(tmpl_->minlen(), 0);
    
    // Create new record state
    rec_active_ = true;
}

void Exporter::endRecord(bool do_export) {
    assert(rec_active_);
    
    // Advance cursor to end of record if exporting
    if (do_export) {
        xcoder_.advance(ec_.len());
    }
    
    // Clear record state
    ec_.clear();
    rec_active_ = false;
}

void Exporter::reserveLength(const InfoElement *ie, size_t len) {
    assert(rec_active_);
    
    // reserve in the present cursor
    ec_.setIELen(ie, len);
    
    // check for fit, start over if not
    if (!ec_.fitsIn(xcoder_.avail())) {
        rollbackRecord();
        flush();
        ensureSet();

        if (!ec_.fitsIn(xcoder_.avail())) {
            // we started over and it still doesn't fit. die.
            throw MTUError("value length reservation");
        }        
    }
}

bool Exporter::putValue(const InfoElement* ie, uint8_t* vp, size_t len) {
    // check for no current record
    if (!rec_active_) {
        throw std::logic_error("cannot put in inactive record");
    }
    
    // skip if the current template doesn't contain the value
    if (!tmpl_->contains(ie)) {
        return false;
    }
    
    // Figure out where to put the value.
    size_t off = ec_.offsetOf(ie);
    
    // Make sure the length is reserved in the export cursor
    // (throws if value put doesn't match preiously reserved value)
    ec_.setIELen(ie, len);
   
    // now encode
    size_t nextoff = xcoder_.encodeAt(vp, len, off, tmpl_->ieFor(ie));
    
    // And tell the export cursor if we have additional varlen encoding
    if (nextoff - off - len > 1) {
        ec_.addLen(nextoff - off - len - 1);
    }
    
    return true;
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
