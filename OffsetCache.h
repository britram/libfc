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
        IEIndexMap                  vlengths_;
        IEIndexMap                  voffsets_;
        size_t                      reclen_;
        bool                        reclen_valid_;

        OffsetCache(const WireTemplate* wt):
            wt_(wt)
            {}

        IETemplateIter begin();
        
        IETemplateIter end();
        
        virtual void recacheOffsets() = 0;

    public:

        size_t offsetFor(const InfoElement* ie);
       
        
        /**
         * Clear the offset cache to the next record
         */
        
        void clear() {
            vlengths_.clear();
            voffsets_.clear();
            reclen_ = wt_->maxFixedOffset();
            reclen_valid_ = false;
        }
    };

    class CollectorOffsetCache : public OffsetCache {

    private:

        Transcoder* xc_;

    protected:

        void recacheOffsets();

    public:

        CollectorOffsetCache(const WireTemplate* wt, Transcoder* xc):
            OffsetCache(wt),
            xc_(xc)
            {}


        /**
         * Step the transcoder to the next record; clears the cache.
         */
        bool advance() {
            if (!reclen_valid_) {
                recacheOffsets();
                assert(reclen_valid_);
            }
            
            if (xc_->advance(reclen_)) {
                clear();
                return true;
            } else {
                return false;
            }
        }
        
    };
    
    class ExporterOffsetCache : public OffsetCache {

        ExporterOffsetCache(const WireTemplate* wt):
            OffsetCache(wt)
            {}

        
        /**
         * Reserve a variable record length in the cache.
         */
        void setIELen(const InfoElement *ie, size_t len);

        void recacheOffsets();
       
    };
}

#endif