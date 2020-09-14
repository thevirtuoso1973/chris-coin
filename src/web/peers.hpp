#pragma once

#include <uvw.hpp>

#include <memory>
#include <ostream>
#include <string>
#include <vector>

class Peer {
    private:
        std::string ip;
        int port;

    public:
        Peer(std::string ip, int port);

        void init(std::shared_ptr<uvw::Loop> loop);

        ~Peer();
        friend std::ostream& operator<<(std::ostream& os, const Peer& peer);
};

class PeerFinder {
    private:
        // takes line without newline at the end
        Peer parsePeerString(const std::string& line);

    public:
        PeerFinder();

        std::vector<Peer> getPeers();

        ~PeerFinder();
};

