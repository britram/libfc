/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * The name of ETH Zürich nor the names of other contributors 
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
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */
#if defined(_LIBFC_HAVE_LOG4CPLUS_)
#  include <log4cplus/logger.h>
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#  define LOG4CPLUS_ERROR(logger, expr)
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */

#include <cstdio>
#include <cstring>
#include <ctime>
#include <cassert>
#include <iostream>

#include "Constants.h"
#include "InfoElement.h"
#include "PrintContentHandler.h"

#include "decode_util.h"
#include "pointer_checks.h"

#define PH_RETURN_CALLBACK_ERROR(call)                                  \
  do {                                                                  \
    /* Make sure call is evaluated only once */                         \
    std::shared_ptr<ErrorContext> err = call;                           \
    if (err != 0)                                                       \
      return err;                                                       \
  } while (0)

namespace libfc {

  PrintContentHandler::PrintContentHandler(uint16_t expected_version)
    : info_model(InfoModel::instance()),
      expected_version(expected_version),
      max_messages(0),
      n_messages(0)
#if defined(_LIBFC_HAVE_LOG4CPLUS_)
                                        , 
      logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("PrintContentHandler")))
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */
  {
  }

  PrintContentHandler::PrintContentHandler(uint16_t expected_version,
                                           unsigned int max_messages)
    : info_model(InfoModel::instance()),
      expected_version(expected_version),
      max_messages(max_messages),
      n_messages(0)
#if defined(_LIBFC_HAVE_LOG4CPLUS_)
                                        , 
      logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("PrintContentHandler")))
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */
  {
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::start_session() {
    std::cerr << "Session starts" << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::end_session() {
    std::cerr << "Session ends" << std::endl;
    libfc_RETURN_OK();
  }
  
  static char* make_local_time(uint32_t t_arg) {
    /* From asctime(1): The asctime_r() function does the same [as
     * asctime], but stores the string in a user-supplied buffer which
     * should have room for at least 26 bytes. */
    char* ret = new char[27];
    time_t t = t_arg;

    asctime_r(localtime(&t), ret);

    assert(ret != 0 && strlen(ret) > 0);

    // Remove terminating \n. Who comes up with these crap APIs?
    ret[strlen(ret) - 1] = '\0';

    return ret;
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::start_message(uint16_t version,
                     uint16_t length,
                     uint32_t export_time,
                     uint32_t sequence_number,
                     uint32_t observation_domain,
                     uint64_t base_time) {
    n_messages++;

    if (max_messages > 0 && n_messages > max_messages)
      libfc_RETURN_ERROR(recoverable, aborted_by_user,
                         "Maximum number of messages (" << max_messages
                         << ") reached; aborting further processing",
                         0, 0, 0, 0, 0);

    char* export_t = make_local_time(export_time);
    char* base_t =  make_local_time(base_time/1000);
    
    std::cerr << "  Message: version=" << std::hex << std::showbase << version
              << ", length=" << std::dec << std::noshowbase << length
              << ", extime=" << export_t
              << ", seq=" << sequence_number
              << ", domain=" << observation_domain
              << ", basetime=" << base_t
              << std::endl;
    
    delete[] export_t;
    delete[] base_t;

    if (version != expected_version)
      libfc_RETURN_ERROR(recoverable, message_version_number,
                         "Expected version number " << expected_version
                         << ", got " << version,
                         0, 0, 0, 0, 0);
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::end_message() {
    std::cerr << "  Message ends" << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::start_template_set(uint16_t set_id,
                          uint16_t set_length,
                          const uint8_t* buf) {
    std::cerr << "    Template set: id=" << set_id
              << ", length=" << set_length
              << std::endl;
    process_template_set(set_id, set_length, buf, false);
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::process_template_set(
      uint16_t set_id,
      uint16_t set_length,
      const uint8_t* buf,
      bool is_options_set) {
    const uint8_t* cur = buf;
    const uint8_t* set_end = buf + set_length;
    const uint16_t header_length 
      = is_options_set ? kOptionsTemplateHeaderLen
                       : kTemplateHeaderLen;

    while (CHECK_POINTER_WITHIN_I(cur + header_length, cur, set_end)) {
      /* Decode template record */
      uint16_t set_id = decode_uint16(cur + 0); 
      uint16_t field_count = decode_uint16(cur + 2);
      uint16_t scope_field_count = is_options_set ? decode_uint16(cur + 4) : 0;
      
      PH_RETURN_CALLBACK_ERROR(start_template_record(set_id, field_count));
      
      cur += header_length;
      
      for (unsigned int field = 0; field < field_count; field++) {
        if (!CHECK_POINTER_WITHIN_I(cur + kFieldSpecifierLen,
                                    cur, set_end)) {
          libfc_RETURN_ERROR(recoverable, long_fieldspec,
                             "Field specifier partly outside template record", 
                             0, 0, 0, 0, cur - buf);
        }
        
        uint16_t ie_id = decode_uint16(cur + 0);
        uint16_t ie_length = decode_uint16(cur + 2);
        bool enterprise = ie_id & 0x8000;
        ie_id &= 0x7fff;
        
        uint32_t enterprise_number = 0;
        if (enterprise) {
          if (!CHECK_POINTER_WITHIN_I(cur + kFieldSpecifierLen
                                      + kEnterpriseLen, cur,
                                      set_end)) {
            libfc_RETURN_ERROR(recoverable, long_fieldspec,
                               "Field specifier partly outside template "
                               "record (enterprise)", 
                               0, 0, 0, 0, cur - buf);
          }
          enterprise_number = decode_uint32(cur + 4);
        }
        
        std::cerr << "        ";
        if (is_options_set && field < scope_field_count)
          std::cerr << "Scope field";
        else if (is_options_set)
          std::cerr << "Options field";
        else /* !is_options_set */
          std::cerr << "Field specifier";
        
        const InfoElement* ie 
          = info_model.lookupIE(enterprise_number, ie_id, ie_length);

        if (ie != 0) 
          std::cerr << ", name=" << ie->name();
        else {
          std::cerr << ( enterprise ? ", enterprise" : "" )
                    << ", ie_id=" << ie_id
                    << ", ie_length=" << ie_length;
          if (enterprise)
            std::cerr << ", enterprise_number=" << enterprise_number;
        }
        std::cerr << std::endl;

        cur += kFieldSpecifierLen + (enterprise ? kEnterpriseLen : 0);
        assert (cur <= set_end);
      }
      
      PH_RETURN_CALLBACK_ERROR(end_template_record());
    }
    libfc_RETURN_OK();
  }


  std::shared_ptr<ErrorContext> PrintContentHandler::end_template_set() {
    std::cerr << "    Template set ends" << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::start_template_record(uint16_t template_id,
                             uint16_t field_count) {
    std::cerr << "      Template record: id=" << template_id
              << ", fields=" << field_count
              << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::end_template_record() {
    std::cerr << "      Template record ends" << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::start_options_template_set(uint16_t set_id,
                                 uint16_t set_length,
                                 const uint8_t* buf) {
    std::cerr << "    Option template set: id=" << set_id
              << ", length=" << set_length
              << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::end_options_template_set() {
    std::cerr << "    Option template set ends" << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::start_options_template_record(uint16_t template_id,
                                    uint16_t field_count,
                                    uint16_t scope_field_count) {
    std::cerr << "      Option template record: id=" << template_id
              << ", fields=" << field_count
              << ", scope-fields=" << scope_field_count
              << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::end_options_template_record() {
    std::cerr << "      Option template record ends" << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::field_specifier(bool enterprise,
                       uint16_t ie_id,
                       uint16_t ie_length,
                       uint32_t enterprise_number) {
    std::cerr << "        Field specifier: id=" << ie_id
              << ", length=";
    if (ie_length == kIpfixVarlen) 
      std::cerr << "Varlen";
    else
      std::cerr << ie_length;

    if (enterprise)
      std::cerr << ", enterprise=" << enterprise_number;
    else 
      std::cerr << ", IETF IE";
    std::cerr << std::endl;
    libfc_RETURN_OK();
  }

  std::shared_ptr<ErrorContext> PrintContentHandler::start_data_set(uint16_t id, uint16_t length, const uint8_t* buf) {
    std::cerr << "    Data set: template-id=" << id
              << ", length=" << length
              << std::endl;
    libfc_RETURN_OK();
  }
  
  std::shared_ptr<ErrorContext> PrintContentHandler::end_data_set() {
    std::cerr << "    Data set ends"
              << std::endl;
    libfc_RETURN_OK();
  }

};

