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
#include <memory>

#include <arpa/inet.h>

#include "ErrorContext.h"
#include "Frontend.h"
#include "InputSource.h"

#include "decode_util.h"

namespace fcold {

    static const size_t kNetflow5Mtu = 1500;
    static const size_t kNetflow9Mtu = 1500;
    static const size_t kIpfixMtu = 65536;
    
    /** Structure for decoding IPFIX message headers */
    struct __attribute__((packed)) v10pduhdr_st {
        uint16_t    version;
        uint16_t    length;
        uint32_t    export_s;
        uint32_t    sequence;
        uint32_t    odid;
    };
    
    typedef struct v10pduhdr_st v10pduhdr_t;
    
    /** Structure for decoding NetFlow V9 PDU headers */
    struct __attribute__((packed)) v9pduhdr_st {
        uint16_t    version;
        uint16_t    count;
        uint32_t    sysuptime_ms;
        uint32_t    export_s;
        uint32_t    sequence;
        uint32_t    generator;
    };
    
    typedef struct v9pduhdr_st v9pduhdr_t;
    
    /** Structure for decoding NetFlow V9/IFPFIX set headers */
    struct __attribute__((packed)) v9sethdr_st {
        uint16_t    id;
        uint16_t    len;
    };

    typedef struct v9sethdr_st v9sethdr_t;
    
    /** Structure for decoding NetFlow V5 PDU headers */
    struct __attribute__((packed)) v5pduhdr_st {
        uint16_t    version;
        uint16_t    count;
        uint32_t    sysuptime_ms;
        uint32_t    export_s;
        uint32_t    export_ns;
        uint32_t    sequence;
        uint8_t     gentype;
        uint8_t     genid;
    };
    
    typedef struct v5sethdr_st v5sethdr_t;
    
    static int peek_next_pdu_version(libfc::InputSource *is) {
        int         rv;
        uint16_t    version;
        
        rv = is->peek(reinterpret_cast<uint8_t*>(&version), sizeof(version));
        if (rv > 0) {
            return ntohs(version);
        } else {
            return rv;
        }
    }
    
    static int peek_next_set_header(libfc::InputSource *is, uint16_t& id, uint16_t& len) {
        int             rv;
        v9sethdr_t      sethdr; // FIXME: Speculative: sethdr_t -> v9sethdr_t
        
        rv = is->peek(reinterpret_cast<uint8_t*>(&sethdr), sizeof(sethdr));
        if (rv > 0) {
            len = ntohs(sethdr.len);
            id = ntohs(sethdr.id);
        }
        return rv;
    }
    
    
    static std::shared_ptr<libfc::ErrorContext>
            next_pdu_v5(libfc::InputSource              *is,
                        std::shared_ptr<MessageBuffer>& mb)
    {
        LIBFC_RETURN_ERROR(fatal, inconsistent_state,
                           "fcold V5 deframe not yet implemented ",
                           0, is, 0, 0, 0);
    }

    static std::shared_ptr<libfc::ErrorContext>
            next_pdu_v9(libfc::InputSource              *is,
                        std::shared_ptr<MessageBuffer>& mb)
    {
        int rv;
        uint16_t setid, setlen;

        /* die if we can't peek */
        if (!is->can_peek()) {
            LIBFC_RETURN_ERROR(fatal, input_source_cant_peek,
                               "fcold needs peekable input source for V9", 0, is, 0, 0, 0);
        }
        
        /* make a new shared message buffer */
        mb = std::make_shared<MessageBuffer>(kNetflow9Mtu);
        
        uint8_t *base_pdubuf = mb->bufptr();
        uint8_t *pdubuf = base_pdubuf;
        size_t  pdumaxsz = mb->bufsz();
        
        /* Read header */
        rv = is->read(pdubuf, sizeof(v9pduhdr_t));
        
        if (rv < 0) {
            LIBFC_RETURN_ERROR(fatal, system_error,
                               "Error reading V9 PDU header",
                               errno, is, 0, 0, 0);
        } else if ((size_t)rv < sizeof(v9pduhdr_t)) {
            LIBFC_RETURN_ERROR(recoverable, short_header,
                               "Short read in V9 PDU header: expected "
                               << sizeof(v9pduhdr_t) << " , got " << rv,
                               0, is, 0, 0, 0);
        }
        
        v9pduhdr_t *v9pduhdr = (v9pduhdr_t*)pdubuf;
        pdubuf += sizeof(v9pduhdr_t);
        
        /* Check version field */
        if (ntohs(v9pduhdr->version) != 9) {
            LIBFC_RETURN_ERROR(recoverable, message_version_number,
                               "Expected PDU version "
                               << LIBFC_HEX(4) << 9
                               << ", got " << LIBFC_HEX(4)
                               << ntohs(v9pduhdr->version),
                               0, is, 0, 0, 0);
        }
        
        /* Count is useless. Read sets in a loop. */
        while (true) {
            if ((rv = peek_next_set_header(is, setid, setlen)) > 0) {
                if (setid < 256) {
                    /* Probably a next PDU header. Exit */
                    LIBFC_RETURN_OK();
                }
            } else {
                LIBFC_RETURN_ERROR(fatal, system_error,
                                   "Error peeking at V9 set header",
                                   errno, is, 0, 0, 0);
            }
            
            if (((pdubuf + setlen) - base_pdubuf) > pdumaxsz) {
              /* FIXME: the original code doesn't compile because of msglen:

                LIBFC_RETURN_ERROR(fatal, inconsistent_state,
                                   "fcold V9 deframe overran internal buffer limit of " << pdumaxsz <<
                                   "; need " << msglen - pdumaxsz << "extra",
                                   0, is, 0, 0, 0);

                I've replaced it with this:
              */
                LIBFC_RETURN_ERROR(fatal, inconsistent_state,
                                   "fcold V9 deframe overran internal buffer limit of " << pdumaxsz <<
                                   "; need some extra",
                                   0, is, 0, 0, 0);
            }
            
            rv = is->read(pdubuf, setlen);
            if (rv < 0) {
                LIBFC_RETURN_ERROR(fatal, system_error,
                                   "Error reading V9 set",
                                   errno, is, 0, 0, 0);
            } else if (rv < setlen) {
                LIBFC_RETURN_ERROR(recoverable, short_body,
                                   "Short read in V9 set: expected "
                                   << setlen << " , got " << rv,
                                   0, is, 0, 0, 0);
            }
            pdubuf += setlen;
        }
    }
    
    static std::shared_ptr<libfc::ErrorContext>
            next_pdu_v10(libfc::InputSource             *is,
                         std::shared_ptr<MessageBuffer>& mb)
    {
        int rv;

        /* make a new shared message buffer FIXME allow sizing */
        mb = std::make_shared<MessageBuffer>(kIpfixMtu);
        
        uint8_t *base_pdubuf = mb->bufptr();
        uint8_t *pdubuf = base_pdubuf;
        size_t  pdumaxsz = mb->bufsz();
        
        /* Read header */
        rv = is->read(pdubuf, sizeof(v10pduhdr_t));
        
        if (rv < 0) {
            LIBFC_RETURN_ERROR(fatal, system_error,
                               "Error reading IPFIX message header",
                               errno, is, 0, 0, 0);
        } else if ((size_t)rv < sizeof(v10pduhdr_t)) {
            LIBFC_RETURN_ERROR(recoverable, short_header,
                               "Short read in IPFIX message header: expected "
                               << sizeof(v10pduhdr_t) << " , got " << rv,
                               0, is, 0, 0, 0);
        }

        v10pduhdr_t *v10pduhdr = (v10pduhdr_t*)pdubuf;
        pdubuf += sizeof(v10pduhdr_t);
        
        /* Check header fields */
        if (ntohs(v10pduhdr->version) != 10) {
            LIBFC_RETURN_ERROR(recoverable, message_version_number,
                               "Expected IPFIX message version "
                               << LIBFC_HEX(4) << 10
                               << ", got " << LIBFC_HEX(4)
                               << ntohs(v10pduhdr->version),
                               0, is, 0, 0, 0);
        }

        size_t msglen = ntohs(v10pduhdr->length);
                               
        if (msglen < sizeof(v10pduhdr_t)) {
           LIBFC_RETURN_ERROR(recoverable, short_message,
                              "IPFIX message length " << msglen << " too short",
                              0, is, 0, 0, 0);
        }
                               
        if (msglen > pdumaxsz) {
            LIBFC_RETURN_ERROR(fatal, inconsistent_state,
                               "fcold IPFIX deframe overran internal buffer limit of " << pdumaxsz <<
                               "; need " << msglen - pdumaxsz << "extra",
                               0, is, 0, 0, 0);
        }

        /* Read and return the rest of the message */
        rv = is->read(pdubuf, msglen - sizeof(v10pduhdr_t));
        
        if (rv < 0) {
            LIBFC_RETURN_ERROR(fatal, system_error,
                               "Error reading IPFIX message body",
                               errno, is, 0, 0, 0);
        } else if (static_cast<size_t>(rv) < msglen - sizeof(v10pduhdr_t)) {
            /* We must have rv >= 0, so the cast to size_t is safe 
             * --neuhaust */
            LIBFC_RETURN_ERROR(recoverable, short_body,
                               "Short read in IPFIX message body: expected "
                               << msglen - sizeof(v10pduhdr_t) << " , got " << rv,
                               0, is, 0, 0, 0);
        }

        mb->setbuflen(msglen);

                               
        LIBFC_RETURN_OK();
    }
    
  Frontend::Frontend(ImpFactory&         nimpfact,
                     libfc::InputSource* nis,
                     int                 npdu_version):
      impfact(nimpfact),
      is(nis),
      pdu_version(npdu_version)
    {
    }
    
    Frontend::~Frontend() {
        if (is) delete is;
    }

    std::shared_ptr<libfc::ErrorContext>
            Frontend::deframe_next(std::shared_ptr<MessageBuffer>& mb)
    {
        if (!pdu_version) {
            if (!is->can_peek()) {
                LIBFC_RETURN_ERROR(fatal, input_source_cant_peek,
                                   "fcold can't peek at next PDU version", 0, is, 0, 0, 0);
            }
            
            pdu_version = peek_next_pdu_version(is);
        }
        
        switch (pdu_version) {
            case 5:
                return next_pdu_v5(is, mb);
            case 9:
                return next_pdu_v9(is, mb);
            case 10:
                return next_pdu_v10(is, mb);
            default:
                LIBFC_RETURN_ERROR(recoverable, message_version_number,
                                   "Expected V5, V9, or IPFIX message header, got"
                                    << LIBFC_HEX(4) << pdu_version, 0, is, 0, 0, 0);
        }
    }

} // namespace fcold
