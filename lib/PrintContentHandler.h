/* Hi Emacs, please use -*- mode: C++; -*- */
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

/**
 * @file
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */

#ifndef _libfc_PRINTCONTENTHANDLER_H_
#  define _libfc_PRINTCONTENTHANDLER_H_

#  if defined(_libfc_HAVE_LOG4CPLUS_)
#    include <log4cplus/logger.h>
#  endif /* defined(_libfc_HAVE_LOG4CPLUS_) */

#  include "InfoModel.h"
#  include "ContentHandler.h"

namespace libfc {

  /** This class decodes messages and prints them to stderr.  It is
   * therefore mainly useful for debugging.
   */
  class PrintContentHandler : public ContentHandler {
  public:
    PrintContentHandler(uint16_t expected_version);
    PrintContentHandler(uint16_t expected_version, unsigned int max_messages);

    std::shared_ptr<ErrorContext> start_session();
    std::shared_ptr<ErrorContext> end_session();

    std::shared_ptr<ErrorContext> start_message(uint16_t version,
                       uint16_t length,
                       uint32_t export_time,
                       uint32_t sequence_number,
                       uint32_t observation_domain,
                       uint64_t base_time);
    std::shared_ptr<ErrorContext> end_message();
    std::shared_ptr<ErrorContext> start_template_set(uint16_t set_id,
                            uint16_t set_length,
                            const uint8_t* buf);
    std::shared_ptr<ErrorContext> end_template_set();
    std::shared_ptr<ErrorContext> start_options_template_set(uint16_t set_id,
                                   uint16_t set_length,
                                   const uint8_t* buf);
    std::shared_ptr<ErrorContext> end_options_template_set();
    std::shared_ptr<ErrorContext> start_data_set(uint16_t id,
                                                 uint16_t length,
                                                 const uint8_t* buf);
    std::shared_ptr<ErrorContext> end_data_set();

    /* Addiional functions */
    std::shared_ptr<ErrorContext> start_template_record(uint16_t template_id,
                                                        uint16_t field_count);
    std::shared_ptr<ErrorContext> end_template_record();
    std::shared_ptr<ErrorContext> start_options_template_record(
      uint16_t template_id,
      uint16_t field_count,
      uint16_t scope_field_count);
    std::shared_ptr<ErrorContext> end_options_template_record();
    std::shared_ptr<ErrorContext> field_specifier(bool enterprise,
                                                  uint16_t ie_id,
                                                  uint16_t ie_length,
                                                  uint32_t enterprise_number);

  private:
    std::shared_ptr<ErrorContext> process_template_set(
      uint16_t set_id,
      uint16_t set_length,
      const uint8_t* buf,
      bool is_options_set);

    /** The cached InfoModel instance. */
    InfoModel& info_model;
    
    /** The version we expect to see in message headers. */
    uint16_t expected_version;

    /** The maximum number of messages before exiting with a synthetic
     * error in start_message().  If this is 0, this class will
     * attempt to process all messages. */
    unsigned int max_messages;

    /** The number of messages seen so far. */
    unsigned int n_messages;

#  if defined(_libfc_HAVE_LOG4CPLUS_)
    log4cplus::Logger logger;
#  endif /* defined(_libfc_HAVE_LOG4CPLUS_) */
 };

} // namespace libfc

#endif // _libfc_PRINTCONTENTHANDLER_H_
