#include "key.h"
#include <random>
#include <cassert>

std::string Key::printKey() {
    std::string output;
    for (const unsigned char & i : key) {
        output += std::to_string(i) + " ";
    }

    return output;
}

void Key::flipBit(const int index) {
    assert(index >= 0);
    const int byteNo = index / 8;
    const int bitNo = index % 8;

    key[byteNo] ^= 1 << bitNo;
}