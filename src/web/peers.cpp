#include "peers.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

Peer::Peer(std::string ip, int port) {
    this->ip = ip;
    this->port = port;
}

PeerFinder::PeerFinder() {}

Peer PeerFinder::parsePeerString(std::string line) {
    int delim = line.find(':');
    if (delim == std::string::npos) {
        throw std::invalid_argument("PeerFinder: input doesn't have a ':' delimiter");
    }
    std::string newIP = (line[0] == '[') ? line.substr(1,delim-2) : line.substr(0,delim);
    int newPort = std::stoi(line.substr(delim+1));
    return Peer(newIP, newPort);
}

std::vector<Peer> PeerFinder::getPeers() {
    std::vector<Peer> out;

    std::fstream nodeFile("./data/nodes.txt");
    std::string line;
    while (std::getline(nodeFile, line)) { // doesn't include \n
        try {
            Peer newPeer = parsePeerString(line);
            out.push_back(newPeer);
        } catch(std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return out;
}

PeerFinder::~PeerFinder() {}
