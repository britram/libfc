/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * A TCPSingleCollector is a collector that accepts IPFIX via TCP 
 * from one connection at a time.
 *
 */

#ifndef IPFIX_TCPSINGLECOLLECTOR_H // idem
#define IPFIX_TCPSINGLECOLLECTOR_H // hack

#include <cstdio>
#include "Collector.h"
#include "NetAddress.h"

namespace IPFIX {
 
class TCPSingleCollector : public Collector {
public:
  
/**
 * Create a new TCPSingleCollector, listening on the default port, via IPv4.
 */
    TCPSingleCollector():
        Collector(),
        session_(NULL),
        netaddr_("4739", IPPROTO_TCP, AF_INET),
        lsock_(-1),
        sock_(-1)
        {}

/**
 * Create a new TCPSingleCollector, listening on a specified NetAddress.
 */
    TCPSingleCollector(const NetAddress& netaddr):
        Collector(),
        session_(NULL),
        netaddr_(netaddr),
        lsock_(-1),
        sock_(-1)
        {}


/**
* TCPSingleCollector destructor; ensures the listening and reading sockets are closed.
*/
    virtual ~TCPSingleCollector();

protected:

    virtual bool _receiveMessage(MBuf& mbuf, std::tr1::shared_ptr<Session>& session);
  
private:
    bool ensureSocket();

    Session*                         session_;
    NetAddress                       netaddr_;
    int                              lsock_;
    int                              sock_;
};

} // namespace IPFIX

#endif // idem hack