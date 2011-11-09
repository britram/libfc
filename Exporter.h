/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * Defines the abstract exporter interface.
 *
 * To send IPFIX Messages, client code should create an instance
 * of an Exporter subclass for the necessary transport, set the
 * observation domain via setDomain() and the export template via
 * setTemplate(), and call exportRecord() to send each record.
 *
 * flush() can be called to explicitly end a message.
 *
 * Template management is achieved through getTemplate() (which will
 * create a new template for export if it doesn't exist yet) and
 * exportTemplatesForDomain().
 */

#ifndef IPFIX_EXPORTER_H // idem
#define IPFIX_EXPORTER_H // hack

#include <ctime>
#include <stdexcept>
#include "Session.h"
#include "Transcoder.h"

namespace IPFIX {

/**
 * Logic error signifying that a too-small
 * MTU has been set on an Exporter for a given message.
 */
class MTUError : public std::runtime_error {
public:
  explicit MTUError(const std::string& what_arg): 
    std::runtime_error(what_arg) {}
};
  
class Exporter {
  
public:

  /**
   * Change the active observation domain for the exporter.
   *
   * @param domain new observation domain ID
   */
  void setDomain(uint32_t domain);

  /**
   * Change the active template for the exporter.
   *
   * @param tid new template ID
   */
  void setTemplate(uint16_t tid);
  
  /**
   * Get the WireTemplate for a given template ID in the current 
   * observation domain. Creates a new inactive template if no
   * template has yet been created for this ID.
   *
   * @param tid template ID to get
   * @return pointer to template for given ID
   */
  WireTemplate *getTemplate(uint16_t tid) { 
    return session_.getTemplate(domain_, tid); 
  }
  
  /**
   * Export all active templates for the current observation domain.
   */
  void exportTemplatesForDomain();
  
  /**
   * Export a record using the current wire template, for a given 
   */
  void exportRecord(const StructTemplate &struct_tmpl, uint8_t* struct_cp);
  
  /**
   * Flush the current message with the given export time
   *
   * @param export time export time in epoch seconds
   */
  void flush(time_t export_time);

  /**
   * Flush the current message with the export time
   *
   * @param export time export time in epoch seconds
   */  
  void flush() { flush(time(NULL)); }
  
  // FIXME fix signed/unsigned issues in export time throughout libfc
  // FIXME add a way to withdraw a template

  /**
   * Exporter virtual destructor
   */
  virtual ~Exporter();
protected:
  /**
   * Create a new Exporter.
   *
   * Called by subclasses to initialize Exporter internal
   * structures.
   *
   * @param domain initial observation domain; 
   *               can be changed after creation with setDomain()
   * @param mtu maximum message size
   */
  Exporter(uint32_t domain, size_t mtu);
  
  
  /**
   * Low-level interface to export interface; overridden by subclasses
   *
   * @param base pointer to buffer to send
   * @param len length of buffer to send
   */  
  virtual void _sendMessage(uint8_t *base, size_t len) = 0;

private:
  // make me uncopyable
  Exporter();
  Exporter(Exporter& rhs);
  Exporter& operator=(Exporter& rhs);

  void endSet() { xcoder_.encodeSetEnd(); set_active_ = false; }
  void ensureSet();
  void ensureTemplateSet();
  void startMessage();
  void endMessage(time_t export_time);
  
  
  uint8_t*                  buf_;
  Transcoder                xcoder_;
  uint16_t                  set_id_;
  bool                      msg_empty_;
  bool                      set_active_;
  Session                   session_;
  WireTemplate*             tmpl_;
  uint32_t                  domain_;
  size_t                    mtu_;
};

}

#endif
