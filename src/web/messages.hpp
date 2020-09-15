#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
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
        // Gets the entire message (not just the payload)
        char* getVersionMessage(
            int32_t version,
            uint64_t services,
            int64_t timestamp,
            net_addr addr_recv
            // Fields below require version >= 106
            // TODO
        );

        MessageBuilder();
        ~MessageBuilder();
};

