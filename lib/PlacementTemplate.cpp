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
#include <climits>

#include <arpa/inet.h>

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include "BasicOctetArray.h"
#include "PlacementTemplate.h"

namespace LIBFC {

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
      fixlen_data_record_size(0),
      template_id(0)
#ifdef _LIBFC_HAVE_LOG4CPLUS_
    , logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("PlacementTemplate")))
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
  {
  }

  PlacementTemplate::~PlacementTemplate() {
    delete[] buf;
  }

  bool PlacementTemplate::register_placement(const InfoElement* ie,
                                             void* p, size_t size) {
    if (size == 0)
      size = ie->len();
    placements[ie] = new PlacementInfo(ie, p, size);
    ies.push_back(ie);

    if (size == kIpfixVarlen)
      varlen_ies.push_back(placements[ie]);
    else
      fixlen_data_record_size += size;

    return true;
  }

  bool PlacementTemplate::lookup_placement(const InfoElement* ie,
                                           void** p, size_t* size) const {
    LOG4CPLUS_TRACE(logger, "ENTER lookup_placement");
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

  unsigned int PlacementTemplate::is_match(
      const IETemplate* t,
      std::set<const InfoElement*>* unmatched) const {
    LOG4CPLUS_TRACE(logger, "ENTER is_match");
    
    bool found = true;

    for (auto i = placements.begin(); i != placements.end(); ++i) {
      LOG4CPLUS_TRACE(logger, "  looking up IE " << i->first->toIESpec());
      if (!t->contains(i->first)) {
        LOG4CPLUS_TRACE(logger, "    not found -> false");
        found = false;
	break;
      }
    }

    if (found) {
      LOG4CPLUS_TRACE(logger, "  all found -> return " << placements.size());
      assert(placements.size() <= UINT_MAX);

      if (unmatched != 0) {
	unmatched->clear();

	for (auto i = t->begin(); i != t->end(); ++i) {
          for (auto k = placements.begin(); k != placements.end(); ++k) {
            if (!k->first->matches(*i))
              unmatched->insert(*i);
          }
	}
      }

      return static_cast<unsigned int>(placements.size());
    } else
      return 0;
  }

  void PlacementTemplate::wire_template(
      uint16_t _template_id,
      const uint8_t** _buf,
      size_t* _size) const {
    LOG4CPLUS_TRACE(logger, "ENTER wire_template");
    if (buf == 0) {
      LOG4CPLUS_TRACE(logger,
                      "  computing wire template, id=" << _template_id);
      assert(_template_id != 0);
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
      
      /* Use IES, not PLACEMENTS for iteration, because now, sequence
       * matters. */
      for (auto i = ies.begin(); i != ies.end(); ++i) {
        LOG4CPLUS_TRACE(logger,
                        "  wire template for (" << (*i)->pen()
                        << "/" << (*i)->number()
                        << ")[" << (*i)->len() << "]");

        uint32_t ie_pen = htonl((*i)->pen());
        uint16_t ie_id = htons((*i)->number()
                               | (ie_pen == 0 ? 0 : (1 << 15)));
        uint16_t ie_len = htons((*i)->len());
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

      /* Can set template ID only once. */
      template_id = _template_id;
      _template_id = htons(_template_id);
      assert(buf + sizeof(_template_id) <= buf + size);
      memcpy(buf, &_template_id, sizeof _template_id);
    }

    if (_buf != 0)
      *_buf = buf;
    *_size = size;
  }

  size_t PlacementTemplate::data_record_size() const {
    size_t ret = fixlen_data_record_size;

    if (varlen_ies.size() != 0) {
      for (auto i = varlen_ies.begin(); i != varlen_ies.end(); ++i) {
        uint16_t varlen_len
          = reinterpret_cast<BasicOctetArray*>((*i)->address)->get_length();
        ret += varlen_len + (varlen_len < 255 ? 1 : 3);
      }
    }
    return ret;
  }

  uint16_t PlacementTemplate::get_template_id() const {
    return template_id;
  }

  std::list<const InfoElement*>::const_iterator 
  PlacementTemplate::begin() const {
    return ies.begin();
  }

  std::list<const InfoElement*>::const_iterator 
  PlacementTemplate::end() const {
    return ies.end();
  }

} // namespace LIBFC
