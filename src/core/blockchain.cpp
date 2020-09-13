#include "blockchain.hpp"

Blockchain::Blockchain() {
    // TODO add genesis block
}

std::vector<Block>::iterator Blockchain::begin() { return blockchain.begin(); }
std::vector<Block>::iterator Blockchain::end() { return blockchain.end(); }
