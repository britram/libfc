#include "TestCommon.h"

#include <signal.h>

static int quit_counter = 0;

void quit_handler(int signum) {
    quit_counter++;
}

void install_quit_handler() {
    struct sigaction sa, osa;

    sa.sa_handler = quit_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT,&sa,&osa)) {
      throw std::runtime_error("sigaction(SIGINT) failed: "+strerror(errno));
    }

    sa.sa_handler = quit_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGTERM,&sa,&osa)) {
      throw std::runtime_error("sigaction(SIGINT) failed: "+strerror(errno));
    }
}

bool didQuit() {
    return quit > 0;
}

uint64_t sys_ms() {
    timeval tv;
    gettimeofday(&tv);
    return static_cast<uint64_t>(tv.tv_sec) * 1000 +  
           static_cast<uint64_t>(tv.tv_usec) / 1000;
}

void makeSimpleFlowTemplate(StructTemplate &sfstmpl){
    sfstmpl.add(model_->lookupIE("flowStartMilliseconds"),    
        offsetof(SimpleFlow, flowStartMilliseconds));
    sfstmpl.add(model_->lookupIE("flowEndMilliseconds"),      
        offsetof(SimpleFlow, flowEndMilliseconds));
    sfstmpl.add(model_->lookupIE("sourceIPv4Address"),        
        offsetof(SimpleFlow, sourceIPv4Address));
    sfstmpl.add(model_->lookupIE("destinationIPv4Address"),   
        offsetof(SimpleFlow, destinationIPv4Address));
    sfstmpl.add(model_->lookupIE("sourceTransportPort"),      
        offsetof(SimpleFlow, sourceTransportPort));
    sfstmpl.add(model_->lookupIE("destinationTransportPort"), 
        offsetof(SimpleFlow, destinationTransportPort));
    sfstmpl.add(model_->lookupIE("packetDeltaCount[4]"),      
        offsetof(SimpleFlow, packetDeltaCount));
    sfstmpl.activate();
}


void makeCapfixPacketTemplate(StructTemplate &cpstmpl){
    cpstmpl.add(model_->lookupIE("observationTimeMilliseconds"), 
        offsetof(CapfixPacket, observationTimeMilliseconds));
    cpstmpl.add(model_->lookupIE("ipTotalLength"),               
        offsetof(CapfixPacket, ipTotalLength));
    cpstmpl.add(model_->lookupIE("ipPacketHeaderSection"),       
        offsetof(CapfixPacket, ipPacketHeaderSection));
    cpstmpl.activate();
}


void initSimpleFlow(SimpleFlow &sf) {
    sf.sourceIPv4Address = 0x0AFFFFFF;
    sf.destinationIPv4Address = 0x0A000000;
    sf.sourceTransportPort = 0;
    sf.destinationTransportPort = 32768;
    sf.packetDeltaCount = 0;
}

void incrSimpleFlow(SimpleFlow &sf) {
    sf.flowEndMilliseconds = sys_ms();
    sf.flowStartMilliseconds = sf.flowStartMilliseconds - 1000;
    --sf.sourceIPv4Address;
    if (sf.sourceIPv4Address < 0x0AFF0000) 
        sf.sourceIPv4Address = 0x0AFFFFFF;
    ++sf.destinationIPv4Address;
    if (sf.destinationIPv4Address > 0x0A00FFFF) 
        sf.destinationIPv4Address = 0x0A000000;
    ++sf.sourceTransportPort;
    ++sf.destinationTransportPort;
    ++sf.packetDeltaCount;
}
