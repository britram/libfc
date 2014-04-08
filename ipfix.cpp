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
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH
 * ZÜRICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#include "ipfix.h"

#include "InfoModel.h"
#include "PlacementTemplate.h"
#include "PlacementCollector.h"
#include "FileInputSource.h"
#include "exceptions/FormatError.h"

static bool infomodel_initialized = false;

class CBinding : public IPFIX::PlacementCollector {
private:
  const struct ipfix_template_t* enclosing;
  IPFIX::PlacementTemplate* tmpl;
  void (*callback) (const ipfix_template_t* t);

public:
  CBinding(const struct ipfix_template_t* enclosing) 
    : enclosing(enclosing),
      tmpl(new IPFIX::PlacementTemplate()), callback(0) {
  }

  virtual ~CBinding() {
    delete tmpl;
  }

  bool register_placement(const char* name, void* address, size_t size) {
    return tmpl->register_placement(IPFIX::InfoModel::instance().lookupIE(name),
                                    address, size);
  }

  void register_callback(void (*c) (const struct ipfix_template_t*)) {
    callback = c;
  }

  void start_placement(const IPFIX::PlacementTemplate* tmpl) {
  }

  void end_placement(const IPFIX::PlacementTemplate* tmpl) {
    callback(enclosing);
  }
};

struct ipfix_template_t {
  CBinding* binding;
};

extern struct ipfix_template_t* ipfix_template_new() {
  if (!infomodel_initialized)
    IPFIX::InfoModel::instance().defaultIPFIX();
  infomodel_initialized = true;

  struct ipfix_template_t* ret = new ipfix_template_t;
  ret->binding = new CBinding(ret);
  return ret;
}

extern void ipfix_template_delete(struct ipfix_template_t* p) {
  delete p->binding;
  delete p;
}

extern int ipfix_register_placement(struct ipfix_template_t* t,
                                    const char* ie_name, void* p, size_t size) {
  return t->binding->register_placement(ie_name, p, size);
}

extern void ipfix_register_callback(struct ipfix_template_t* t,
                                    void (*c) (const struct ipfix_template_t*)) {
  t->binding->register_callback(c);
}

extern int ipfix_collect_from_file(int fd, struct ipfix_template_t* t) {
  int ret = 1;

  IPFIX::FileInputSource is(fd);
  try {
    t->binding->collect(is);
  } catch (IPFIX::FormatError e) {
    std::cerr << "Format error: " << e.what() << std::endl;
    ret = 0;
  }

  return ret;
}
