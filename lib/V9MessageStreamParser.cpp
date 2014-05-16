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
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <sstream>

#include "Constants.h"
#include "V9MessageStreamParser.h"

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#  include <log4cplus/logger.h>
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include "decode_util.h"

namespace LIBFC {

  V9MessageStreamParser::V9MessageStreamParser() 
    : offset(0)
#ifdef _LIBFC_HAVE_LOG4CPLUS_
               ,
    logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("V9MessageStreamParser")))
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
 {
  }

  std::shared_ptr<ErrorContext>
  V9MessageStreamParser::parse(InputSource& is) {
    LOG4CPLUS_TRACE(logger, "ENTER parse()");

    /* Use assert() instead of error handler since this must (and
     * will) be caught in testing. */
    assert(content_handler != 0);

    /* Why do we need this? Read the long comment below and find out. */
    if (!is.can_peek())
      LIBFC_RETURN_ERROR(fatal, input_source_cant_peek, 
			 "V9 messages can only be parsed with "
			 "peekable input streams", 0, &is, 0, 0, 0);

    /* I would normally declare the message_size further down, but
     * it's needed for the expansion of LIBFC_RETURN_CALLBACK_ERROR. */
    uint16_t message_size = 0;

    LIBFC_RETURN_CALLBACK_ERROR(start_session());

    /* Take care when changing message from an array to a pointer. */
    memset(message, '\0', sizeof(message));

    /* Member `offset' initialised here as well as in the constructor
     * so that you know it's not forgotten. */
    offset = 0;

    /** The number of bytes available after the latest read operation,
     * or -1 if a read error occurred. */
    errno = 0;
    ssize_t nbytes = is.read(message, kV9MessageHeaderLen);

    while (nbytes > 0) {
      uint8_t* cur = message;

      /* Decode message header */
      if (static_cast<size_t>(nbytes) < kV9MessageHeaderLen) {
	LIBFC_RETURN_ERROR(recoverable, short_header, 
			   "Wanted " 
			   << kV9MessageHeaderLen
			   << " bytes for V9 message header, got only "
			   << nbytes,
			   0, &is, message, nbytes, 0);
      }
      assert(static_cast<size_t>(nbytes) == kV9MessageHeaderLen);

      /* Via Brian and demux_statdat.c: the v9 format does not have
       * the message size (in bytes) in the header, but rather the
       * number of records.  Since records are in sets and since we
       * can only see the set headers but not count the records
       * without actually going through them one by one, this record
       * count is totally useless.
       *
       * So, in order JUST to get the message size, we need to iterate
       * over the message, set by set, stopping only when we see the
       * next message header, or EOF.  Don't you like v9 already?
       */
      message_size = 0;
      
      /* Take care when changing message from an array to a pointer. */
      assert(cur + kV9SetHeaderLen <= message + sizeof(message));
      nbytes = is.peek(cur, kV9SetHeaderLen);

      unsigned int set_no = 1;

      while (nbytes == kV9SetHeaderLen) {
	LOG4CPLUS_TRACE(logger, "Set number " << set_no);

	uint16_t id = decode_uint16(cur + 0);
	if (id == kV9Version || id == kV5Version)
	  break;

	/* Please leave this assert in. It *ought* to be always true,
	 * and in thie case, the compiler should be able to optimize
	 * it away. */
	assert(kV9SetLenOffset + sizeof(uint16_t) <= kV9SetHeaderLen);
	uint16_t set_length = decode_uint16(cur + kV9SetLenOffset);

	LOG4CPLUS_TRACE(logger, "Is a set of size " << set_length);

	/* Take care when changing message from an array to a pointer. */
	if (cur + set_length > message + sizeof(message))
	  LIBFC_RETURN_ERROR(recoverable, long_set, 
			     "While scanning V9 message, set size " 
			     << set_length << " exceeds message space",
			     0, &is, message, cur - message,
			     message_size);
	  
	/* Take care when changing message from an array to a pointer. */
	assert(cur + set_length <= message + sizeof(message));
	ssize_t read_bytes = is.read(cur, set_length);
	
	if (read_bytes != set_length)
	  LIBFC_RETURN_ERROR(recoverable, short_body, 
			     "While scanning V9 message, wanted " 
			     << set_length << " bytes for set, got " 
			     << read_bytes,
			     errno, &is, message, cur - message,
			     message_size);

	assert(read_bytes == set_length);
	cur += set_length;
	message_size += set_length;

	/* Take care when changing message from an array to a pointer. */
	assert(cur + kV9SetHeaderLen <= message + sizeof(message));
	nbytes = is.peek(cur, kV9SetHeaderLen);

	set_no++;
      }

      /* Basetime computation as per email from Brian:
       *
       * (2) The header in general is different, crucially containing
       * information from which a basetime (router start time) can be
       * derived, since the timestamps in the message are all relative
       * to the basetime. The uncorrected basetime in epoch
       * milliseconds is given by:
       *
       *   uint64_t basetime_ms = (uint64_t)ntohl(hdr->export_s) * 1000 
       *     - ntohl(hdr->sysuptime_ms);
       */
      LIBFC_RETURN_CALLBACK_ERROR(
        start_message(decode_uint16(message +  0),
		      message_size,
		      decode_uint32(message +  8),
		      decode_uint32(message + 12),
		      decode_uint32(message + 16),
		      static_cast<uint64_t>(decode_uint32(message + 8))*1000 
		      - static_cast<uint64_t>(decode_uint32(message + 4))));
      
      const uint8_t* message_end = message + message_size;

      /* Start over again, this time decoding sets. */
      cur = message + kV9MessageHeaderLen;
      assert (cur <= message_end);

      offset += kIpfixMessageHeaderLen;

      errno = 0;
      nbytes = is.read(cur, message_size - kIpfixMessageHeaderLen);
      if (nbytes < 0) {
        LIBFC_RETURN_ERROR(fatal, system_error, 
			   "Wanted to read " 
			   << message_size - kIpfixMessageHeaderLen
			   << " bytes, got a read error", errno, &is,
			   message, message_size, offset);
      } else if (static_cast<size_t>(nbytes) 
                 != message_size - kIpfixMessageHeaderLen) {
        LIBFC_RETURN_ERROR(recoverable, short_body, 
			   "Wanted " << message_size - kIpfixMessageHeaderLen
			   << " bytes for message body, got " << nbytes,
			   0, &is, message, message_size, offset);
      }
      
      /* Decode sets.
       *
       * Note to prospective debuggers of the code below: I am aware
       * that the various comparisons of pointers to message
       * boundaries with "<=" instead of "<" look wrong.  After all,
       * we all write "while (p < end) p++;". But, gentle reader,
       * please be assured that these comparisons have all been
       * meticulously checked and found to be correct.  There are two
       * reasons for the use of "<=" over "<":
       *
       * (1) In one case, I check whether there are still N bytes left
       * in the buffer. In this case, if "end" points to just beyond
       * the buffer boundary, "cur + N <= end" is the correct
       * comparison. (Think about it.)
       *
       * (2) In the other case, I check that "cur" hasn't been
       * incremented to the point where it's already beyond the end of
       * the buffer, but where it's OK if it's just one byte past
       * (because that will be checked on the next iteration
       * anyway). In this case too, "cur <= end" is the correct test.
       *
       * -- Stephan Neuhaus
       */
      while (cur + kIpfixSetHeaderLen <= message_end) {
        /* Decode set header. */
        uint16_t set_id = decode_uint16(cur + 0);
        uint16_t set_length = decode_uint16(cur + 2);
        const uint8_t* set_end = cur + set_length;
        
        if (set_end > message_end) {
          std::stringstream sstr;
          sstr << "set_len=" << set_length 
               << ",set_end=" << static_cast<const void*>(set_end) 
               << ",message_len=" << message_size
               << ",message_end=" << static_cast<const void*>(message_end);
	  LIBFC_RETURN_ERROR(recoverable, long_set, 
			     "Long set: set_len=" << set_length 
			     << ",set_end=" << static_cast<const void*>(set_end) 
			     << ",message_len=" << message_size
			     << ",message_end=" << static_cast<const void*>(message_end),
			     0, &is, message, message_size, offset);
        }

        cur += kIpfixSetHeaderLen;

        if (set_id == kIpfixTemplateSetID) {
	  LIBFC_RETURN_CALLBACK_ERROR(
	    start_template_set(
              set_id, set_length - kIpfixSetHeaderLen, cur));
	  cur += set_length - kIpfixSetHeaderLen;
	  LIBFC_RETURN_CALLBACK_ERROR(end_template_set());
        } else if (set_id == kIpfixOptionTemplateSetID) {
	  LIBFC_RETURN_CALLBACK_ERROR(
            start_options_template_set(
              set_id, set_length - kIpfixSetHeaderLen, cur));
          cur += set_length - kIpfixSetHeaderLen;
	  LIBFC_RETURN_CALLBACK_ERROR(
            end_options_template_set());
        } else {
          LIBFC_RETURN_CALLBACK_ERROR(
            start_data_set(
              set_id, set_length - kIpfixSetHeaderLen, cur));
          cur += set_length - kIpfixSetHeaderLen;
	  LIBFC_RETURN_CALLBACK_ERROR(end_data_set());
        }

        assert(cur == set_end);
        assert(cur <= message_end);
      }

      LIBFC_RETURN_CALLBACK_ERROR(end_message());

      offset += nbytes;
      memset(message, '\0', sizeof(message));
      errno = 0;
      nbytes = is.read(message, kIpfixMessageHeaderLen);
    }

    if (nbytes < 0) {
        LIBFC_RETURN_ERROR(fatal, system_error, 
			   "Wanted to read " 
			   << kIpfixMessageHeaderLen
			   << " bytes, got a read error", errno, &is,
			   0, 0, 0);
    }
    assert(nbytes == 0);

    /* This is important, don't remove it!  Otherwise, if
     * end_session() gives an error, message_size bytes may be copied
     * from a (now non-existent) message. */
    message_size = 0;
    memset(message, '\0', sizeof(message));

    LIBFC_RETURN_CALLBACK_ERROR(end_session());

    LIBFC_RETURN_OK();
  }

} // namespace LIBFC
