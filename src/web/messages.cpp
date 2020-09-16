#include "messages.hpp"

// assumes there's enough space
void writeLittleEndian(unsigned long bigEndian, char* toWriteInto) {
    while (bigEndian > 0) {
        unsigned long newValue = bigEndian & 0xFF;
        *(toWriteInto++) = newValue;

        bigEndian = bigEndian >> 8;
    }
}

void writeMagic(Magic magic, char* toWriteInto) {
    switch (magic) {
        case MAIN:
            writeLittleEndian(0xD9B4BEF9, toWriteInto);
            break;
        case TESTNET:
            writeLittleEndian(0xDAB5BFFA, toWriteInto);
            break;
        case TESTNET3:
            writeLittleEndian(0x0709110B, toWriteInto);
            break;
        case NAMECOIN:
            writeLittleEndian(0xFEB4BEF9, toWriteInto);
            break;
    }
}

char* MessageBuilder::createMessage(
    Magic magic,
    char* command,
    uint32_t length,
    uint32_t checksum,
    unsigned char* payload,
    int payload_length
) {
    char* data = new char[4+12+4+4+payload_length];
    writeMagic(magic, data);
    // TODO
    return data;
}

MessageBuilder::MessageBuilder() {}

char* MessageBuilder::getVersionMessage(
    int32_t version,
    uint64_t services,
    int64_t timestamp,
    net_addr addr_recv,
    net_addr addr_from,
    uint64_t nonce,
    std::string user_agent_string,
    int32_t start_height,
    bool relay
) {
    // TODO
    return 0;
}

char* MessageBuilder::getVerackMessage() {
    // TODO
    return 0;
}

MessageBuilder::~MessageBuilder() {}
