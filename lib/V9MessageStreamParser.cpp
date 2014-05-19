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
      message_size = kV9MessageHeaderLen;
      
      /* Take care when changing message from an array to a pointer. */
      assert(cur + kV9SetHeaderLen <= message + sizeof(message));
      cur = message + message_size;

      errno = 0;
      nbytes = is.peek(cur, kV9SetHeaderLen);

      unsigned int set_no = 1;
      uint16_t current_set_id = 0;

      while (nbytes > 0 && static_cast<size_t>(nbytes) == kV9SetHeaderLen) {
	current_set_id = decode_uint16(cur + 0);
	if (current_set_id == kV9Version)
	  break;
	else if (current_set_id == kV5Version) {
	  LOG4CPLUS_WARN(logger, "V5 set ID encountered in V9 stream "
			 << is.get_name());
	  break;
	}

	/* Please leave this assert in. It *ought* to be always true,
	 * and in thie case, the compiler should be able to optimize
	 * it away. */
	assert(kV9SetLenOffset + sizeof(uint16_t) <= kV9SetHeaderLen);
	uint16_t set_length = decode_uint16(cur + kV9SetLenOffset);

	/* Take care when changing message from an array to a pointer. */
	if (cur + set_length > message + sizeof(message))
	  LIBFC_RETURN_ERROR(recoverable, long_set, 
			     "While scanning V9 message, set size " 
			     << set_length << " exceeds message space",
			     0, &is, message, cur - message,
			     message_size);
	  
	/* Take care when changing message from an array to a pointer. */
	assert(cur + set_length <= message + sizeof(message));
	errno = 0;
	ssize_t read_bytes = is.read(cur, set_length);
	
	if (read_bytes != set_length)
	  LIBFC_RETURN_ERROR(recoverable, short_body, 
			     "While scanning V9 message, wanted " 
			     << set_length << " bytes for set, got " 
			     << read_bytes,
			     errno, &is, message, cur - message,
			     message_size);

	assert(read_bytes == set_length);
	message_size += set_length;
	cur = message + message_size;

	/* Take care when changing message from an array to a pointer. */
	assert(cur + kV9SetHeaderLen <= message + sizeof(message));
	errno = 0;
	nbytes = is.peek(cur, kV9SetHeaderLen);

	set_no++;
      }

      if (nbytes < 0) 
	LIBFC_RETURN_ERROR(fatal, system_error, "read error", errno,
			   &is, message, 0, 0);

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

      /* This assert should be true since message_size is a uint16_t
       * and message is a static buffer of size 65535. But beware if
       * you change message to a pointer. */
      assert(message_size <= sizeof(message));
      const uint8_t* message_end = message + message_size;

      /* Now the message is read. Start over again, this time decoding sets. */
      offset = kV9MessageHeaderLen;
      cur = message + kV9MessageHeaderLen;
      assert (cur <= message_end);
      
      /* Decode sets.
       *
       * If you don't like the pointer comparisons using <=, please
       * read the corresponding comment in IPFIXMessageStreamParser.cpp.
       */
      set_no = 1;
      while (cur + kV9SetHeaderLen <= message_end) {
        /* Decode set header. */
        uint16_t set_id = decode_uint16(cur + 0);
        uint16_t set_length = decode_uint16(cur + 2);
        const uint8_t* set_end = cur + set_length;
        
        if (set_end > message_end)
	  LIBFC_RETURN_ERROR(recoverable, long_set, 
			     "Long set: set_len=" << set_length 
			     << ",set_end=" << static_cast<const void*>(set_end) 
			     << ",message_len=" << message_size
			     << ",message_end=" << static_cast<const void*>(message_end),
			     0, &is, message, message_size, offset);

        cur += kV9SetHeaderLen;

        if (set_id == kV9TemplateSetID) {
	  LIBFC_RETURN_CALLBACK_ERROR(
	    start_template_set(
              set_id, set_length - kV9SetHeaderLen, cur));
	  cur += set_length - kV9SetHeaderLen;
	  LIBFC_RETURN_CALLBACK_ERROR(end_template_set());
        } else if (set_id == kV9OptionTemplateSetID) {
	  LIBFC_RETURN_CALLBACK_ERROR(
            start_options_template_set(
              set_id, set_length - kV9SetHeaderLen, cur));
          cur += set_length - kV9SetHeaderLen;
	  LIBFC_RETURN_CALLBACK_ERROR(
            end_options_template_set());
        } else if (set_id >= kV9MinDataSetId) {
          LIBFC_RETURN_CALLBACK_ERROR(
            start_data_set(
              set_id, set_length - kV9SetHeaderLen, cur));
          cur += set_length - kV9SetHeaderLen;
	  LIBFC_RETURN_CALLBACK_ERROR(end_data_set());
        } else
	  LIBFC_RETURN_ERROR(recoverable, format_error,
			     "Set has ID " << set_id << ", which is not "
			     "a V9 template, options template or data set ID",
			     0, &is, message, message_size, offset);

        assert(cur == set_end);
        assert(cur <= message_end);

	set_no++;
      }

      LOG4CPLUS_TRACE(logger, "Got " << (set_no - 1) << " sets");

      LIBFC_RETURN_CALLBACK_ERROR(end_message());

      offset += nbytes;
      memset(message, '\0', sizeof(message));
      nbytes = is.read(message, kV9MessageHeaderLen);
    }


    /* This is important, don't remove it!  Otherwise, if
     * end_session() gives an error, message_size bytes may be copied
     * from a (now non-existent) message. */
    message_size = 0;
    memset(message, '\0', sizeof(message));

    LIBFC_RETURN_CALLBACK_ERROR(end_session());

    LIBFC_RETURN_OK();
  }

} // namespace LIBFC
