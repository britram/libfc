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