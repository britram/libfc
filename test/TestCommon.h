#include "FileWriter.h"
#include "FileReader.h"

static const uint32_t kTestDomain = 12345;

struct SimpleFlow {
    uint64_t flowStartMilliseconds;
    uint64_t flowEndMilliseconds;
    uint32_t sourceIPv4Address;
    uint32_t destinationIPv4Address;
    uint16_t sourceTransportPort;
    uint16_t destinationTransportPort;
    uint32_t packetDeltaCount;
};

struct CapfixPacket {
    uint64_t observationTimeMilliseconds;
    uint32_t ipTotalLength;
    VarlenField ipPacketHeaderSection;
};

void quit_handler(int signum);
void install_quit_handler();
bool didQuit();

uint64_t sys_ms();

void makeSimpleFlowTemplate(StructTemplate &sfstmpl);
void makeCapfixPacketTemplate(StructTemplate &sfstmpl);

void initSimpleFlow(SimpleFlow &sf);
void incrSimpleFlow(SimpleFlow &sf);