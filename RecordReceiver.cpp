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

bool RecordReceiver::getValue(const InfoElement* ie, void *vp, size_t len) {
    if (!wt_->contains(ie)) return false;
    size_t nextoff = xc_->decodeAt(vp, len, oc_.offsetOf(ie), wt_->ieFor(ie));
    return true;
}

}