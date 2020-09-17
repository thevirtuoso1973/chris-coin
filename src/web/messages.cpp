#include "messages.hpp"

#include <bits/stdint-uintn.h>
#include <openssl/evp.h>

// assumes there's enough space, sizeof(T) should be the number of bytes to write
template<typename T>
void writeLittleEndian(T bigEndian, char* toWriteInto) {
    int numBytes = sizeof(T);
    while (numBytes-- > 0) {
        uint8_t newValue = bigEndian & 0xFF;
        *(toWriteInto++) = newValue;

        bigEndian = bigEndian >> 8;
    }
}

void writeMagic(Magic magic, char* toWriteInto) {
    // declaring these explicitly since we need the 'uint32_t' type
    const uint32_t main = 0xD9B4BEF9;
    const uint32_t testnet = 0xDAB5BFFA;
    const uint32_t testnet3 = 0x0709110B;
    const uint32_t namecoin = 0xFEB4BEF9;

    switch (magic) {
        case MAIN:
            writeLittleEndian(main, toWriteInto);
            break;
        case TESTNET:
            writeLittleEndian(testnet, toWriteInto);
            break;
        case TESTNET3:
            writeLittleEndian(testnet3, toWriteInto);
            break;
        case NAMECOIN:
            writeLittleEndian(namecoin, toWriteInto);
            break;
    }
}

unsigned char* getDoubleHashed(const char* data, uint32_t length) {
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
    char* payload
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
    char *payload = new char[payload_length];
    writeLittleEndian(version, payload);
    writeLittleEndian(services, payload+4);
    writeLittleEndian(timestamp, payload+12);
    return createMessage(Magic::TESTNET3, command, payload_length, payload);
}

char* MessageBuilder::getVerackMessage() {
    // TODO
    return 0;
}

MessageBuilder::~MessageBuilder() {}
