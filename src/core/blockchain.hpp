#pragma once

#include <vector>
#include "block.hpp"

class Blockchain {
    private:
        std::vector<Block> blockchain;
        bool isValidNewBlock(const Block& new_block);

    public:
        Blockchain();

        std::vector<Block>::iterator begin();
        std::vector<Block>::iterator end();
};
