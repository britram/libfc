#include "TestCommon.h"

#include <signal.h>
#include <sys/time.h>
#include <cerrno>

static int quit_counter = 0;

void doQuit(int signum) {
    quit_counter++;
}

void install_quit_handler() {
    struct sigaction sa, osa;

    sa.sa_handler = doQuit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT,&sa,&osa)) {
      throw std::runtime_error(std::string("sigaction(SIGINT) failed: ") + strerror(errno));
    }

    sa.sa_handler = doQuit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGTERM,&sa,&osa)) {
      throw std::runtime_error(std::string("sigaction(SIGINT) failed: ") + strerror(errno));
    }
}

bool didQuit() {
    return (quit_counter > 0);
}

uint64_t sys_ms() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<uint64_t>(tv.tv_sec) * 1000 +  
           static_cast<uint64_t>(tv.tv_usec) / 1000;
}
void makeSimpleFlowTemplate(StructTemplate& sfstmpl) {
    InfoModel& model = InfoModel::instance();
  
    sfstmpl.add(model.lookupIE("flowStartMilliseconds"),    offsetof(SimpleFlow, flowStartMilliseconds));
    sfstmpl.add(model.lookupIE("flowEndMilliseconds"),      offsetof(SimpleFlow, flowEndMilliseconds));
    sfstmpl.add(model.lookupIE("sourceIPv4Address"),        offsetof(SimpleFlow, sourceIPv4Address));
    sfstmpl.add(model.lookupIE("destinationIPv4Address"),   offsetof(SimpleFlow, destinationIPv4Address));
    sfstmpl.add(model.lookupIE("sourceTransportPort"),      offsetof(SimpleFlow, sourceTransportPort));
    sfstmpl.add(model.lookupIE("destinationTransportPort"), offsetof(SimpleFlow, destinationTransportPort));
    sfstmpl.add(model.lookupIE("packetDeltaCount[4]"),      offsetof(SimpleFlow, packetDeltaCount));
    sfstmpl.activate();
}

void makeCapfixPacketTemplate(StructTemplate& cpstmpl) {
    InfoModel& model = InfoModel::instance();

    cpstmpl.add(model.lookupIE("observationTimeMilliseconds"), 
        offsetof(CapfixPacket, observationTimeMilliseconds));
    cpstmpl.add(model.lookupIE("ipTotalLength[4]"),
        offsetof(CapfixPacket, ipTotalLength));
    cpstmpl.add(model.lookupIE("ipHeaderPacketSection"),       
        offsetof(CapfixPacket, ipHeaderPacketSection));
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
    sf.flowStartMilliseconds = sf.flowEndMilliseconds - 1000;
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


void matchSimpleFlow(SimpleFlow& expected, SimpleFlow& actual) {
    if (expected.packetDeltaCount != actual.packetDeltaCount) {
        std::cout << "packet count mismatch expected " <<
                     expected.packetDeltaCount << " got " <<
                     actual.packetDeltaCount << std::endl;
    }
}