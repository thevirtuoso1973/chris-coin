#pragma once

#include "peers.hpp"
#include "../core/block.hpp"
#include "../core/blockchain.hpp"

#include <uvw.hpp>

#include <memory>
#include <vector>
#include <mutex>


class FullNode {
  private:
    Blockchain blockchain;
    std::mutex blockchainLock;

    std::shared_ptr<uvw::Loop> loop;

    std::vector<Peer> peers; // not necessarily open/closed

  public:
    FullNode();

    void run(int portNum);
    void end();

    virtual ~FullNode();
};
