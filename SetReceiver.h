#ifndef IPFIX_SETRECEIVER_H // idem
#define IPFIX_SETRECEIVER_H // hack

#include <ctime>
#include <stdexcept>
#include "IETemplate.h"
#include "XCoder.h"

namespace IPFIX {

  class Collector;
  
  typedef std::tr1::shared_ptr<class SetReceiver> SetReceiver_SP;
    
  class SetReceiver {
    // collector iterates over sets, and calls a function 
    // with a xcoder pointing to the set base and the current template
    // it is up to the called entity to actually decode.
    // note that this needs restrictable xcoders to work.
  public:    
    virtual void receive(const Collector* collector, 
                         XCoder& setxc, 
                         const WireTemplate* iet) = 0;
    
    // setreceiver may supply helpers for doing this
  };

}
#endif // idem hack