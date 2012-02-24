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
        netaddr_("4739", IPPROTO_UDP, AF_INET),
        sock_(-1)
        {}

/**
 * Create a new UDPCollector, listening on a specified NetAddress.
 */
    UDPCollector(const NetAddress& netaddr):
        Collector(),
        netaddr_(netaddr),
        sock_(-1)
        {}


/**
* UDPCollector destructor; ensures the socket is closed.
*/
    virtual ~UDPCollector();

protected:

    virtual bool _receiveMessage(MBuf& mbuf, std::tr1::shared_ptr<Session>& session);
  
private:
    NetAddress      netaddr_;
    int             sock_;
};

} // namespace IPFIX

#endif // idem hack