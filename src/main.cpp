#include "web/node.hpp"

#include <iostream>

int main() {
    FullNode fullNode;

    fullNode.run(18333); // default for testnet

    std::cout << "Input anything to end." << std::endl;
    std::string ignored;
    std::cin >> ignored;

    fullNode.end();
}
