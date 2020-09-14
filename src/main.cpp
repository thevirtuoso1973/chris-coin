#include "web/node.hpp"

#include <iostream>

int main() {
    FullNode fullNode;

    fullNode.run(8333);

    std::cout << "Input anything to end." << std::endl;
    std::string ignored;
    std::cin >> ignored;

    fullNode.end();
}
