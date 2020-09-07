#include "web.hpp"

#include <mutex>

FullNode::FullNode() {}

void FullNode::run(int portNum) {
    Blockchain& tBlockchain = blockchain;
    std::mutex& tBlockchainLock = blockchainLock;
    auto& peers = peerIPs;
    CROW_ROUTE(app, "/")
        ([&tBlockchain, &tBlockchainLock] { // HACK: why can't I capture private variables directly?
            std::string outString = "";
            const std::lock_guard<std::mutex> lock{tBlockchainLock};
            for (auto block : tBlockchain) {
                outString += block.getSummary();
            }
            return outString;
        });
    CROW_ROUTE(app, "/new_block")
        .methods("POST"_method)([&tBlockchain, &tBlockchainLock](const crow::request &req) {
            auto body = crow::json::load(req.body);
            if (body and tBlockchain.tryAddBlock(body, tBlockchainLock)) {
                return crow::response{"Added block."};
            }
            return crow::response(400);
        });
    CROW_ROUTE(app, "/new_tx")
        .methods("POST"_method)([&peers](const crow::request &req) {
            // TODO: If !seen transaction, then propagate.
            for (auto peerString : peers) {
            }
            return crow::response(400);
        });
    app.port(portNum).multithreaded().run();
}

FullNode::~FullNode() {}
