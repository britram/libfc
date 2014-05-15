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
#ifndef _LIBFC_ERROR_H_
#  define _LIBFC_ERROR_H_

#  include <string>

namespace LIBFC {

  class Error {
  public:
    enum error_t {
      /** No error occurred. */
      no_error,

      /** Input stream can't peek(). */
      input_source_cant_peek,

      /** Calling parse() while another parse is in progress. */
      parse_while_parsing,

      /** A callback decided that further processing is not needed or
       * wanted, even though the data stream is fine. This error is
       * guaranteed never to be generated by any function in this
       * library (except perhaps in ipfix.cpp, to translate from C to
       * C++). */
      aborted_by_user,

      /** A system error occurred; more details are available through
       * errno. */
      system_error,

      /** The message header is too short. */
      short_header,

      /** The message body is too short. */
      short_body,

      /** Set size exceeds message size. */
      long_set,

      /** Field specification exceeds set. */
      long_fieldspec,

      /** Version number in message header is not what it should be. */
      message_version_number,

      /** Message shorter than minimum message size */
      short_message,

      /** IPFIX messages don't have a base time. */
      ipfix_basetime,

      /** General format error. */
      format_error,
    };

    Error(error_t e);
    const std::string to_string() const;

  private:
    error_t e;
  };

} // namespace LIBFC

#endif // _LIBFC_ERROR_H_
