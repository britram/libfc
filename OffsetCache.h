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

/** 
 * @file
 *
 * Defines the OffsetCache interface
 *
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 */


#ifndef IPFIX_OFFSETCACHE_H // idem
#define IPFIX_OFFSETCACHE_H // hack

#include <cassert>
#include <iostream>

#include "InfoElement.h"
#include "WireTemplate.h"
#include "Transcoder.h"
#include "exceptions/CursorError.h"

namespace IPFIX {

    /** 
     * An OffsetCache wraps a wire template and a transcoder, and gets
     * offsets for specific Information Elements within a specific
     * record. Used by both random access encoding and decoding within
     * libfc. Client code should not use OffsetCache directly.
     */

    class OffsetCache {

    private:
        IETemplateIter              viter_;

    protected:

        const WireTemplate*         wt_;
        Transcoder*                 xc_;
        IEIndexMap                  vlengths_;
        IEIndexMap                  voffsets_;
        size_t                      reclen_;

        /** 
         * Create a new offset cache; used by subclasses to access 
         * template and transcoder.
         */

         OffsetCache(const WireTemplate* wt, Transcoder* xc);

        /** 
         * Begin iterating over Information Elements in the Template with
         * first variable-length IE.
         */

        IETemplateIter varlenBegin();
        
        IETemplateIter varlenEnd();
        
        virtual void recacheOffsets() = 0;

    public:

        size_t offsetOf(const InfoElement* ie);

        /** 
         * Return the length of an information element's content
         * (not including any varlen length encoding)
         */

        size_t lengthOf(const InfoElement* ie) {
            return vlengths_[ie];
        }
       
        /**
         * Step the transcoder to the next record; clears the cache.
         */
         void advance();
        
        /**
         * Clear the offset cache to prepare for the next record.
         */
        void clear() {
            vlengths_.clear();
            voffsets_.clear();
            if (!wt_->varlenCount()) {
                reclen_ = wt_->minlen();
            } else {
                reclen_ = 0;
            }
        }

        /**
         * Return the length of the record if it is known, otherwise 0.
         */        
        size_t reclen() {
            return reclen_;
        }
        
        void dump (std::ostream& os);
    };

    /** 
     * A CollectorOffsetCache wraps a wire template and a transcoder, 
     * and gets offsets for specific Information Elements within a
     * specific record from the record contents in the received message.
     */

    class CollectorOffsetCache : public OffsetCache {

    protected:

        void recacheOffsets();

    public:

        CollectorOffsetCache(const WireTemplate* wt, Transcoder* xc):
            OffsetCache(wt, xc)
            {}
        
    };
    
    /** 
     * An ExporterOffsetCache wraps a wire template and a transcoder, 
     * and gets offsets for specific Information Elements within a
     * specific record from the 
     */

    class ExporterOffsetCache : public OffsetCache {

    protected:

        void recacheOffsets();

    public:
        
        ExporterOffsetCache(const WireTemplate* wt, Transcoder *xc):
            OffsetCache(wt, xc)
            {}
        
        /**
         * Reserve a length for a variable length information element
         * in the cache. len is the content length (not including varlen 
         * length encoding).
         */
        void setLength(const InfoElement *ie, size_t len);
    };
}

#endif