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
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 */

#ifndef _FCOLD_MESSAGEBUFFER_H_
#  define _FCOLD_MESSAGEBUFFER_H_

#include <iostream>
#include <cstdlib>
#include <cstdint>

#include <sys/socket.h>

#include "InputSource.h"

namespace fcold {
    
    const size_t kMBufSz = 65536;
    const size_t kSrcNameLen = 80;
    
    class MessageBuffer : public libfc::InputSource {

    private:
        uint8_t*            buf;
        size_t              buf_sz;
        size_t              buf_len;
        bool                buf_owned;
        
        size_t              stream_pos;
        uint64_t            recv_ms;
        uint64_t            msgclk_ms;
        
        mutable const char* source_name;
        size_t              off;
        
    public:
        /**
         * Create a new, empty MessageBuffer able to store up to
         * 64k (kMBufSz) bytes. The buffer will be filled by direct access
         * to
         */
        MessageBuffer();

        /**
         * Create a new, empty MessageBuffer able to store up to sz bytes.
         *
         * @param sz maximum size of the message to be stored in the cre
         */
        MessageBuffer(size_t sz);

        /**
         * Create a new MessageBuffer around an existing buffer, optionally
         * copying or taking ownership of the buffer.
         *
         * @param buf   buffer containing a message
         * @param sz    size of buffer
         * @param own   if true, take ownership of the (dynamically 
         *              allocated) buffer; will delete[] the buffer on
         *              destruction.
         * @param copy  make a copy of the buffer; implies own.
         */
        MessageBuffer(uint8_t *buf, size_t sz, bool own, bool copy);

        /**
         * Destroy this MessageBuffer, deleting any owned internal storage.
         */
        ~MessageBuffer();

        // Direct buffer access
        
        uint8_t *bufptr() { return buf; }
        size_t bufsz() const { return buf_sz; }
        size_t buflen() const { return buf_len; }
        void setbuflen(size_t len);

        // Metadata access
        
        void set_metadata(size_t   stream_pos,
                          uint64_t recv_ms,
                          uint64_t msgclk_ms,
                          const char *name);

        size_t get_stream_pos() { return stream_pos;}
        uint64_t get_recv_ms() { return recv_ms; }
        uint64_t get_msgclk_ms() { return msgclk_ms; }
        const char* get_name() const;

        // Implementation of InputSource
        
        ssize_t read(uint8_t* buf, uint16_t len);
        ssize_t peek(uint8_t* buf, uint16_t len);
        bool resync();
        
        bool can_peek() const;
        
        /**
         * Get offset into buffer of first byte of message.
         * For error reporting; always returns zero.
         */
        size_t get_message_offset() const;
        
        /**
         * Set message offset marker to current read offset.
         * For error reporting; always returns zero.
         */
        void advance_message_offset();

    };
}

#endif /* defined(_FCOLD_MESSAGEBUFFER_H_) */
