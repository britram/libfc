/* Copyright (c) 2011, NEC Europe Ltd, Consorzio Nazionale 
 * Interuniversitario per le Telecomunicazioni, Institut 
 * Telecom/Telecom Bretagne, ETH Zürich, INVEA-TECH a.s. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of NEC Europe Ltd, Consorzio Nazionale 
 *      Interuniversitario per le Telecomunicazioni, Institut Telecom/Telecom 
 *      Bretagne, ETH Zürich, INVEA-TECH a.s. nor the names of its contributors 
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
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#include <cassert>

#include <arpa/inet.h>

#include "WireTemplate.h"

namespace IPFIX {

    void WireTemplate::add(const InfoElement* ie) {  
        add_inner(ie);

        // calculate offset
        size_t offset;
        if (offsets_.empty()) {
            offset = 0;
        } else if (offsets_.back() == kVarlen ||
                  ie->len() == kVarlen) {
            offset = kVarlen;
        } else {
            offset = offsets_.back() + ies_.back()->len();
        }

        // Add offset to offset table
        offsets_.push_back(offset);

        // Track maximum constant offset
        if (offset != kVarlen) {
            maxoff_ = offset;
        } 

        // Add the length of the IE to the minimum length
        if (ie->len() == kVarlen) {
            minlen_ += 1;
            varlen_count_ += 1;
        } else {
            minlen_ += ie->len();
        }

        // Add the length of the IE's fields to the template record length
        trlen_ += ie->pen() ? 8 : 4;
    }

void WireTemplate::clear() {
  ies_.clear();
  offsets_.clear();
  index_map_.clear();
  minlen_ = 0;
  trlen_ = 0;
  active_ = false;
}

void WireTemplate::mimic(const IETemplate& rhs) {
    clear();
    for (IETemplateIter i = rhs.begin(); i != rhs.end(); ++i) {
        add(*i);
    }
    activate();
}

bool WireTemplate::encodeStruct(Transcoder& xc, const StructTemplate& struct_tmpl, uint8_t* struct_cp) const
{  
  // Refuse to encode an with inactive template
  assert(active_);
  
  // Refuse to encode unless we have at _least_ minimum length avail.
  if (xc.avail() < minlen_) {
    //std::cerr << "encoder overrun (" << xc.avail() << " avail, " << minlen_ << " required.)" << std::endl;
    return false;
  }
  
  // Note: Void casts here are safe, since we checked minlen. 
  //       When we move to varlen encoding, we'll need to checkpoint and rollback.
  // FIXME this should be sped up by specifically building a transcode plan.
  xc.checkpoint();
  for (IETemplateIter iter = ies_.begin();
       iter != ies_.end();
       iter++) {
    if (struct_tmpl.contains(*iter)) {
      size_t off = struct_tmpl.offset(*iter);
      size_t len = struct_tmpl.length(*iter);
      if (len == kVarlen) {
        VarlenField *vf = reinterpret_cast<VarlenField *>(struct_cp + off);
        if (!xc.encode(vf, *iter)) goto err;
      } else {
        if (!xc.encode(struct_cp + off, len, *iter)) goto err;
      }
    } else {
      if (!xc.encodeZero(*iter)) goto err;
    }
  }
  
  return true;

err:
  xc.rollback();
  return false;
}


bool WireTemplate::encodeTemplateRecord(Transcoder &xc) const {

  // Refuse to encode an inactive template
  assert(active_);

  // Make sure the encoder has space for the encoded version of the template
  // Void casts from here are safe, since the template record length is 
  // constant.
  if (xc.avail() < trlen_) { 
    return false;
  }
  
  // encode template record header
  (void) xc.encode(tid_);
  (void) xc.encode(static_cast<uint16_t>(ies_.size()));
  
  // encode fields
  for (IETemplateIter iter = ies_.begin();
       iter != ies_.end();
       iter++) {
    if ((*iter)->pen()) {
      (void) xc.encode(static_cast<uint16_t>((*iter)->number() | kEnterpriseBit));
      (void) xc.encode((*iter)->len());
      (void) xc.encode((*iter)->pen());
    } else {
      (void) xc.encode((*iter)->number());
      (void) xc.encode((*iter)->len());
    }
  }
  
  return true;
}

bool WireTemplate::decodeStruct(Transcoder& xc, const StructTemplate &struct_tmpl, uint8_t* struct_cp) const {  
  // Refuse to decode an with inactive template
  assert(active_);
  
  // Refuse to decode unless we have at _least_ minimum length avail.
  if (xc.avail() < minlen_) {
    //std::cerr << "decoder overrun (" << xc.avail() << " avail, " << minlen_ << " required.)" << std::endl;
    return false;
  }
  
  // FIXME speed this up with a transcode plan

  xc.checkpoint();
  for (IETemplateIter iter = ies_.begin();
       iter != ies_.end();
       iter++) {
    if (struct_tmpl.contains(*iter)) {
      size_t off = struct_tmpl.offset(*iter);
      size_t len = struct_tmpl.length(*iter);
      if (len == kVarlen) {
        VarlenField *vf = reinterpret_cast<VarlenField *>(struct_cp + off);
        if (!xc.decode(vf, *iter)) goto err;
      } else {
        //fprintf(stderr, "dec %-60s at 0x%016lx to [%u:%u] at 0x%016lx\n", 
        //    (*iter)->toIESpec().c_str(), reinterpret_cast<long>(xc.cur()), 
        //    off, off + len, reinterpret_cast<long>(struct_cp));
        if (!xc.decode(struct_cp + off, len, *iter)) goto err;
      }
    } else {
        // not in struct template, need to skip this IE
        xc.decodeSkip(*iter);
    }
  }
  return true;

err:
  xc.rollback();
  return false;
}

}
