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

/* Hi Emacs, please use -*- mode: C++ -*- */
/**
 * @file
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */
#ifndef IPFIX_TEMPLATEREGISTRY_H
#  define IPFIX_TEMPLATEREGISTRY_H

#  include <map>
#  include <string>

namespace IPFIX {

  /** Register template by name. 
   *
   * This class is deliberately without any connections to the rest of
   * this library. */
  class TemplateRegistry {
  public:
    /** Gets the template registry instance.
     *
     * @return the unique template registry instance.
     */
    static TemplateRegistry& instance();

    /** Returns the template ID associated with the template name.
     *
     * @param template_name the name of the template.  This name must
     *   be unique for each template.
     *
     * @return the ID associated with the template.  If the template
     *   was unknown before, a new template ID is created.
     */
    unsigned int get_template_id(const std::string& template_name);

  private:
    /** Private constructor. */
    TemplateRegistry();

    TemplateRegistry(const TemplateRegistry&) = delete;
    TemplateRegistry& operator=(const TemplateRegistry&) = delete;

    /** The registry instance. */
    static TemplateRegistry registry_instance;

    /** Maps strings to template IDs. */
    std::map<std::string, unsigned int> registry;

    /** The most recently assigned template ID. */
    unsigned int last_id;
  };

} // namespace IPFIX

#endif // IPFIX_TEMPLATEREGISTRY_H
