#include "peers.hpp"

#include <fstream>
#include <string>
#include <vector>

PeerFinder::PeerFinder() {}

std::vector<std::string> PeerFinder::getPeerSockets() {
    std::vector<std::string> out;

    std::fstream nodeFile("./data/nodes.txt");
    std::string line;
    while (std::getline(nodeFile, line)) { // doesn't include \n
        out.push_back(line);
    }
    return out;
}

PeerFinder::~PeerFinder() {}
