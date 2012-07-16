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


#include "RecordReceiver.h"

namespace IPFIX {

void RecordReceiver::receiveSet(const Collector* collector, 
                                Transcoder& setxc, 
                                const WireTemplate* wt) {

    // FIXME thread-unsafe

    // stash transcoder and wire template
    xc_ = &setxc;
    wt_ = wt;
    
    // create a new offset cache
    oc_ = CollectorOffsetCache(wt_, xc_);

    while (xc_->avail() >= wt->minlen()) {
        // let the subclass read the record
        receiveRecord();

        // advance to next record
        oc_.advance();
    }
}

const void *RecordReceiver::getPointer(const InfoElement* ie, size_t& len) {
    // FIXME come up with a way to do this without reusing varlen fields.
    VarlenField vf;
    
    size_t nextoff = xc_->decodeAt(vf, oc_.offsetOf(ie), wt_->ieFor(ie));
    if (nextoff == 0) return NULL;
    len = vf.len;
    return vf.cp;
}


bool RecordReceiver::getValue(const InfoElement* ie, void *vp, size_t len) {
    if (!wt_->contains(ie)) return false;
    size_t nextoff = xc_->decodeAt(vp, len, oc_.offsetOf(ie), wt_->ieFor(ie));
    return true;
}

bool RecordReceiver::getValue(const InfoElement* ie, std::string& v) {
    if (!wt_->contains(ie)) return false;
    
    size_t len = 0;
    const char *cp = static_cast<const char *>(getPointer(ie, len));
    v = std::string(cp, len);
    return true;
}


}
