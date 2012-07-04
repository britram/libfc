/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of ETH Zürich nor the names of other contributors 
 *      may be used to endorse or promote products derived from this software 
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <cassert>

#include "Exporter.h"
#include "Constants.h"

#include "exceptions/MTUError.h"

namespace IPFIX {

Exporter::Exporter(uint32_t domain, size_t mtu): 
  buf_(NULL),
  mtu_(mtu),
  //  session_(),
  domain_(domain),
  set_id_(0),
  msg_empty_(true),
  set_active_(false),
  tmpl_(NULL),
  rec_active_(false),
  oc_(NULL, &xcoder_)
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
  oc_ = ExporterOffsetCache(tmpl_, &xcoder_);
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
    
//    std::cerr << "----- begin record" << std::endl;
}

void Exporter::endRecord(bool do_export) {
    assert(rec_active_);
    
    // Advance cursor to end of record if exporting
    if (do_export) {
        oc_.advance();
    } else {
        oc_.clear();
    }
    
    // Clear record state
    rec_active_ = false;

//    std::cerr << "----- end record" << std::endl;
}

void Exporter::reserveVarlen(const InfoElement *ie, size_t len) {
    assert(rec_active_);
    
    // reserve in the present cursor
    oc_.setIELen(ie, len);
    
    // check for fit, start over if not
    if (!oc_.mightFit()) {
        rollbackRecord();
        flush();
        beginRecord();
        
        if (!oc_.mightFit()) {
            // we started over and it still doesn't fit. die.
            throw MTUError("value length reservation");
        }
    }
}

bool Exporter::putValue(const InfoElement* ie, const void* vp, size_t len) {
    // check for no current record
    if (!rec_active_) {
        throw std::logic_error("cannot put in inactive record");
    }
    
    // if the current template doesn't contain the value,
    // just pretend we put it.
    if (!tmpl_->contains(ie)) {
        return true;
    }
    
    // Figure out where to put the value.
    size_t off = oc_.offsetOf(ie);

    // now encode, catch error (could include overrun)
    if (!xcoder_.encodeAt(vp, len, off, tmpl_->ieFor(ie))) {
        rollbackRecord();
        flush();

        return false;
    }

    // Make sure the length is reserved
    oc_.setIELen(ie, len);

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
