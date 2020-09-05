#include "web.hpp"

FullNode::FullNode() {}

void FullNode::run(int portNum) {
    auto tBlockchain = blockchain;
    CROW_ROUTE(app, "/")
        ([tBlockchain] { // HACK: why can't I capture blockchain?
            std::string outString = "";
            for (auto block : tBlockchain) {
                outString += block.getSummary();
            }
            return outString;
        });
    app.port(portNum).multithreaded().run();
}

FullNode::~FullNode() {}
