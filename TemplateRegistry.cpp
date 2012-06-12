#include "Constants.h"
#include "TemplateRegistry.h"

namespace IPFIX {

  TemplateRegistry TemplateRegistry::registry_instance;

  TemplateRegistry& TemplateRegistry::instance() {
    return registry_instance;
  }

  unsigned int 
  TemplateRegistry::get_template_id(const std::string& template_name) {
    auto i = registry.find(template_name);
    if (i == registry.end()) {
      registry[template_name] = ++last_id;
      return last_id;
    }
    else
      return i->second;
  }

  TemplateRegistry::TemplateRegistry() : last_id(kMinSetID) {
  }

} // namespace IPFIX
