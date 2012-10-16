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

#include "Decoder.h"

namespace IPFIX {

  static std::list<std::pair<const InformationElement*, void*> >::iterator
  find_ie(std::list<std::pair<const InformationElement*, void*> >& ies,
          const InformationElement* ie) {
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

  void Decoder::add_src_ie(const InformationElement* ie) {
    auto i = find_ie(ies, ie);
    assert(i == ies.end());

    ies.push_back(std::make_pair(ie, 0));
  }

  void Decoder::add_dst_ie(const InformationElement* ie, void* p) {
    auto i = find_ie(ies, ie);
    assert(i != ies.end());

    i->second = p;
  }

  void Decoder::decode(Transcoder& xcoder) const {
  }

} // namespace IPFIX
