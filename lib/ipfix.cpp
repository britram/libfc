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
#include <set>

#include <cstddef>

#include "ipfix.h"

#include "InfoModel.h"
#include "PlacementTemplate.h"
#include "PlacementCollector.h"
#include "FileInputSource.h"
#include "exceptions/FormatError.h"

static bool infomodel_initialized = false;

struct ipfix_template_t {
  IPFIX::PlacementTemplate* tmpl;
  int (*callback) (const ipfix_template_t* t, void *vp);
  void *vparg;
};

class CBinding : public IPFIX::PlacementCollector {
private:
  std::set<IPFIX::PlacementTemplate*> templates;

public:
  CBinding() {
  }

  virtual ~CBinding() {
    for (auto i = templates.begin(); i != templates.end(); ++i)
      delete *i;
    templates.clear();
  }

  void add_template(ipfix_template_t* t) {
    templates.insert(t->tmpl);
  }

  void start_placement(const IPFIX::PlacementTemplate* tmpl) {
  }

  void end_placement(const IPFIX::PlacementTemplate* t) {
    // I wonder if this is portable?  --neuhaust
    const ipfix_template_t* this_template = 
          reinterpret_cast<const ipfix_template_t*>(
            reinterpret_cast<const unsigned char*>(t) -
            offsetof(struct ipfix_template_t, tmpl));
    if (this_template != 0)
      this_template->callback(this_template, this_template->vparg);
  }
};

struct ipfix_template_set_t {
  CBinding* binding;
};

extern struct ipfix_template_set_t* ipfix_template_set_new() {
  if (!infomodel_initialized)
    IPFIX::InfoModel::instance().defaultIPFIX();
  infomodel_initialized = true;

  struct ipfix_template_set_t* ret = new ipfix_template_set_t;
  ret->binding = new CBinding();
  return ret;
}

extern struct ipfix_template_t* ipfix_template_new(
    struct ipfix_template_set_t* s) {
  if (!infomodel_initialized)
    IPFIX::InfoModel::instance().defaultIPFIX();
  infomodel_initialized = true;

  struct ipfix_template_t* ret = new ipfix_template_t;
  ret->callback = 0;
  ret->tmpl = new IPFIX::PlacementTemplate;
  s->binding->add_template(ret);
  return ret;
}

extern void ipfix_template_set_delete(struct ipfix_template_set_t* s) {
  delete s->binding;
}

extern int ipfix_register_placement(struct ipfix_template_t* t,
                                    const char* ie_name, void* p, size_t size) {
  return t->tmpl->register_placement(
           IPFIX::InfoModel::instance().lookupIE(ie_name), p, size);
}

extern void ipfix_register_callback(struct ipfix_template_t* t,
                                    int (*c) (const struct ipfix_template_t*, 
                                               void *),
                                    void *vparg) {
  t->callback = c;
  t->vparg = vparg;
}

extern int ipfix_collect_from_file(int fd, const char* name,
				   struct ipfix_template_set_t* t) {
  int ret = 1;

  IPFIX::FileInputSource is(fd, name);
  try {
    t->binding->collect(is);
  } catch (IPFIX::FormatError e) {
    std::cerr << "Format error: " << e.what() << std::endl;
    ret = 0;
  }

  return ret;
}

extern int ipfix_collect_from_wandio(io_t *wio, struct ipfix_template_set_t* t) {
  int ret = 1;

  IPFIX::WandioInputSource is(wio);
  try {
    t->binding->collect(is);
  } catch (IPFIX::FormatError e) {
    std::cerr << "Format error: " << e.what() << std::endl;
    ret = 0;
  }

  return ret;
}
