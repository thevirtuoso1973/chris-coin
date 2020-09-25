#include "node.hpp"
#include "peers.hpp"

#include <uvw.hpp>

#include <iostream>


FullNode::FullNode() {
    PeerFinder peerFinder;
    peers = peerFinder.getPeers();

    loop = uvw::Loop::getDefault();
}


void FullNode::listen(std::shared_ptr<uvw::TCPHandle> tcp, int port) {
    tcp->on<uvw::ListenEvent>([](const uvw::ListenEvent&, uvw::TCPHandle& srv) {
        std::clog << "A listening event has occured!" << std::endl;

        std::shared_ptr<uvw::TCPHandle> client = srv.loop().resource<uvw::TCPHandle>();
        client->on<uvw::CloseEvent>([ptr = srv.shared_from_this()](const uvw::CloseEvent &, uvw::TCPHandle &) { ptr->close(); });
        client->on<uvw::EndEvent>([](const uvw::EndEvent &, uvw::TCPHandle &client) { client.close(); });
        srv.accept(*client);
        client->read();
    });
    tcp->bind("127.0.0.1", port);
    tcp->listen();
}

void FullNode::run(int portNum) {
    if (peers.size() == 0) {
        std::cerr << "Not enough peers." << std::endl;
    }

    listen(loop->resource<uvw::TCPHandle>(), portNum);

    for (auto peer : peers) {
        std::shared_ptr<uvw::TCPHandle> tcp;
        int failcount = 0;
        while (!(tcp = loop->resource<uvw::TCPHandle>()) && failcount++ < 5);
        if (tcp) {
            peer.init(tcp);
        } else {
            std::cerr << "Failed to create tcp handle" << std::endl;
        }
    }
    loop->run<uvw::Loop::Mode::NOWAIT>();
}

void FullNode::end() {
    loop->walk(uvw::Overloaded{
        [](uvw::TCPHandle &h){ h.close(); },
        [](auto &&){/* ignored */}
    });
}

FullNode::~FullNode() {}
