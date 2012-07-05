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
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * Defines the record receiver interface. 
 *
 */

#ifndef IPFIX_RECORDRECEIVER_H // idem
#define IPFIX_RECORDRECEIVER_H // hack

#include "SetReceiver.h"
#include "OffsetCache.h"

namespace IPFIX {

/**
 * A RecordReceiver is a SetReceiver that allows per-record random access;
 * it is the collector-side analogue to the random access putValue() - based
 * exporter interface. To use, inherit from RecordReceiver, implement 
 * receiveRecord, and call getValue() to retrieve values from the record.
 */

    class RecordReceiver : public SetReceiver {
        
    private:

        const WireTemplate* wt_;
        Transcoder* xc_;
        CollectorOffsetCache oc_;

    protected:
        RecordReceiver():
            wt_(NULL),
            xc_(NULL),
            oc_(NULL,NULL) {}
            
        const void *getPointer(const InfoElement* ie, size_t& len);
        
        bool getValue(const InfoElement* ie, void *vp, size_t len);

        bool getValue(const InfoElement* ie, uint64_t& val) {
            return getValue(ie, &val, sizeof(val));
        }

        bool getValue(const InfoElement* ie, uint32_t& val) {
            return getValue(ie, &val, sizeof(val));
        }

        bool getValue(const InfoElement* ie, uint16_t& val) {
            return getValue(ie, &val, sizeof(val));
        }
        
        bool getValue(const InfoElement* ie, uint8_t& val) {
            return getValue(ie, &val, sizeof(val));
        }

        bool getValue(const InfoElement* ie, std::string& val);
        
    public:
        
        void receiveSet(const Collector* collector, 
                        Transcoder& setxc, 
                        const WireTemplate* wt);
                        
        virtual void receiveRecord() = 0;

    };

}

#endif
