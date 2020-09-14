#pragma once

#include <string>
#include <vector>

class Peer {
    private:
        std::string ip;
        int port;
    public:
        Peer(std::string ip, int port);
        ~Peer();
};

class PeerFinder {
    private:
        // takes line without newline at the end
        Peer parsePeerString(std::string line);

    public:
        PeerFinder();

        std::vector<Peer> getPeers();

        ~PeerFinder();
};

