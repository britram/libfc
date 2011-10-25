#ifndef IPFIX_SETRECEIVER_H // idem
#define IPFIX_SETRECEIVER_H // hack

#include <ctime>
#include <stdexcept>
#include "IETemplate.h"
#include "Transcoder.h"

namespace IPFIX {

  class Collector;
      
  class SetReceiver {
    // collector iterates over sets, and calls a function 
    // with a xcoder pointing to the set base and the current template
    // it is up to the called entity to actually decode.
    // note that this needs restrictable xcoders to work.
  public:    
    virtual void receive(const Collector* collector, 
                         Transcoder& setxc, 
                         const WireTemplate* iet) = 0;
    
    // setreceiver may supply helpers for doing this
  };

}
#endif // idem hack