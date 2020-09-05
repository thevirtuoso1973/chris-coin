#pragma once

#include <vector>
#include "../core/block.hpp"
#include "crow_all.h"

class FullNode {
    private:
      std::vector<Block> blockchain;
      crow::SimpleApp app;

    public:
        FullNode();

        void run(int portNum);

        virtual ~FullNode();
};
