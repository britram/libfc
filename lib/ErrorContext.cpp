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

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/logger.h>
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include "ErrorContext.h"

namespace IPFIX {

  ErrorContext::ErrorContext(error_severity_t severity,
			     Error e,
			     int system_errno,
			     const char* explanation,
			     InputSource* is,
                             const uint8_t* message,
                             uint16_t size,
                             uint16_t off) 
    : severity(severity),
      e(e),
      system_errno(system_errno),
      explanation(explanation),
      is(is),
      message(0),
      size(message != 0 ? size : 0),
      off(message != 0 ? off : 0)
#ifdef _LIBFC_HAVE_LOG4CPLUS_
                      ,
      logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("logger")))
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
  {
    if (message != 0) {
      this->message = new const uint8_t[size];
      memcpy(this->message, message, size);
    }
  }

  ErrorContext::~ErrorContext() {
    if (message != 0)
      delete message;
  }

  const Error& ErrorContext::get_error() const {
    return e;
  }

  const int ErrorContext::get_system_errno() const {
    return system_errno;
  }

  const char* ErrorContext::get_explanation() const {
    return explanation;
  }

  InputSource* ErrorContext::get_input_source() const {
    return is;
  }

  void ErrorContext::set_input_source(InputSource* is) const {
    if (this->is == 0)
      this->is = is;
  }

  const uint8_t* ErrorContext::get_message() const {
    return message;
  }

  void ErrorContext::set_message(const uint8_t* message, uint16_t size) {
    if (this->message == 0) {
      this->message = new const uint8_t[size];
      memcpy(this->message, message, size);
      this->size = size;
    }
  }

  const ErrorContext::uint16_t get_offset() const {
    return off;
  }

  void ErrorContext::set_offset(uint16_t off) {
    this->off = off;
  }

} // namespace IPFIX
