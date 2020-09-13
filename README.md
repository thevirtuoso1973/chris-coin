# Disclaimer

This is a WIP, attempting to use this client on the bitcoin mainnet is highly inadvisable.

# ChrisCoin

This is an attempt to implement an SPV client for Bitcoin using (mostly) Satoshi's [original paper](https://bitcoincore.org/bitcoin.pdf).

# Project Goals

1. Create a node that connects to the testnet and can send/receive messages.
2. SPV - verify the validity of the blockchain.

## Constraints

Do it without using the currently existing Bitcoin Core implementation.

# Build

My build command:
`cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && make && mv compile_commands.json ../compile_commands.json`
