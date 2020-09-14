#pragma once

#include <vector>
#include <mutex>

#include "peers.hpp"
#include "../core/block.hpp"
#include "../core/blockchain.hpp"

class FullNode {
  private:
    Blockchain blockchain;
    std::mutex blockchainLock;

    std::vector<Peer> peers; // not necessarily open/closed

  public:
    FullNode();

    void run(int portNum);

    virtual ~FullNode();
};
