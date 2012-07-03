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
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * A UDPCollector is a collector that accepts IPFIX via UDP.
 *
 * IPFIX over UDP is inherently unreliable and slightly dangerous, but
 * it seems to be the most widely implemented transport protocol. 
 *
 */

#ifndef IPFIX_UDPCOLLECTOR_H // idem
#define IPFIX_UDPCOLLECTOR_H // hack

#include <cstdio>
#include "Collector.h"
#include "NetAddress.h"

namespace IPFIX {
 
class UDPCollector : public Collector {
public:
  
/**
 * Create a new UDPCollector, listening on the default port, via IPv4.
 */
    UDPCollector():
        Collector(),
        session_(new Session()),
        netaddr_("4739", IPPROTO_UDP, AF_INET),
        sock_(-1)
        {}

/**
 * Create a new UDPCollector, listening on a specified NetAddress.
 */
    UDPCollector(const NetAddress& netaddr):
        Collector(),
        session_(new Session()),
        netaddr_(netaddr),
        sock_(-1)
        {}


/**
* UDPCollector destructor; ensures the socket is closed.
*/
    virtual ~UDPCollector();

protected:

    virtual bool _receiveMessage(MBuf& mbuf, std::shared_ptr<Session>& session);
  
private:
    bool ensureSocket();

    std::shared_ptr<Session>         session_;
    NetAddress      netaddr_;
    int             sock_;
};

} // namespace IPFIX

#endif // idem hack