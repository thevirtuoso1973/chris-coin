#include "messages.hpp"

char* MessageBuilder::createMessage(
    Magic magic,
    char* command,
    uint32_t length,
    uint32_t checksum,
    unsigned char* payload
) {
    return 0; // TODO
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
