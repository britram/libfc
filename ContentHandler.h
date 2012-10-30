/* Hi Emacs, please use -*- mode: C++; -*- */
/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
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
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
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

#ifndef IPFIX_CONTENTHANDLER_H
#  define IPFIX_CONTENTHANDLER_H

#  include <cstdint>

namespace IPFIX {

  /** Abstract base class for classes that handle IPFIX messages. 
   * 
   * This API was heavily based on, not to say stolen, from SAX; see
   * http://sax.sourceforge.net/
   */
  class ContentHandler {
  public:
    /** Receives notification at the start of a session.
     *
     * This method will be invoked only once, before any other
     * callbacks.  It will be invoked even if the first attempt to
     * read from the message source causes an unrecoverable error.
     */
    virtual void start_session() = 0;

    /** Receives notification of the end of a session. 
     *
     * This method will be invoked at most once, and if it is, it will
     * be the last method invoked during the parse.  The method will
     * not be invoked if parsing this message has been abandoned due
     * to a nonrecoverable error. 
     */
    virtual void end_session() = 0;

    /** Receives notification that a new message has started.
     *
     * This callback reports the message header as per RFC 5101.
     *
     * @param version the version number in the header
     * @param length overall length of message in octets
     * @param export_time seconds since Jan 1, 1970 when this header
     *   left the exporter. 
     * @param sequence_number sequence number as per RFC 5101
     * @param observation_domain observation domain as per RFC 5101
     */
    virtual void start_message(uint16_t version,
                               uint16_t length,
                               uint32_t export_time,
                               uint32_t sequence_number,
                               uint32_t observation_domain) = 0;

    /** Receives notification of the end of a message. 
     *
     * This method will be invoked at most once per message, and if it
     * is, it will be the last method invoked during the parse of this
     * message.  The method will not be invoked if parsing this
     * message has been abandoned due to a nonrecoverable error.
     */
    virtual void end_message() = 0;

    /** Receives notification that a new template set begins.
     *
     * @param set_id set ID as per RFC 5101 (should be 2)
     * @param set_length overall set length as per RFC 5101
     */
    virtual void start_template_set(uint16_t set_id, uint16_t set_length) = 0;

    /** Receives notification that a template set ends. */
    virtual void end_template_set() = 0;

    /** Receives notification that a template record is starting.
     *
     * @param template_id template ID as per RFC 5101
     * @param field_count number of fields in this template record
     */
    virtual void start_template_record(uint16_t template_id,
                                       uint16_t field_count) = 0;

    /** Receives notification that a template record ends. */
    virtual void end_template_record() = 0;

    /** Receives notification that a new option template set begins.
     *
     * @param set_id set ID as per RFC 5101 (should be 3)
     * @param set_length overall set length as per RFC 5101
     */
    virtual void start_option_template_set(uint16_t set_id,
                                           uint16_t set_length) = 0;

    /** Receives notification that an option template set ends. */
    virtual void end_option_template_set() = 0;

    /** Receives notification that an option template record is starting.
     *
     * @param template_id option template ID as per RFC 5101
     * @param field_count number of fields in this template record
     * @param scope_field_count number of scope fields as per RFC 5101
     */
    virtual void start_option_template_record(uint16_t template_id,
                                              uint16_t field_count,
                                              uint16_t scope_field_count) = 0;

    /** Receives notification that an option template record ends. */
    virtual void end_option_template_record() = 0;

    /** Receives notification of a field specifier.
     *
     * @param enterprise true if this an enterprise-specific
     *     information element, false if it is defined by the IETF
     * @param ie_id information element identifier as per RFC 5102
     * @param ie_length length of wire representation of this
     *     information element as per RFC 5102
     * @param enterprise_number IANA enterprise number as per the IANA
     *     Private Enterprise Number Registry. This value is only
     *     meaningful if the enterprise parameter is true.
     */
    virtual void field_specifier(bool enterprise,
                                 uint16_t ie_id,
                                 uint16_t ie_length,
                                 uint32_t enterprise_number) = 0;

    /** Receives notification that a data set is available.
     *
     * @param id set id (= template ID) as per RFC 5101, > 255
     * @param length length in octets of the data records in this set
     *     (excluding the header)
     * @param buf pointer to the beginning of the data records
     *     (excluding the header)
     */
    virtual void start_data_set(uint16_t id,
                                uint16_t length,
                                const uint8_t* buf) = 0;

    /** Receives notification that a data set has ended. */
    virtual void end_data_set() = 0;
  };

} // namespace IPFIX

#endif // IPFIX_CONTENTHANDLER_H
