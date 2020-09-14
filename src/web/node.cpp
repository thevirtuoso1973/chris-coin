#include "node.hpp"
#include "peers.hpp"

#include <uvw.hpp>

#include <iostream>


FullNode::FullNode() {
    PeerFinder peerFinder;
    peers = peerFinder.getPeers();
}

void FullNode::run(int portNum) {
    if (peers.size() == 0) {
        std::cerr << "Not enough peers." << std::endl;
    }
}

FullNode::~FullNode() {}
