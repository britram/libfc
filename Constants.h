#ifndef IPFIX_K_H // idem
#define IPFIX_K_H // hack

#include <stdint.h>

namespace IPFIX {
    
    static const size_t kIpfixVersion = 10;
    static const size_t kMessageHeaderLen = 16;
    static const size_t kSetHeaderLen = 4;
    static const size_t kSetLenOffset = 2;
    static const size_t kTemplateHeaderLen = 4;
    
    static const size_t kMaxMessageLen = 65535;
    static const size_t kMaxSetLen = 65519;
    
    static const uint16_t kTemplateSetID = 2;
    static const uint16_t kOptionTemplateSetID = 3;
    static const uint16_t kMinSetID = 0x0100;

    /** The special IE length value signifying a variable-length IE */
    static const uint16_t kVarlen = 65535;
    
    /** Set this bit in enterprise-specific IEs */
    static const uint16_t kEnterpriseBit = 0x8000;

}

#endif // idem hack