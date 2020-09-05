#pragma once

#include <vector>
#include <string>

class TxOutputPair {
    private:
        int value;
        std::string outAddress;

    public:
        TxOutputPair(int val, std::string address) {
            value = val;
            outAddress = address;
        }

        int getValue() {
            return value;
        }
};

// A transaction.
class Tx {
    private:
        std::vector<std::string> inputReferences;
        int totalInputValue;

        std::vector<TxOutputPair> outputs;

    public:
        Tx() {
            // TODO
        }

        int getTransactionFee() { // getTransactionFee() <= 0 implies invalid transaction
            int outputVal = 0;
            for (auto pair : outputs) {
                outputVal += pair.getValue();
            }
            return totalInputValue-outputVal;
        }
};
