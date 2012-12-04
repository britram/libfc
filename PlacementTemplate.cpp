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

#include <arpa/inet.h>

#ifdef _IPFIX_HAVE_LOG4CPLUS_
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_DEBUG(logger, expr)
#endif /* _IPFIX_HAVE_LOG4CPLUS_ */

#include "BasicOctetArray.h"
#include "PlacementTemplate.h"

namespace IPFIX {

  class PlacementTemplate::PlacementInfo {
  public:
    PlacementInfo(const InfoElement* ie, void* address, size_t size_on_wire);

    /** Information element.
     *
     * This is used to find out the type of varlen-encoded IEs
     */
    const InfoElement* ie;

    /** Address where to write/read values from/to. */
    void* address;
    
    /** Size of InfoElement on the wire. This is useful only when
     * exporting. */
    size_t size_on_wire;
  };

  PlacementTemplate::PlacementInfo::PlacementInfo(const InfoElement* _ie,
                                                  void* _address,
                                                  size_t _size_on_wire) 
    : ie(_ie), address(_address), size_on_wire(_size_on_wire) {
  }

  PlacementTemplate::PlacementTemplate() 
    : buf(0), 
      size(0),
      fixlen_data_record_size(0)
#ifdef _IPFIX_HAVE_LOG4CPLUS_
    , logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("logger")))
#endif /* _IPFIX_HAVE_LOG4CPLUS_ */
  {
  }

  PlacementTemplate::~PlacementTemplate() {
    delete[] buf;
  }

  bool PlacementTemplate::register_placement(const InfoElement* ie,
                                             void* p, size_t size) {
    LOG4CPLUS_DEBUG(logger, "ENTER register_placement");
    if (size == 0)
      size = ie->canonical()->len();
    placements[ie] = new PlacementInfo(ie, p, size);
    ies.push_back(ie);

    if (size == kVarlen)
      varlen_ies.push_back(placements[ie]);
    else
      fixlen_data_record_size += size;

    return true;
  }

  bool PlacementTemplate::lookup_placement(const InfoElement* ie,
                                           void** p, size_t* size) const {
    LOG4CPLUS_DEBUG(logger, "ENTER lookup_placement");
    std::map<const InfoElement*, PlacementInfo*>::const_iterator it
      = placements.find(ie);
    if (it == placements.end()) {
      *p = 0;
      return false;
    } else {
      *p = it->second->address;
      if (size != 0)
        *size = it->second->size_on_wire;
      return true;
    }
  }

  bool PlacementTemplate::is_match(const MatchTemplate* t) const {
    LOG4CPLUS_DEBUG(logger, "ENTER is_match");

    for (auto i = placements.begin(); i != placements.end(); ++i) {
      LOG4CPLUS_DEBUG(logger, "  looking up IE " << i->first->toIESpec());
      auto p = t->find(i->first);
      if (p == t->end()) {
        LOG4CPLUS_DEBUG(logger, "    not found -> false");
        return false;
      }
    }
    LOG4CPLUS_DEBUG(logger, "  all found -> true");
    return true;
  }

  void PlacementTemplate::wire_template(
      uint16_t template_id,
      const uint8_t** _buf,
      size_t* _size) const {
    LOG4CPLUS_DEBUG(logger, "ENTER wire_template");
    if (buf == 0) {
      /* Templates start with a 2-byte template ID and a 2-byte field
       * count. */
      size =  sizeof(uint16_t) + sizeof(uint16_t);
      uint16_t n_fields = 0;

      for (auto i = placements.begin(); i != placements.end(); ++i) {
        /* A template record is 2 bytes for the IE id, 2 bytes for
         * the field length and a potential 4 more bytes for the
         * private enterprise number, if there is one. */
        size = size + sizeof(uint16_t) + sizeof(uint16_t)
          + (i->first->pen() == 0 ? 0 : sizeof(uint32_t));
        n_fields++;
      }

      buf = new uint8_t[size];

      uint8_t* p = buf + sizeof(uint16_t);

      n_fields = htons(n_fields);
      assert(p + sizeof(n_fields) <= buf + size);
      memcpy(p, &n_fields, sizeof n_fields); p += sizeof n_fields;
      
      for (auto i = placements.begin(); i != placements.end(); ++i) {
        uint32_t ie_pen = htonl(i->first->pen());
        uint16_t ie_id = htons(i->first->number()
                               | (ie_pen == 0 ? 0 : (1 << 15)));
        uint16_t ie_len = htons(i->first->len());
        assert(p + sizeof(ie_id) <= buf + size);
        memcpy(p, &ie_id, sizeof ie_id); p += sizeof ie_id;
        assert(p + sizeof(ie_len) <= buf + size);
        memcpy(p, &ie_len, sizeof ie_len); p += sizeof ie_len;

        if (ie_pen != 0) {
          assert(p + sizeof(ie_pen) <= buf + size);
          memcpy(p, &ie_pen, sizeof ie_pen);
          p += sizeof ie_pen;
        }
      }
      assert(p <= buf + size);
    }

    template_id = htons(template_id);
    assert(buf + sizeof(template_id) <= buf + size);
    memcpy(buf, &template_id, sizeof template_id);

    if (_buf != 0)
      *_buf = buf;
    *_size = size;
  }

  size_t PlacementTemplate::data_record_size() const {
    LOG4CPLUS_DEBUG(logger, "ENTER data_record_size");
    size_t ret = fixlen_data_record_size;

    if (varlen_ies.size() != 0) {
      for (auto i = varlen_ies.begin(); i != varlen_ies.end(); ++i)
        ret += reinterpret_cast<BasicOctetArray*>((*i)->address)->get_length();
    }
    LOG4CPLUS_DEBUG(logger, "RETURN data_record_size=" << ret);
    return ret;
  }

  std::list<const InfoElement*>::const_iterator 
  PlacementTemplate::begin() const {
    return ies.begin();
  }

  std::list<const InfoElement*>::const_iterator 
  PlacementTemplate::end() const {
    return ies.end();
  }

} // namespace IPFIX
