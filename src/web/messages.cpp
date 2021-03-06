#include "messages.hpp"

#include <bits/stdint-uintn.h>
#include <openssl/evp.h>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

// assumes there's enough space, sizeof(T) should be the number of bytes to write
template<typename T>
void MessageBuilder::writeLittleEndian(T bigEndian, char* toWriteInto) {
    const int numBytes = sizeof(T);
    for (int i = 0; i < numBytes; i++) {
        uint8_t newValue = bigEndian & 0xFF;
        *(toWriteInto++) = newValue;

        bigEndian = bigEndian >> 8;
    }
}

// assumes there's enough space, sizeof(T) should be the number of bytes to write
template<typename T>
void MessageBuilder::writeBigEndian(T bigEndian, char* toWriteInto) {
    int numBytes = sizeof(T);
    for (int i = 0; i < numBytes; i++) {
        uint8_t newValue = (bigEndian >> 8*(numBytes-i-1)) & 0xFF;
        *(toWriteInto++) = newValue;
    }
}

void MessageBuilder::writeMagic(Magic magic, char* toWriteInto) {
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
    const char* command,
    uint32_t payload_length,
    const char* payload
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
    delete payload;
    return data;
}

MessageBuilder::MessageBuilder() {}

std::vector<std::string> split(std::string s, char delim) {
    std::vector<std::string> out;
    int currStart = 0;
    int delimPos;
    while (currStart < s.size() && (delimPos = s.find(delim, currStart)) != std::string::npos) {
        out.push_back((currStart < delimPos) ? s.substr(currStart, delimPos-currStart) : "");
        currStart = delimPos+1;
    }
    if (currStart < s.size()) {
        out.push_back(s.substr(currStart));
    }
    return out;
}

std::vector<uint8_t> getIPv6Bytes(const std::string ipAddr) { // FIXME: doesn't support all IPv6 formats
    std::vector<uint8_t> out;
    std::vector<std::string> hexStrings = split(ipAddr, ':');
    for (auto s : hexStrings) {
        uint8_t temp;
        std::stringstream ss;

        ss << std::hex << s.substr(0,2);
        ss >> temp;
        out.push_back(temp);
        ss << std::hex << s.substr(2);
        ss >> temp;
        out.push_back(temp);
    }
    return out;
}

std::vector<uint8_t> getIPv4Bytes(const std::string ipAddr) {
    std::vector<uint8_t> out;
    std::vector<std::string> bytestrings = split(ipAddr, '.');
    for (auto s : bytestrings) {
        out.push_back(std::stoi(s));
    }
    return out;
}

void MessageBuilder::writeIPAddr(const char* ipAddr, bool isIPv6, char* toWrite) {
    if (isIPv6) {
        auto bytes = getIPv6Bytes(ipAddr);
        assert(bytes.size() == 16 && "Expecting 16 bytes for IPv6");
        for (int i = 0; i < 16; i++) {
            *(toWrite++) = bytes[0];
        }
    } else {
        // 10 empty bytes followed by FF FF and then the 4 bytes of IP
        for (int i = 0; i < 10; i++) {
            *(toWrite++) = 0x00;
        }
        *(toWrite++) = 0xFF;
        *(toWrite++) = 0xFF;
        auto bytes = getIPv4Bytes(ipAddr);
        assert(bytes.size() == 4 && "Expecting 4 bytes for IPv4");
        for (int i = 0; i < 4; i++) {
            *(toWrite++) = bytes[i];
        }
    }
}

// writes var_int and then the char array.
void MessageBuilder::writeVarString(const std::string s, char *toWrite) {
    int sLength = s.size();
    if (sLength < 0xFD) {
        uint8_t len = sLength;
        *(toWrite++) = len;
    } else if (sLength <= 0xFFFF) {
        uint16_t len = sLength;
        *(toWrite++) = 0xFD;
        writeLittleEndian(len, toWrite);
        toWrite += 2;
    } else if (sLength <= 0xFFFFFFFF) {
        uint32_t len = sLength;
        *(toWrite++) = 0xFE;
        writeLittleEndian(len, toWrite);
        toWrite += 4;
    } else {
        uint64_t len = sLength;
        *(toWrite++) = 0xFF;
        writeLittleEndian(len, toWrite);
        toWrite += 8;
    }
    // write the actual string:
    for (int i = 0; i < sLength; i++) {
        *(toWrite++) = s[0];
    }
}

int MessageBuilder::getVarStrSize(std::string s) {
    int intLen;
    if (s.size() < 0xFD) {
        intLen = 1;
    } else if (s.size() <= 0xFFFF) {
        intLen = 3;
    } else if (s.size() <= 0xFFFFFFFF) {
        intLen = 5;
    } else {
        intLen = 9;
    }
    return intLen+s.size();
}

char* MessageBuilder::createVersionPayload(
    int32_t version,
    uint64_t services,
    int64_t timestamp,
    net_addr addr_recv,
    net_addr addr_from,
    const uint64_t nonce,
    std::string user_agent_string,
    int32_t start_height,
    bool relay
) {
    int varStrLen = getVarStrSize(user_agent_string);
    uint32_t payload_length = 4+8+8+26+26+8+varStrLen+4+1;
    char *payload = new char[payload_length];
    writeLittleEndian(version, payload);
    writeLittleEndian(services, payload+4);
    writeLittleEndian(timestamp, payload+12);

    // addr_recv:
    writeLittleEndian(addr_recv.services, payload+20);
    writeIPAddr(addr_recv.ipAddr, addr_recv.isIPv6, payload+28);
    writeBigEndian(addr_recv.port, payload+44); // network byte order is big endian
    // addr_from:
    writeLittleEndian(addr_from.services, payload+46);
    writeIPAddr(addr_from.ipAddr, addr_recv.isIPv6, payload+54);
    writeBigEndian(addr_from.port, payload+70);

    writeLittleEndian(nonce, payload+72);
    writeVarString(user_agent_string, payload + 80);
    writeLittleEndian(start_height, payload+80+varStrLen);
    uint8_t relayBool = (relay) ? 0x01 : 0x00;
    writeBigEndian(relayBool, payload+80+varStrLen+4);
    return payload;
}

char* MessageBuilder::getVersionMessage(
    int32_t version,
    uint64_t services,
    int64_t timestamp,
    net_addr addr_recv,
    net_addr addr_from,
    const uint64_t nonce,
    std::string user_agent_string,
    int32_t start_height,
    bool relay
) {
    const char *command = "version\0\0\0\0";
    const char* payload = createVersionPayload(
        version,
        services,
        timestamp,
        addr_recv,
        addr_from,
        nonce,
        user_agent_string,
        start_height,
        relay
    );
    int varStrLen = getVarStrSize(user_agent_string);
    uint32_t payload_length = 4+8+8+26+26+8+varStrLen+4+1;
    return createMessage(Magic::TESTNET3, command, payload_length, std::move(payload));
}

char* MessageBuilder::getVerackMessage() {
    const char *command = "verack\0\0\0\0\0";
    return createMessage(Magic::TESTNET3, command, 0, nullptr);
}
