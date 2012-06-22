#ifndef IPFIX_RECORDRECEIVER_H // idem
#define IPFIX_RECORDRECEIVER_H // hack

#include "SetReceiver.h"

namespace IPFIX {

    class RecordReceiver : public SetReceiver {
        
        void receiveSet(const Collector* collector, 
                        Transcoder& setxc, 
                        const WireTemplate* wt);
                        
        virtual void receiveRecord() = 0;

        void* getPtr(const InfoElement* ie);
        
        bool getValue(const InfoElement* ie, uint32_t& v);

        bool getValue(const InfoElement* ie, uint64_t& v);
                    
    };

}