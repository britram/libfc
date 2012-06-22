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
