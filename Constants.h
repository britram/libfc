/** 
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * Defines constants used in libfc
 */
 

#ifndef IPFIX_K_H // idem
#define IPFIX_K_H // hack

#include <stdint.h>

namespace IPFIX {
    
    /** IPFIX framing constant: message header version */
    static const size_t kIpfixVersion = 10;
    /** IPFIX framing constant: message header length */
    static const size_t kMessageHeaderLen = 16;
    /** IPFIX framing constant: set header length */
    static const size_t kSetHeaderLen = 4;
    /** IPFIX framing constant: offset into set header of set length field */
    static const size_t kSetLenOffset = 2;
    /** IPFIX framing constant: length of template record header */
    static const size_t kTemplateHeaderLen = 4;
    
    /** Maximum length of an IPFIX message */
    static const size_t kMaxMessageLen = 65535;
    /** Maximum length of an IPFIX set */
    static const size_t kMaxSetLen = 65519;
    
    /** Set ID for template sets */
    static const uint16_t kTemplateSetID = 2;
    /** Set ID for options template sets */
    static const uint16_t kOptionTemplateSetID = 3;
    /** Minimum Set ID for data sets */
    static const uint16_t kMinSetID = 0x0100;

    /** The special IE length value signifying a variable-length IE */
    static const uint16_t kVarlen = 65535;
    
    /** Set this bit in enterprise-specific IEs */
    static const uint16_t kEnterpriseBit = 0x8000;

}

#endif // idem hack