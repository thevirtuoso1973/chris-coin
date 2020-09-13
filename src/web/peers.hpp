#pragma once

#include <fstream>
#include <string>
#include <vector>

class PeerFinder {
    public:
        PeerFinder();

        std::vector<std::string> getPeerSockets();

        ~PeerFinder();
};
