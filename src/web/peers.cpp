#include "peers.hpp"

#include <uvw.hpp>

#include <memory>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <stdexcept>

Peer::Peer(std::string ip, int port) {
    this->ip = ip;
    this->port = port;
}

void Peer::init(std::shared_ptr<uvw::Loop> loop) {
    auto tcp = loop->resource<uvw::TCPHandle>();
    if (tcp) {
        tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent& event, uvw::TCPHandle& tcp) {
            // TODO: handle errors
        });
        tcp->once<uvw::ConnectEvent>([](const uvw::ConnectEvent& event, uvw::TCPHandle& tcp) {
            // TODO: connect to the peer, send version message etc.
        });
        tcp->connect(ip, port);
    } else {
        std::cerr << "TCP handle failed to init." << std::endl;
    }
}

Peer::~Peer() {}

std::ostream& operator<<(std::ostream& os, const Peer& peer) {
    os << peer.ip << " : " << peer.port;
    return os;
}

// MARK: PeerFinder
PeerFinder::PeerFinder() {}

Peer PeerFinder::parsePeerString(const std::string& line) { // NOTE: no error handling done on file format
    int delim = line.rfind(':');
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
