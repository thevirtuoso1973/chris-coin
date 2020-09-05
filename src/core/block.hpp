#pragma once

#include <vector>
#include <string>
#include "transaction.hpp"

class Block {
    private:
        std::vector<Tx> transactions;
    public:
        // Paragraph summary of the block
        std::string getSummary() {
            // TODO
            return "";
        }
};
