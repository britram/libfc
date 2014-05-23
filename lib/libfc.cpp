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

#include "libfc.h"

#include "InfoModel.h"
#include "PlacementTemplate.h"
#include "PlacementCollector.h"
#include "FileInputSource.h"
#include "exceptions/FormatError.h"

static bool infomodel_initialized = false;

struct libfc_template_t {
  LIBFC::PlacementTemplate* tmpl;
  void (*callback) (const libfc_template_t* t);
};

class CBinding : public LIBFC::PlacementCollector {
private:
  std::set<LIBFC::PlacementTemplate*> templates;

public:
  CBinding() 
    : PlacementCollector(PlacementCollector::ipfix) {
  }

  virtual ~CBinding() {
    for (auto i = templates.begin(); i != templates.end(); ++i)
      delete *i;
    templates.clear();
  }

  void add_template(libfc_template_t* t) {
    templates.insert(t->tmpl);
  }

  std::shared_ptr<LIBFC::ErrorContext>
      start_placement(const LIBFC::PlacementTemplate* tmpl) {
    LIBFC_RETURN_OK();
  }

  std::shared_ptr<LIBFC::ErrorContext>
      end_placement(const LIBFC::PlacementTemplate* t) {
    // I wonder if this is portable?  --neuhaust
    const libfc_template_t* this_template 
      = reinterpret_cast<const libfc_template_t*>(
	  reinterpret_cast<const unsigned char*>(t) 
	  - offsetof(struct libfc_template_t, tmpl));
    if (this_template != 0)
      this_template->callback(this_template);
    /* TODO: Adapt callback() so that it too can return an
     * error object */
    LIBFC_RETURN_OK();
  }
};

struct libfc_template_set_t {
  CBinding* binding;
};

extern struct libfc_template_set_t* libfc_template_set_new() {
  if (!infomodel_initialized)
    LIBFC::InfoModel::instance().defaultIPFIX();
  infomodel_initialized = true;

  struct libfc_template_set_t* ret = new libfc_template_set_t;
  ret->binding = new CBinding();
  return ret;
}

extern struct libfc_template_t* libfc_template_new(
    struct libfc_template_set_t* s) {
  if (!infomodel_initialized)
    LIBFC::InfoModel::instance().defaultIPFIX();
  infomodel_initialized = true;

  struct libfc_template_t* ret = new libfc_template_t;
  ret->callback = 0;
  ret->tmpl = new LIBFC::PlacementTemplate;
  s->binding->add_template(ret);
  return ret;
}

extern void libfc_template_set_delete(struct libfc_template_set_t* s) {
  delete s->binding;
}

extern int libfc_register_placement(struct libfc_template_t* t,
                                    const char* ie_name, void* p, size_t size) {
  return t->tmpl->register_placement(
           LIBFC::InfoModel::instance().lookupIE(ie_name), p, size);
}

extern void libfc_register_callback(struct libfc_template_t* t,
                                    void (*c) (const struct libfc_template_t*)) {
  t->callback = c;
}

extern int libfc_collect_from_file(int fd, const char* name,
				   struct libfc_template_set_t* t) {
  int ret = 1;

  LIBFC::FileInputSource is(fd, name);
  try {
    t->binding->collect(is);
  } catch (LIBFC::FormatError e) {
    std::cerr << "Format error: " << e.what() << std::endl;
    ret = 0;
  }

  return ret;
}
