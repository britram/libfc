/* Hi Emacs, please use -*- mode: C++; -*- */
/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
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
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH 
 * ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
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
 * Defines constants used in libfc
 */
 

#ifndef _LIBFC_CONSTANTS_H_
#  define _LIBFC_CONSTANTS_H_

#  include <cstring>
#  include <cstdint>

namespace LIBFC {
  /** IPFIX framing constant: message header version */
  static const size_t kIpfixVersion = 10;

  /** IPFIX framing constant: message header length */
  static const size_t kIpfixMessageHeaderLen = 16;

  /** IPFIX framing constant: set header length */
  static const size_t kIpfixSetHeaderLen = 4;

  /** IPFIX framing constant: offset into set header of set length field */
  static const size_t kIpfixSetLenOffset = 2;

  /** IPFIX framing constant: length of template record header */
  static const size_t kIpfixTemplateHeaderLen = 4;

  /** IPFIX framing constant: length of options template record header */
  static const size_t kIpfixOptionsTemplateHeaderLen = 6;

    
  /** Maximum length of an IPFIX message */
  static const size_t kIpfixMaxMessageLen = 65535;
  
  /** Maximum length of an IPFIX set */
  static const size_t kIpfixMaxSetLen = 65519;
    
  /** Set ID for template sets */
  static const uint16_t kIpfixTemplateSetID = 2;

  /** Set ID for options template sets */
  static const uint16_t kIpfixOptionTemplateSetID = 3;

  /** Minimum Set ID for data sets */
  static const uint16_t kIpfixMinSetID = 0x0100;

  /** Size of field specifier */
  static const size_t kIpfixFieldSpecifierLen = 4;

  /** Size of enterprose number field */
  static const size_t kIpfixEnterpriseLen = 4;

  
  /** The special IE length value signifying a variable-length IE */
  static const uint16_t kIpfixVarlen = 65535;
    
  /** Set this bit in enterprise-specific IEs */
  static const uint16_t kIpfixEnterpriseBit = 0x8000;


    
  /** IPFIX framing constant: message header version */
  static const size_t kV9Version = 9;

  /** IPFIX framing constant: message header length */
  static const size_t kV9MessageHeaderLen = 20;

  /** IPFIX framing constant: set header length */
  static const size_t kV9SetHeaderLen = 4;

  /** IPFIX framing constant: offset into set header of set length field */
  static const size_t kV9SetLenOffset = 2;



  /** Maximum length of @em{any} message that we want to parse. 
   *
   * Please make this the maximum of any kXXXMaxMessageLen-s defined
   * above.  I tried to make your life easier and say
   *
   * @code
   * static const size_t kMaxMessageLen = std::max(kIpfixMaxMessageLen, 
   *                                               static_cast<size_t>(0));
   * @endcode
   *
   * but then the compiler complained that a declaration like
   *
   * @code
   * uint8_t message[kMaxMessageLen];
   * @endcode
   *
   * was invalid because the array size was not an integer constant,
   * despite my (probably erroneous) reasoning that the maximum of two
   * constants is also a constant.
   */
  static const size_t kMaxMessageLen = kIpfixMaxMessageLen;
}

#endif // _LIBFC_CONSTANTS_H_
