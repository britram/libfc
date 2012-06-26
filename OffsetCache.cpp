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

#include "OffsetCache.h"
namespace IPFIX {

    IETemplateIter OffsetCache::begin() {
         if (!viter_cached_) {
            IETemplateIter i;
            for (i = wt_->begin(); 
                 i != wt_->end() && (*i)->len() != kVarlen;
                 i++);
            viter_ = i;
            viter_cached_ = true;
        }
        return viter_;
    }

    IETemplateIter OffsetCache::end() {
        return wt_->end();
    }
    
    bool OffsetCache::advance() {
        if (!reclen_valid_) {
            recacheOffsets();
            if (!reclen_valid_) {
                throw CursorError("attempt to advance without valid record length");
            }
        }
        
        if (xc_->advance(reclen_)) {
            clear();
            return true;
        } else {
            return false;
        }
    }


    size_t OffsetCache::offsetOf(const InfoElement* ie) {
        assert(wt_);
    
        size_t off;
    
        // If offset available in template, return it.
        if ((off = wt_->offset(ie)) != kVarlen) {
            return off;
        }
    
        // If offset already stored, return it.
        if ((off = voffsets_[ie])) {
            return off;
        }
    
        // Nope, fill in offsets.
        recacheOffsets();

        // Now look again. If not there, we have to give up.
        if (!(off = voffsets_[ie])) {
            throw CursorError("offset cache miss: varlen not reserved?");
        }
    
        return off;
    }
    
    void CollectorOffsetCache::recacheOffsets() {
        size_t nextoff = wt_->maxFixedOffset();
        size_t vl;
        
        for (IETemplateIter i = begin(); 
                            i != end(); 
                            i++) {
            voffsets_[*i] = nextoff;
            if (!(nextoff = xc_->decodeVarlenLengthAt(nextoff, vl))) {
                // transcoder signalled unexpected end
                throw CursorError("end of buffer caching offsets: input data not IPFIX?");
            }
            vlengths_[*i] = vl;
        }
        // note that we have a real record length
        reclen_ = nextoff;
        reclen_valid_ = true;
    }

    void ExporterOffsetCache::recacheOffsets() {
        // note that the record length will be valid unless we
        // have to break out due to a missing length.
        reclen_valid_ = true;

        size_t nextoff = wt_->maxFixedOffset();

        for (IETemplateIter i = begin(); 
                            i != end(); 
                            i++) {
            voffsets_[*i] = nextoff;
            size_t len = (*i)->len();
            if (len == kVarlen) {
                if (vlengths_.find(*i) == vlengths_.end()) {
                    reclen_valid_ = false;
                    break;
                } else {
                    // account for vl encoding in offset calculation
                    len = vlengths_[*i] + (len < 255 ? 1 : 3);
                }
            }
            nextoff += len;
        }
        reclen_ = nextoff;
    }

    void ExporterOffsetCache::setIELen(const InfoElement *ie, size_t len) {
        assert(wt_);
    
        if (wt_->contains(ie) && wt_->ieFor(ie)->len() == kVarlen) {
            if (vlengths_[ie]) {
                assert(vlengths_[ie] == len);
            } else {
                vlengths_[ie] = len;
                reclen_ += len;
                reclen_valid_ = false;
            }
        }
    }
    
}
