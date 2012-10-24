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
 * This file specifies the interface to WireTemplate, an IETemplate
 * subclass that represents an IPFIX Template on the wire. WireTemplates
 * are used to encode and decode IPFIX-framed information, and can be
 * read from and written to IPFIX Messages.
 */

#ifndef IPFIX_WIRETEMPLATE_H // idem
#define IPFIX_WIRETEMPLATE_H // hack

#include "IETemplate.h"
#include "StructTemplate.h"

namespace IPFIX {

/**
 * Convenience type for a domain/tid pair uniquely identifying a Template within a session
 */
typedef std::pair<uint32_t, uint16_t>            WireTemplateKey;
  
class WireTemplate : public IETemplate {
  
public:

  /** 
   * Create a new WireTemplate. 
   *
   * Client code should use Session::getTemplate() instead, 
   * which automatically creates a new template when necessary.
   *
   * @param domain observation domain ID in which the template lives
   * @param tid template identifier
   */
  WireTemplate(uint32_t domain, uint16_t tid):
    IETemplate(),
    domain_(domain),
    tid_(tid),
    trlen_(kTemplateHeaderLen),
    nextoff_(0),
    max_fixed_offset_(0),
    varlen_count_(0) {}
    
  /** 
   * Create a new WireTemplate. 
   *
   * Client code should use Session::getTemplate() instead, 
   * which automatically creates a new template when necessary.
   *
   * @param domain observation domain ID in which the template lives
   * @param tid template identifier
   * @param n_ies number of ies expected to be in this template
   */
 WireTemplate(uint32_t domain, uint16_t tid, size_t n_ies):
    IETemplate(n_ies),
    domain_(domain),
    tid_(tid),
    trlen_(kTemplateHeaderLen),
    nextoff_(0),
    max_fixed_offset_(0),
    varlen_count_(0) {}
    
  /**
   * Clear and deactivate this WireTemplate.
   *
   * FIXME should interact with template lifetime mechanism
   */
   void clear();
  
  /**
   * Clear this WireTemplate, replace its contents with those of
   * another given Template, and activate it.
   *
   * @param rhs template to mimic
   */
  void mimic(const IETemplate& rhs);

  /**
   * Add an InformationElement to this Template.
   *
   * @param ie pointer to Information Element to add; must be canonical
               (retrieved from InfoModel::instance())
   */
  void add(const InfoElement* ie);

  /**
   * Use this Template to encode a structure described by a template to 
   * a given transcoder. Client code should use Exporter::exportStruct().
   *
   * @param encoder transcoder cued to the location to encode the record
   * @param struct_tmpl template describing the structure in struct_vp
   * @param struct_vp void pointer to struct to encode
   * @return true if encode succeeded, false if not enough space
   */
  bool encodeStruct(Transcoder& encoder, const StructTemplate &struct_tmpl, uint8_t* struct_cp) const;
  
  /**
   * Encode this Template as a template record to a given transcoder.
   * Used internally by Exporter.
   *
   * @param xcoder transcoder cued to the location to encode the template record
   * @return true if encode succeeded, false if not enough space
   */
  bool encodeTemplateRecord(Transcoder& xcoder) const;
  
  /**
   * Use this Template to decode a structure described by a template from 
   * a given transcoder. 
   *
   * Client code should use this call within SetReceiver::receiveSet().
   *
   * @param encoder transcoder cued to the location of the record to decode
   * @param struct_tmpl template describing the structure in struct_vp
   * @param struct_vp void pointer to struct into which to decode
   * @return true if decode succeeded, false if not enough content
   */  
  bool decodeStruct(Transcoder& decoder, const StructTemplate &struct_tmpl, uint8_t* struct_cp) const;

  /**
   * Return the template's observation domain
   */
  uint32_t domain() const { return domain_; }
  
  /**
   * Return the template's template ID
   */
  uint16_t tid() const { return tid_; }
  
  /**
   * Return the maximum non-variable offset of an Information Element
   */
  
  size_t maxFixedOffset() const { return max_fixed_offset_; }
  
  /**
   * Return the number of variable length IEs
   */
  
  size_t varlenCount() const { return varlen_count_; }

  
  /**
   * Return a WireTemplateKey uniquely identifying this template
   */
  const WireTemplateKey key() const {return WireTemplateKey(domain_, tid_);}
  
  virtual void dumpIdent(std::ostream& os) const {
    os << "*** WireTemplate " << domain_ << "/" << tid_ << std::endl;
  }
   
  private:
    WireTemplate():
    IETemplate() {}

    uint32_t                            domain_;
    uint16_t                            tid_;
    size_t                              trlen_;
    size_t                              nextoff_;
    size_t                              max_fixed_offset_;
    size_t                              varlen_count_;
  
  };

} // namespace IPFIX 

#endif /* idem */
