#pragma once

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <string>
#include <sys/types.h>

const static int32_t VERSION = 70015;
const static uint64_t SERVICES = 0;
const static int32_t HEIGHT = 0;
const static bool RELAY = false;

enum Magic { MAIN, TESTNET, TESTNET3, NAMECOIN };

struct net_addr {
    const uint32_t time;
    const uint64_t services;
    const char* ipAddr;
    const uint16_t port;
    const bool isIPv6;
};

class MessageBuilder {
    private:
        MessageBuilder();

    public:
        template<typename T>
        static void writeLittleEndian(T bigEndian, char* toWriteInto);

        template<typename T>
        static void writeBigEndian(T bigEndian, char* toWriteInto);

        static void writeMagic(Magic magic, char* toWriteInto);
        static void writeVarString(const std::string s, char *toWrite);
        static void writeIPAddr(const char* ipAddr, bool isIPv6, char* toWrite) ;

        // Helper function that wraps the header around the payload. Will generate the checksum.
        static char* createMessage(
            Magic magic,
            const char* command,
            uint32_t payload_length,
            char* payload
        );

        // Gets the entire message (not just the payload)
        static char* getVersionMessage(
            int32_t version,
            uint64_t services,
            int64_t timestamp,
            net_addr addr_recv,
            // Fields below require version >= 106:
            net_addr addr_from,
            const uint64_t nonce,
            std::string user_agent_string,
            int32_t start_height,
            // requires version >= 70001:
            bool relay
        ) ;
        static char* getVerackMessage();

        // gets number of bytes including the var_int
        static int getVarStrSize(std::string s);
};
