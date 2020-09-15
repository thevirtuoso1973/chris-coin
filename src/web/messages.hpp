#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <string>
#include <sys/types.h>

enum Magic { MAIN, TESTNET, TESTNET3, NAMECOIN };

struct net_addr {
    uint32_t time;
    uint64_t services;
    char* ipAddr;
    uint16_t port;
};

class MessageBuilder {
    private:
        // helper function that wraps the header around the payload
        char* createMessage(
            Magic magic,
            char* command,
            uint32_t length,
            uint32_t checksum,
            unsigned char* payload
        );

    public:
        MessageBuilder();

        // Gets the entire message (not just the payload)
        char* getVersionMessage(
            int32_t version,
            uint64_t services,
            int64_t timestamp,
            net_addr addr_recv,
            // Fields below require version >= 106:
            net_addr addr_from,
            uint64_t nonce,
            std::string user_agent_string,
            int32_t start_height,
            // requires version >= 70001:
            bool relay
        );

        char* getVerackMessage();

        ~MessageBuilder();
};

