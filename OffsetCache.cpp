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


#include "OffsetCache.h"
namespace IPFIX {

     OffsetCache::OffsetCache(const WireTemplate* wt, Transcoder* xc):
        wt_(wt),
        xc_(xc),
        reclen_(0)
        {
            if (wt_) {
                if (wt_->varlenCount()) {
                    IETemplateIter i;
                    for (i = wt_->begin(); 
                         i != wt_->end() && (*i)->len() != kVarlen;
                         i++);
                    viter_ = i;
                } else {
                    reclen_ = wt_->minlen();
                } 
            }
        }


    IETemplateIter OffsetCache::varlenBegin() {
        return viter_;
    }

    IETemplateIter OffsetCache::varlenEnd() {
        return wt_->end();
    }
    
    void OffsetCache::advance() {
        if (!reclen()) {
                throw CursorError("attempt to advance without valid record length");
        }
        
        if (xc_->advance(reclen())) {
//            std::cerr << "advanced " << reclen_ << " octets" << std::endl;
            clear();
        } else {
            throw CursorError("internal error: record too big");
        }
    }


    size_t OffsetCache::offsetOf(const InfoElement* ie) {
        assert(wt_);
    
        size_t off;
    
        // If offset available in template, return it.
        if ((off = wt_->offset(ie)) != kVarlen) {
//            std::cerr << "offset of " << ie->name() << " found in template: " << off << std::endl;
            return off;
        }
    
        // If offset already stored, return it.
        if ((off = voffsets_[ie])) {
//            std::cerr << "offset of " << ie->name() << " found in cache:    " << off << std::endl;
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

    void OffsetCache::dump(std::ostream& os) {
        recacheOffsets();
        wt_->dump(os);
        os << "  in OffsetCache at " << std::hex << reinterpret_cast<uint64_t>(xc_->cur()) << " with cached offsets:" << std::endl;
        for (auto voff_iter = voffsets_.begin();
             voff_iter != voffsets_.end();
             voff_iter++)
        {
            os << "    " << voff_iter->first->toIESpec() << " off " << voff_iter->second << std::endl;
        }
    }

    void CollectorOffsetCache::recacheOffsets() {
        size_t nextoff = wt_->maxFixedOffset();
        size_t vl;

        // std::cerr << "recaching offsets" << std::endl;
        
        for (IETemplateIter i = varlenBegin(); 
                            i != varlenEnd(); 
                            i++) {
            voffsets_[*i] = nextoff;
            if (!(nextoff = xc_->decodeVarlenLengthAt(nextoff, vl))) {
                // transcoder signalled unexpected end
                throw CursorError("end of buffer caching offsets: input data not IPFIX?");
            }
            // advance past record content
            nextoff += vl;
            // store length
            vlengths_[*i] = vl;

            // std::cerr << "  length of " << (*i)->toIESpec() << " is " << vl << "; next offset is " << nextoff << std::endl;
        }
        // note that we have a real record length
        reclen_ = nextoff;
        // std::cerr << "  total record length " << reclen_ << std::endl;

    }

    void ExporterOffsetCache::recacheOffsets() {
        size_t nextoff = wt_->maxFixedOffset();
        bool reclen_valid = true;
        
        //std::cerr << "recaching offsets" << std::endl;
        
        for (IETemplateIter i = varlenBegin(); 
                            i != varlenEnd(); 
                            i++) {
            voffsets_[*i] = nextoff;
            size_t len = (*i)->len();
            if (len == kVarlen) {
                if (vlengths_.find(*i) == vlengths_.end()) {
                    //std::cerr << "  length of " << (*i)->toIESpec() << " is not available" << std::endl;
                    reclen_valid = false;
                    break;
                } else {
                    // account for vl encoding in offset calculation
                    len = vlengths_[*i] + ( vlengths_[*i] < 254 ? 1 : 3);
                    //std::cerr << "  length of " << (*i)->toIESpec() << " is " << vlengths_[*i] << "; adding " << len << std::endl;
                }
            }
            nextoff += len;
        }
        reclen_ = reclen_valid ? nextoff : 0;
    }

    void ExporterOffsetCache::setLength(const InfoElement *ie, size_t len) {
        assert(wt_);
    
        if (wt_->contains(ie) && wt_->ieFor(ie)->len() == kVarlen) {
            if (vlengths_[ie]) {
                assert(vlengths_[ie] == len);
            } else {
                vlengths_[ie] = len;
            }
        }
    }
}
