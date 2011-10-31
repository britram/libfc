/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 *
 * This file specifies the interface to StructTemplate, an IETemplate
 * subclass that can be used to represent a C structure in memory.
 */

#ifndef IPFIX_STRUCTTEMPLATE_H // idem
#define IPFIX_STRUCTTEMPLATE_H // hack

#include "IETemplate.h"

namespace IPFIX {

class StructTemplate : public IETemplate {
  
public:
  
  /** 
   * Create a new StructTemplate. 
   */
  StructTemplate():
    IETemplate() {}

  /**
   * Add an InformationElement to this Template.
   *
   * @param ie pointer to Information Element to add; must be canonical
    *          (retrieved from InfoModel::instance())
   * @param offset unused, MUST be zero.
   */
  virtual void add(const InfoElement* ie, size_t offset = 0);

  };

} // namespace IPFIX

// FIXME someone, somewhere, should check minlen in a struct template.

#endif /* idem */