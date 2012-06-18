#ifndef IPFIX_EXPORT_CURSOR_H // idem
#define IPFIX_EXPORT_CURSOR_H // hack

#include <cassert>
#include "InfoElement.h"
#include "WireTemplate.h"
#include "exceptions/CursorError.h"

namespace IPFIX {

    class ExportCursor {
        
    private:
        // Wire template
        const WireTemplate*         wt_;
        // Current record offsets table
        IEIndexMap                  rec_offsets_;
        // Current record lengths table
        IEIndexMap                  rec_lengths_;
        // Cached iterator for variable-offset IEs in WT
        IETemplateIter              iter_;
        // Flag if iter_ is valid
        bool                        iter_cached_;
        // reserved length
        size_t                      reslen_;
        // additional length (3 byte varlen encoding)
        size_t                      addlen_;

        IETemplateIter begin() {
            if (!iter_cached_) {
                IETemplateIter ieiter;
                for (ieiter = wt_->begin(); 
                     ieiter != wt_->end() && (*ieiter)->len() != kVarlen;
                     ieiter++);
                iter_ = ieiter;
                iter_cached_ = true;
            }
            return iter_;
        }
        
        IETemplateIter end() {
            return wt_->end();
        }
        
    public:

        ExportCursor():
            wt_(NULL),
            iter_cached_(false),
            reslen_(0),
            addlen_(0) {}
        
        ExportCursor(const WireTemplate* wt):
            wt_(wt),
            iter_cached_(false),
            reslen_(0),
            addlen_(0) {}
        
        void setIELen(const InfoElement *ie, size_t len) {
            assert(wt_);
            
            if (wt_->contains(ie) && wt_->ieFor(ie)->len() == kVarlen) {
                if (rec_lengths_[ie]) {
                    assert(rec_lengths_[ie] == len);
                } else {
                    rec_lengths_[ie] = len;
                    reslen_ += len;
                }
            }
        }

        size_t offsetOf(const InfoElement* ie) {
            assert(wt_);
            
            size_t off;
            
            // If offset available in template, return it.
            if ((off = wt_->offset(ie)) != kVarlen) {
                return off;
            }
            
            // If offset already stored, return it.
            if ((off = rec_offsets_[ie])) {
                return off;
            }

            // Nope, walk the template and fill out all the offsets we can
            size_t nextoff = wt_->maxFixedOffset();
            for (IETemplateIter ieiter = begin(); 
                                ieiter != end(); 
                                ieiter++) {
                rec_offsets_[*ieiter] = nextoff;
                size_t len = (*ieiter)->len();
                if (len == kVarlen) {
                    if (rec_lengths_.find(*ieiter) == rec_lengths_.end()) {
                        break;
                    } else {
                        len = rec_lengths_[*ieiter];
                    }
                }
                nextoff += len;
            }
            
            // Now look again. If not there, we have to give up.
            if (!(off = rec_offsets_[ie])) {
                throw CursorError("can't find offset, length not reserved");
            }
            
            return off;
        }
        
        size_t len() {
            // reslen = reserved length
            // addlen = additional length (three-byte varlen encoding)
            // minlen = fixed length plus one byte per varlen
            return reslen_ + addlen_ + wt_->minlen();
        }
        
        void addLen(size_t addlen) {
            addlen_ += addlen;
        }
        
        bool fitsIn(size_t avail) {
            assert(wt_);
            
            return avail >= (len());
        }
        
        void clear() {
            rec_lengths_.clear();
            rec_offsets_.clear();
            reslen_ = 0;
            addlen_ = 0;
        }
    };

}

#endif