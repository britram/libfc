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

#ifndef _libfc_CONTENTHANDLER_H_
#  define _libfc_CONTENTHANDLER_H_

#  include <cstdint>
#  include <memory>

#  include <ErrorContext.h>

namespace libfc {

  /** Abstract base class for classes that handle IPFIX messages. 
   * 
   * This API was heavily based on, not to say stolen, from SAX; see
   * http://sax.sourceforge.net/
   */
  class ContentHandler {
  public:
    virtual ~ContentHandler() = 0;

    /** Receives notification at the start of a session.
     *
     * This method will be invoked only once, before any other
     * callbacks.  It will be invoked even if the first attempt to
     * read from the message source causes an unrecoverable error.
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> start_session() = 0;

    /** Receives notification of the end of a session. 
     *
     * This method will be invoked at most once, and if it is, it will
     * be the last method invoked during the parse.  The method will
     * not be invoked if parsing this message has been abandoned due
     * to a nonrecoverable error. 
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> end_session() = 0;

    /** Receives notification that a new message has started.
     *
     * This callback reports properties of the message, some taken
     * directly from the message header, some computed from it.
     *
     * @param version the version number in the header
     * @param length overall length of message in bytes
     * @param export_time seconds since Jan 1, 1970 when this header
     *   left the exporter. 
     * @param sequence_number sequence number as per RFC 5101
     * @param observation_domain observation domain as per RFC 5101
     * @param base_time number of milliseconds since Jan 1, 1970 until
     *   the exporter was last (re)started.  This makes sense only for
     *   NetFlow v5 and NetFlow v9; for IPFIX, this must be zero.
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> start_message(uint16_t version,
                               uint16_t length,
                               uint32_t export_time,
                               uint32_t sequence_number,
                               uint32_t observation_domain,
                               uint64_t base_time) = 0;

    /** Receives notification of the end of a message. 
     *
     * This method will be invoked at most once per message, and if it
     * is, it will be the last method invoked during the parse of this
     * message.  The method will not be invoked if parsing this
     * message has been abandoned due to a nonrecoverable error.
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> end_message() = 0;

    /** Receives notification that a new template set begins.
     *
     * @param set_id set ID as per RFC 5101 (should be 2)
     * @param set_length set length (excluding header) in bytes
     * @param buf pointer to the buffer containing the template set
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> start_template_set(
        uint16_t set_id,
        uint16_t set_length,
        const uint8_t* buf) = 0;

    /** Receives notification that a template set ends.
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> end_template_set() = 0;

    /** Receives notification that a new option template set begins.
     *
     * @param set_id set ID as per RFC 5101 (should be 3)
     * @param set_length set length (excluding header) in bytes
     * @param buf pointer to the buffer containing the template set
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> start_options_template_set(
        uint16_t set_id,
        uint16_t set_length,
        const uint8_t* buf) = 0;

    /** Receives notification that an option template set ends. 
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> end_options_template_set() = 0;

    /** Receives notification that a data set is available.
     *
     * @param id set id (= template ID) as per RFC 5101, > 255
     * @param length length in bytes of the data records in this set
     *     (excluding the header)
     * @param buf pointer to the beginning of the data records
     *     (excluding the header)
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> start_data_set(
        uint16_t id,
        uint16_t length,
        const uint8_t* buf) = 0;

    /** Receives notification that a data set has ended. 
     *
     * @return a (shared) pointer to an error context, or null if no
     * error occurred
     */
    virtual std::shared_ptr<ErrorContext> end_data_set() = 0;
  };

} // namespace libfc

#endif // _libfc_CONTENTHANDLER_H_
