#ifndef IPFIX_RECORDRECEIVER_H // idem
#define IPFIX_RECORDRECEIVER_H // hack

#include "SetReceiver.h"
#include "OffsetCache.h"

namespace IPFIX {

    class RecordReceiver : public SetReceiver {
        
    private:

        const WireTemplate* wt_;
        Transcoder* xc_;
        CollectorOffsetCache oc_;

        bool getValue(const InfoElement* ie, void *vp, size_t len);
        
    protected:
        RecordReceiver():
            wt_(NULL),
            xc_(NULL),
            oc_(NULL,NULL) {}
        
    public:
        
        void receiveSet(const Collector* collector, 
                        Transcoder& setxc, 
                        const WireTemplate* wt);
                        
        virtual void receiveRecord() = 0;

    };

}

#endif