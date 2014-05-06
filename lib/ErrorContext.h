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
 

#ifndef _LIBFC_ERRORCONTEXT_H_
#  define _LIBFC_ERRORCONTEXT_H_

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
#    include <log4cplus/logger.h>
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#  include "Error.h"
#  include "InputSource.h"

namespace IPFIX {
  /** An error context.
   *
   * This class is used to show upper layers where in the processing
   * of a particular InputSource an error happened and what kind of
   * error it is.  For this purpose, an ErrorContext object contains
   * the error that happened, the value of errno, and the InputSource
   * in which the error occurred.
   *
   * The InputSource object also holds information about the location
   * of the error, in the form of offset counters. (Depending on the
   * type of input source, the offset may or may not be meaningful.
   * For example, for message-based input sources, such as
   * UDPInputSource, the offset of the current message makes no sense,
   * since there is no message stream.)
   */
  class ErrorContext {
  public:
    /** The severity of the error. */
    enum error_severity_t {
      /** Everything is hunky dory. */
      fine,

      /** Something is weird with the message, even though it's not an
       * error. This could be a message that purports to be from 2301
       * (because, let's face it, if we haven't replaced IPFIX by the
       * 24th century, something is VERY wrong) or a message that's
       * supposedly from January 1, 1970. */
      warning,

      /** The message has an error and must be discarded, but the next
       * message in the stream may be recoverable.  This could happen
       * if the message contained a syntax error but the stream itself
       * seems to be OK. (For example, it's entirely possible that a
       * bit or two in a large number of large files could flip simply
       * through bit rot.) */
      recoverable,

      /** The entire message stream must be discarded.  This can
       * happen when a read operation returns an operating system
       * error. */
      fatal
    };

    /** Creates an ErrorContext object.
     *
     * @param severity the error's severity
     * @param e the error that occurred
     * @param system_errno the value of errno after the error was
     *   detected (this may be zero)
     * @param is the input source in which the error was detectd.
     */
    ErrorContext(error_severity_t severity, Error e,
		 int system_errno, const char* message,
		 InputSource& is);

    /** Returns the error.
     *
     * @return the error that occurred
     */
    const Error& get_error() const;

    /** Returns the value of errno when the error occurred.
     *
     * @return the saved value of errno (might be zero)
     */
    const int get_system_errno() const;

    /** Returns the message. 
     *
     * @return the message given in the constructor
     */
    const char* get_message() const;

    /** Returns the input stream.
     *
     * @return the input stream in which the error occurred.
     */
    InputSource& get_input_stream();

  private:
    error_severity_t severity;
    Error e;
    int system_errno;
    const char* message;
    InputSource& is;

#ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger;
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
  };

} // namespace IPFIX

#endif /* _LIBFC_ERRORCONTEXT_H_ */
