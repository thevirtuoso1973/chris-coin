#pragma once

#include <vector>
#include <mutex>

#include "../core/block.hpp"
#include "../core/blockchain.hpp"

class FullNode {
  private:
    Blockchain blockchain;
    std::mutex blockchainLock;

    std::vector<std::string> peerSockets;

  public:
    FullNode();

    void run(int portNum);

    virtual ~FullNode();
};
