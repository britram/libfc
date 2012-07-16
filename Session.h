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
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 */
#ifndef IPFIX_SESSION_H // idem
#define IPFIX_SESSION_H // hack

#include <stdint.h>
#include <list>
#include <map>

#include <memory>

#include "InfoModel.h"
#include "WireTemplate.h"

namespace IPFIX {

typedef std::map<WireTemplateKey, std::shared_ptr<WireTemplate> >::iterator    TibIter;

/** A container for per-session state. 
 *
 * It is used by Collector and Exporter, which expose any necessary
 * functionality to client code.
 *
 * Session is the owner for all WireTemplate instances in libfc;
 * therefore, WireTemplates should only be handled by pointer as
 * retrieved from Session::getTemplate()
 */
class Session {

public:
  Session() {}

  uint32_t nextSequence(uint32_t domain, uint32_t stream) { 
      // FIXME stream ignored until we do SCTP
      return next_seq_[domain]; 
  }

  uint32_t incrementSequence(uint32_t domain, uint32_t stream, uint32_t increment) {
      // FIXME stream ignored until we do SCTP
       next_seq_[domain] += increment;
       return next_seq_[domain];
  }

  bool checkSequence(uint32_t domain, uint32_t stream, uint32_t sequence);

  WireTemplate *getTemplate(uint32_t domain, uint16_t tid);
  void deleteTemplate(uint32_t domain, uint16_t tid);
  
  std::list<const WireTemplate *>activeTemplates(uint32_t domain);

  bool decodeTemplateRecord(Transcoder& xc, uint32_t domain);
  
  void reset() {
      next_seq_.clear();
      tib_.clear();
  }

private:
  std::map<uint32_t, uint32_t >                 next_seq_;
  std::map<WireTemplateKey, std::shared_ptr<WireTemplate> >  tib_;
};

}

#endif
