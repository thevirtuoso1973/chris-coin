#include "blockchain.hpp"

Blockchain::Blockchain() {
    // TODO add genesis block
}

bool Blockchain::tryAddBlock(const Block new_block, std::mutex& mutex) {
    // TODO
     return 0;
}
bool Blockchain::tryAddBlock(const crow::json::rvalue& jsonBody, std::mutex& mutex) {
    // TODO
     return 0;
}

std::vector<Block>::iterator Blockchain::begin() { return blockchain.begin(); }
std::vector<Block>::iterator Blockchain::end() { return blockchain.end(); }
