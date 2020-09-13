#include "node.hpp"
#include "peers.hpp"

#include <uvw.hpp>

#include <iostream>


FullNode::FullNode() {
    PeerFinder peerFinder;
    peerSockets = peerFinder.getPeerSockets();
}

void FullNode::run(int portNum) {
    if (peerSockets.size() == 0) {
        std::cerr << "Not enough peers." << std::endl;
    }
}

FullNode::~FullNode() {}
