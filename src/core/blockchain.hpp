#pragma once

#include <vector>
#include "block.hpp"
#include "crow_all.h"

class Blockchain {
    private:
        std::vector<Block> blockchain;
        bool isValidNewBlock(const Block& new_block);

    public:
        Blockchain();

        // returns true if valid and added
        bool tryAddBlock(const Block new_block, std::mutex& mutex);
        bool tryAddBlock(const crow::json::rvalue& jsonBody, std::mutex& mutex);

        std::vector<Block>::iterator begin();
        std::vector<Block>::iterator end();
};
