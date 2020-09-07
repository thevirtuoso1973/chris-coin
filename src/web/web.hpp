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

  public:
    FullNode();

    void run(int portNum);

    virtual ~FullNode();
};
