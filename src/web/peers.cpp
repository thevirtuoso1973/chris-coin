#include "peers.hpp"

#include <bits/stdint-intn.h>
#include <ctime>
#include <uvw.hpp>

#include <memory>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <stdexcept>

Peer::Peer(std::string ip, int port) {
    this->messageBuilder = MessageBuilder();
    this->ip = ip;
    this->port = port;
}

// creates TCP handle and sets listeners
void Peer::init(std::shared_ptr<uvw::Loop> loop) {
    auto tcp = loop->resource<uvw::TCPHandle>();
    if (tcp) {
        tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent& event, uvw::TCPHandle& tcp) {
            event.what();
            tcp.close();
        });
        tcp->once<uvw::ConnectEvent>([this](const uvw::ConnectEvent& event, uvw::TCPHandle& tcp) {
            std::clog << "Connecting to " << this << std::endl;
            int64_t tStamp = std::time(nullptr);
            net_addr addr_recv = net_addr {0, SERVICES, &this->ip[0], static_cast<uint16_t>(this->port)};
            char localIp[] = "127.0.0.1";
            net_addr addr_from = net_addr {0, SERVICES, localIp, 18333};
            std::string userAgent = ""; // NOTE: empty user agent string
            auto dataWrite = this->messageBuilder.getVersionMessage(
                VERSION,
                SERVICES,
                tStamp,
                addr_recv,
                addr_from,
                std::rand(),
                userAgent,
                HEIGHT,
                false
            );
            tcp.write(
                std::move(dataWrite),
                4+8+8+26+26+8+userAgent.size()+1+4+1 // number of bytes
            );
        });
        tcp->on<uvw::DataEvent>([this](const uvw::DataEvent& event, uvw::TCPHandle& tcp) {
            std::clog << this << ' ' << event.data.get() << std::endl;
            // TODO
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
