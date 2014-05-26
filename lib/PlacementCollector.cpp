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

#include "IPFIXMessageStreamParser.h"
#include "PlacementCollector.h"
#include "V9MessageStreamParser.h"

namespace LIBFC {

  PlacementCollector::PlacementCollector(Protocol protocol) {
    switch (protocol) {
    case ipfix:
      ir = new IPFIXMessageStreamParser();
      break;
    case netflowv9:
      ir = new V9MessageStreamParser();
      break;
    case netflowv5:
      ir = 0;
      break;
    }

    if (ir != 0)
      ir->set_content_handler(&d);
  }

  PlacementCollector::~PlacementCollector() {
    delete ir;
  }

  std::shared_ptr<ErrorContext> PlacementCollector::collect(InputSource& is) {
    return ir->parse(is);
  }

  void PlacementCollector::register_placement_template(
      const PlacementTemplate* placement) {
    d.register_placement_template(placement, this);
  }

  std::shared_ptr<ErrorContext>
  PlacementCollector::unhandled_data_set(
      uint32_t observation_domain, uint16_t id,
      uint16_t length, const uint8_t* buf) {
    LIBFC_RETURN_OK();
  }

} // namespace LIBFC
