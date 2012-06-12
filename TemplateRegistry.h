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
    TemplateRegistry& instance();

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
