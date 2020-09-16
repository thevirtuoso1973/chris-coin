#include "messages.hpp"

#include <bits/stdint-uintn.h>
#include <openssl/evp.h>

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

unsigned char* getDoubleHashed(const unsigned char* data, uint32_t length) {
    auto mdctx = EVP_MD_CTX_new();
    auto md = EVP_sha256();
    unsigned char* md_value = new unsigned char[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    // first hash:
    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, data, length);
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    // second hash:
    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, md_value, md_len);
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);

    EVP_MD_CTX_free(mdctx);

    return md_value;
}

char* MessageBuilder::createMessage(
    Magic magic,
    char* command,
    uint32_t payload_length,
    unsigned char* payload
) {
    char* data = new char[4+12+4+4+payload_length];
    writeMagic(magic, data); // pointer 0
    for (int write = 4, s = 0; write < 4 + 12; write++, s++) { // 4
      data[write] = command[s];
    }
    writeLittleEndian(payload_length, &data[16]); // 16
    auto digest = getDoubleHashed(payload, payload_length); // 20
    for (int write = 20, i = 0; write < 20 + 4; write++, i++) {
        data[write] = digest[i];
    }
    for (int write = 24, i = 0; write < 24 + payload_length; write++, i++) { // 24
        data[write] = payload[i];
    }
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
    char *command = (char*) "version";
    uint32_t payload_length = 4+8+8+26+26+8+user_agent_string.size()+4+1;
    unsigned char *payload = new unsigned char[payload_length];
    // TODO
    return createMessage(Magic::TESTNET3, command, payload_length, payload);
}

char* MessageBuilder::getVerackMessage() {
    // TODO
    return 0;
}

MessageBuilder::~MessageBuilder() {}
