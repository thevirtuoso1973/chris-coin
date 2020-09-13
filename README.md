# Disclaimer

This is a WIP, attempting to connect this node to the bitcoin mainnet is highly inadvisable.

# ChrisCoin

This is an attempt to implement an SPV node for Bitcoin using Satoshi's [original paper](https://bitcoincore.org/bitcoin.pdf) and the
protocol documentation from the [wiki](https://en.bitcoin.it/wiki/Protocol_documentation#Block_Headers).

# Project Goals

1. Create a node that connects to the testnet and can send/receive messages.
2. SPV - verify the validity of the blockchain.

## Constraints

Do it without using the currently existing Bitcoin Core implementation.

# Build

My build command:
`cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && make && mv compile_commands.json ../compile_commands.json`
