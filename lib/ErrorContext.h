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

  /** Returns a shared pointer to an ErrorContext object.
   *
   * Initialising and returning such objects is (a) tedious, and (b)
   * always the same.  This macro takes away some of the pain.
   *
   * @param severity the severity as per ErrorContext::error_severity_t
   * @param error the error as per Error::error_t
   * @param message_stream an error message, assemblee as a stream
   * @param system_errno the value of errno, or 0
   * @param is the input stream in which the error was detected
   */
#  define LIBFC_RETURN_ERROR(severity, error, message_stream,         \
			     system_errno, is, message, size, off)    \
    do {                                                              \
      std::stringstream ss;                                           \
                                                                      \
      ss << message_stream;                                           \
      return std::shared_ptr<ErrorContext>(                           \
        new ErrorContext(ErrorContext::severity, Error(Error::error), \
                         system_errno, ss.str().c_str(), is, message, \
			 size, off));				      \
    } while (0)

  /** Returns an ErrorContext signaling success. */
#  define LIBFC_RETURN_OK()			\
    do {					\
      return std::shared_ptr<ErrorContext>(0);	\
    } while (0)

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
   *
   * Many of the functions that parse messages return a pointer to an
   * ErrorContext.  That pointer is non-0 if an error occurred, and 0
   * otherwise.  The ContentHandler methods are called from a
   * MessageStreamParser object's parse() function that in turn
   * returns such a pointer.  The idea is that all functions where
   * potential errors may occur return an ErrorContext pointer.  This
   * then causes the caller to stop what it's doing, potentially
   * augmenting the ErrorContext object with additional information
   * and passing it to the caller's caller and so on, right up to the
   * initial caller of parse().
   *
   * If a method detects an error itself (as opposed to calling a
   * method that returns a non-0 ErrorContext pointer), you should use
   * the LIBFC_RETURN_ERROR macro somewhat like this (see the
   * documentation for error_severity_t below and Error::error_t in
   * the Error class):
   *
   * @code
   * uint8_t* message = ...;
   * uint16_t size = ...;
   * uint16_t off = ...;
   *
   * if (some_error_condition) {
   *   LIBFC_RETURN_ERROR(recoverable, short_header, 
   *                     "Expected " << kMessageHeaderSize 
   *                                 << " bytes in header, got "
   *                                 << nbytes, 
   *                      system_errno, is, message, size, off);
   * }
   * @endcode
   *
   * If you call a function that returns a non-zero ErrorContext
   * pointer, you should do something like this:
   *
   * @code
   * std::shared_ptr<ErrorContext> p = func(message + offset);
   * if (p != 0) {
   *   p->set_message(message, size);
   *   p->set_offset(p->get_offset() + offset);
   *   return p;
   * }
   * @endcode
   *
   * In this example, we assumed that func() didn't have access to the
   * entire message, so we augmented the error context object and
   * adjusted the offset.
   *
   * It is expected that occurrences like these willl be rare, so it's
   * perfectly OK if the code in the error handler is somewhat slow.
   * Don't try to write clever code, @em{especially} not in error
   * handling.
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
     * @param explanation a message further identifying and/or explaining
     *   the error
     * @param is the input source in which the error was
     *   detected. It's OK if this parameter is 0; the InputSource can
     *   be set later.
     * @param message the message that caused the error. The
     *   constructor will make a copy of the message. It's OK if this
     *   parameter is 0.  The message can be set later.
     * @param size the size of the message. If the message parameter
     *   is 0, this value will be ignored.
     * @param off the offset at which the error was detected. This
     *   value will not be ignored, even is message is 0.  The reason
     *   is that offsets will be built gradually, until the
     *   ErrorContext has percolated upward through the stack to the
     *   point where the message is wholly visible.
     */
    ErrorContext(error_severity_t severity, Error e,
		 int system_errno, const char* explanation,
		 InputSource* is, const uint8_t* message,
                 uint16_t size, uint16_t off);

    /** Don't copy ErrorContext-s. */
    ErrorContext(const ErrorContext& rhs) = delete;

    /** Don't assign ErrorContext-s. */
    ErrorContext& operator=(const ErrorContext& rhs) = delete;

    /** Destructor. */
    ~ErrorContext();

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

    /** Returns the explanation. 
     *
     * @return the explanation given in the constructor
     */
    const char* get_explanation() const;

    /** Returns the input stream.
     *
     * @return the input stream in which the error occurred.  May
     *   return 0.
     */
    InputSource* get_input_source() const;

    /** Sets the input source.
     *
     * This method has an effect only if the input source is not
     * already set. 
     *
     * @param is the new input source
     */
    void set_input_source(InputSource* is);

    /** Returns the message that caused the error.
     *
     * This returns a pointer to the beginning of a copy of the
     * message that caused the error. If the message has not yet been
     * set, this method may well return 0.  The size of the message
     * thus returned can be inquired with get_size().
     *
     * @return a pointer to the beginning of the erroneous message
     */
    const uint8_t* get_message() const;

    /** Sets the message that caused the error.
     *
     * This method causes a copy of the argument to be made, so it's
     * safe to delete the message argument after constructing an
     * ErrorContext object or to let the buffer go out of scope.
     *
     * This method has no effect if the message has already been set,
     * either through the constructor or through a previous call to
     * set_message().
     *
     * @param message the message that caused the error
     * @param size the size of the message
     */
    void set_message(const uint8_t* message, uint16_t size);

    /** Gets the offset into the message at which the error occurred.
     *
     * @return offset into the message at which the error occurred
     */
    const uint16_t get_offset() const;

    /** Sets the offset into the message at which the error occurred.
     *
     * @param off offset into the message at which the error occurred
     */
    void set_offset(uint16_t off);

  private:
    error_severity_t severity;
    Error e;
    int system_errno;
    const char* explanation;
    InputSource* is;

    const uint8_t* message;
    uint16_t size;
    uint16_t off;

#ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger;
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
  };

} // namespace IPFIX

#endif /* _LIBFC_ERRORCONTEXT_H_ */
