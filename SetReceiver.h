/**
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * @section DESCRIPTION
 * 
 * Defines the set receiver interface.
 *
 * Once a SetReceiver is registered for a given minimal template
 * (see Collector::registerReceiver()), its receive() method will 
 * be called for each set for which it is registered in the message.
 */

// FIXME provide a SetReceiver subclass that does record-level transcoding.

#ifndef IPFIX_SETRECEIVER_H // idem
#define IPFIX_SETRECEIVER_H // hack

#include "Transcoder.h"
#include "WireTemplate.h"

namespace IPFIX {

  class Collector;
      
  class SetReceiver {
  public:    
    /**
     * Receive the next set for which this receiver is registered. 
     *
     * @param collector the collector on which this set was received
     * @param setxc a Transcoder, focused on the content of the set
     * @param wt the wire template describing the records in the set
     *
     * FIXME what else might a receiver require?
     */
    virtual void receive(const Collector* collector, 
                         Transcoder& setxc, 
                         const WireTemplate* wt) = 0;
    
  };

}
#endif // idem hack