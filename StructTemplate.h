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
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 *
 * This file specifies the interface to StructTemplate, an IETemplate
 * subclass that can be used to represent a C structure in memory.
 */

#ifndef IPFIX_STRUCTTEMPLATE_H // idem
#define IPFIX_STRUCTTEMPLATE_H // hack

#include "IETemplate.h"

namespace IPFIX {

class StructTemplate : public IETemplate {
  
public:
  
  /** 
   * Create a new StructTemplate. 
   */
  StructTemplate():
    IETemplate() {}

  /**
   * Add an InformationElement to this Template.
   *
   * @param ie pointer to Information Element to add; must be canonical
    *          (retrieved from InfoModel::instance())
   * @param offset offset to struct member, as returned by offsetof
   */
  void add(const InfoElement* ie, size_t offset);

  virtual void dumpIdent(std::ostream& os) const {
    os << "*** StructTemplate " << reinterpret_cast<uint64_t> (this) << std::endl;
  }

};
  

} // namespace IPFIX

#endif /* idem */