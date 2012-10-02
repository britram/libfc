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
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch> (stealing code
 * from Brian Trammell <trammell@tik.ee.ethz.ch>)
 *
 * @section DESCRIPTION
 * 
 * A BufWriter is an Exporter that writes to a buffer.
 *
 */

#ifndef IPFIX_BUFWRITER_H // idem
#define IPFIX_BUFWRITER_H // hack

#include <cstdio>
#include "Exporter.h"

namespace IPFIX {
 
class BufWriter : public Exporter {
public:
  
  static const size_t kDefaultMTU = 65535;

  BufWriter(uint32_t domain, size_t mtu=kDefaultMTU);
  
  virtual ~BufWriter();

  const unsigned char* buf() const;
  size_t len() const;

protected:
  void _sendMessage(uint8_t *base, size_t len);

private:
  unsigned char* _buf;
  unsigned char* _cur;
  size_t _len;
  size_t _capacity;
};

} // namespace IPFIX

#endif // idem hack
