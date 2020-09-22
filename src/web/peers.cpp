#include "peers.hpp"
#include "messages.hpp"

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <ctime>
#include <uvw.hpp>

#include <memory>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>

Peer::Peer(std::string ip, bool isIPv6, int port) {
    this->ip = ip;
    this->isIPv6 = isIPv6;
    this->port = port;
}

void printHex(const char* toPrint, int size, int width = 16) {
    for (int i = 0; i < size; i++) {
        if (i % width == 0) {
            std::cout << std::endl;
        }
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) toPrint[i] << ' ';
    }
    std::cout << std::endl << std::dec;
}

// creates TCP handle and sets listeners
void Peer::init(std::shared_ptr<uvw::Loop> loop) {
    auto tcp = loop->resource<uvw::TCPHandle>();
    auto c_ip = &ip[0];
    auto c_isIPv6 = isIPv6;
    auto c_port = port;
    if (tcp) {
        tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent& event, uvw::TCPHandle& tcp) {
            event.what();
            tcp.close();
        });
        tcp->once<uvw::ConnectEvent>([c_ip,
                                      c_isIPv6,
                                      c_port](const uvw::ConnectEvent& event, uvw::TCPHandle& tcp) { // FIXME: connecting too the same peer
            int64_t tStamp = std::time(nullptr);
            std::clog << "Connecting to " << c_ip << " at time " << tStamp << std::endl;
            net_addr addr_recv = net_addr {
                0,
                SERVICES,
                c_ip,
                static_cast<uint16_t>(c_port),
                c_isIPv6
            };
            char localIp[] = "127.0.0.1";
            net_addr addr_from = net_addr {
                0,
                SERVICES,
                localIp,
                18333,
                c_isIPv6
            };
            std::string userAgent = ""; // NOTE: empty user agent string
            const int dataLength = 4+12+4+4 // header
                +4+8+8+26+26+8+MessageBuilder::getVarStrSize(userAgent)+4+1; // payload
               
            auto dataWrite = MessageBuilder::getVersionMessage(
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
            //printHex(dataWrite, dataLength);
            tcp.write(
                std::move(dataWrite),
                dataLength
            );
        });
        tcp->on<uvw::DataEvent>([this](const uvw::DataEvent& event, uvw::TCPHandle& tcp) {
            std::cout << this->ip << '\n' << event.data.get() << std::endl;
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
    bool isIPv6 = line[0] == '[';
    std::string newIP = (isIPv6) ? line.substr(1,delim-2) : line.substr(0,delim);
    int newPort = std::stoi(line.substr(delim+1));
    return Peer(newIP, isIPv6, newPort);
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
