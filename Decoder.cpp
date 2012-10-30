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
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */

#include <algorithm>
#include <cassert>

#include "Constants.h"
#include "Decoder.h"

namespace IPFIX {

  static std::list<std::pair<const InfoElement*, void*> >::iterator
  find_ie(std::list<std::pair<const InfoElement*, void*> >& ies,
          const InfoElement* ie) {
    // Replace this linear search by something smarter if it turns out
    // to be a performance problem. It shouldn't be, though, since
    // this member function will be called once for every IE that a
    // client is interested in, but before the loop decoding records.
    auto i = ies.begin();
    while (i != ies.end()) {
      if (i->first == ie)
        break;
      ++i;
    }

    return i;
  }

  void Decoder::add_src_ie(const InfoElement* ie) {
    auto i = find_ie(ies, ie);
    assert(i == ies.end());

    ies.push_back(std::make_pair(ie, static_cast<void*>(0)));
  }

  void Decoder::add_dst_ie(const InfoElement* ie, void* p) {
    auto i = find_ie(ies, ie);
    assert(i != ies.end());
    assert(i->second == 0);

    i->second = p;
  }

  void Decoder::decode_record(Transcoder& xcoder) const {
    for (auto ie = ies.begin(); ie != ies.end(); ++ie) {
      if (ie->second == 0)      /* Skip this IE */
        xcoder.decodeSkip(ie->first);
      else {
        switch (ie->first->ietype()->number()) {
        case IEType::kUnsigned8:
        case IEType::kSigned8:
        case IEType::kBoolean:
          break;
        case IEType::kUnsigned16:
        case IEType::kSigned16:
          break;
        case IEType::kUnsigned32:
        case IEType::kSigned32:
        case IEType::kFloat32:
        case IEType::kIpv4Address:
        case IEType::kDateTimeSeconds:
          break;
        case IEType::kUnsigned64:
        case IEType::kSigned64:
        case IEType::kFloat64:
        case IEType::kDateTimeMilliseconds:
        case IEType::kDateTimeMicroseconds:
        case IEType::kDateTimeNanoseconds:
          break;
        case IEType::kMacAddress:
          break;
        case IEType::kIpv6Address:
          break;
        case IEType::kOctetArray:
        case IEType::kString:
          if (ie->first->len() == kVarlen) { // Varlen string/octet array
          } else { // Fixlen string/octet array
          }
          break;
        default: break;
        }
      }
    }
  }

} // namespace IPFIX
