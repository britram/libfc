/* Copyright (c) 2011, NEC Europe Ltd, Consorzio Nazionale 
 * Interuniversitario per le Telecomunicazioni, Institut 
 * Telecom/Telecom Bretagne, ETH Zürich, INVEA-TECH a.s. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of NEC Europe Ltd, Consorzio Nazionale 
 *      Interuniversitario per le Telecomunicazioni, Institut Telecom/Telecom 
 *      Bretagne, ETH Zürich, INVEA-TECH a.s. nor the names of its contributors 
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
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "TCPExporter.h"

#include "exceptions/IOError.h"

namespace IPFIX {

    void TCPExporter::_sendMessage(uint8_t *base, size_t len) {
        int      rv;
        timespec ts;
        
        if (fd_ < 0) {
            fd_ = addr_.create_socket();
        }

        while ((rv = send(fd_, base, len, 0)) < 0) {
            // we failed. close.
            close(fd_);
            
            if (retry_) {
                // increase retry delay
                if (retry_delay_ * 2 <= retry_delay_max_) {
                    retry_delay_ *= 2;
                }

                // wait before retry
                ts.tv_sec = retry_delay_;
                ts.tv_nsec = 0;

                // FIXME check return, add max count, all that friendly stuff
                if (nanosleep(&ts, NULL) < 0) {
                    throw IOError("TCPExporter killed while retrying.");
                }

                // try to reconnect
                fd_ = addr_.create_socket();
                
            } else {
                throw IOError(strerror(errno));
            }            
        }
        
        // we succeeded, reset retry delay
        retry_delay_ = retry_delay_min_;
    }

    TCPExporter::~TCPExporter() {
        if (fd_ >= 0) {
            flush();
            close(fd_);
            fd_ = -1;
        }

    }

}
