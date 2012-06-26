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

#include <cstring>
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
