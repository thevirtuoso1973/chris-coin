#include "node.hpp"
#include "peers.hpp"

#include <uvw.hpp>

#include <iostream>


FullNode::FullNode() {
    PeerFinder peerFinder;
    peers = peerFinder.getPeers();

    loop = uvw::Loop::getDefault();
}

void FullNode::run(int portNum) {
    if (peers.size() == 0) {
        std::cerr << "Not enough peers." << std::endl;
    }

    for (auto peer : peers) {
        peer.init(loop);
    }
    loop->run();
}

void FullNode::end() {
    loop->walk(uvw::Overloaded{
        [](uvw::TCPHandle &h){ h.close(); },
        [](auto &&){/* ignored */}
    });
}

FullNode::~FullNode() {}
