#ifndef IPFIX_OFFSETCACHE_H // idem
#define IPFIX_OFFSETCACHE_H // hack

#include <cassert>

#include "InfoElement.h"
#include "WireTemplate.h"
#include "Transcoder.h"
#include "exceptions/CursorError.h"

namespace IPFIX {

    /** 
     * An OffsetCache wraps a wire template and a transcoder, and gets
     * offsets for specific Information Elements within a specific
     * record. Used by both random access encoding and decoding within
     * libfc.
     */

    class OffsetCache {

    private:
        IETemplateIter              viter_;
        bool                        viter_cached_;

    protected:

        const WireTemplate*         wt_;
        Transcoder*                 xc_;
        IEIndexMap                  vlengths_;
        IEIndexMap                  voffsets_;
        size_t                      reclen_;
        bool                        reclen_valid_;

        OffsetCache(const WireTemplate* wt, Transcoder* xc):
            wt_(wt),
            xc_(xc)
            {}

        IETemplateIter begin();
        
        IETemplateIter end();
        
        virtual void recacheOffsets() = 0;

    public:

        size_t offsetOf(const InfoElement* ie);

        size_t lengthOf(const InfoElement* ie) {
            return vlengths_[ie];
        }
       
        /**
         * Step the transcoder to the next record; clears the cache.
         */
         bool advance();
        
        /**
         * Clear the offset cache to the next record
         */
        
        void clear() {
            vlengths_.clear();
            voffsets_.clear();
            reclen_ = wt_->maxFixedOffset();
            reclen_valid_ = false;
        }
        
        size_t reclen() {
            return reclen_valid_ ? reclen_ : 0;
        }
    };

    class CollectorOffsetCache : public OffsetCache {

    protected:

        void recacheOffsets();

    public:

        CollectorOffsetCache(const WireTemplate* wt, Transcoder* xc):
            OffsetCache(wt, xc)
            {}
        
    };
    
    class ExporterOffsetCache : public OffsetCache {

    protected:

        void recacheOffsets();

    public:
        
        ExporterOffsetCache(const WireTemplate* wt, Transcoder *xc):
            OffsetCache(wt, xc)
            {}
        
        /**
         * Reserve a variable record length in the cache.
         */
        void setIELen(const InfoElement *ie, size_t len);
        
        bool mightFit() {
            return xc_->avail() >= reclen_;
        }
    };
}

#endif