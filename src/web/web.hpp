#pragma once

#include <vector>
#include <mutex>
#include "crow_all.h"

#include "../core/block.hpp"
#include "../core/blockchain.hpp"

class FullNode {
  private:
    Blockchain blockchain;
    std::mutex blockchainLock;

    crow::SimpleApp app;
    std::vector<std::string> peerIPs;

  public:
    FullNode();

    void run(int portNum);

    virtual ~FullNode();
};
