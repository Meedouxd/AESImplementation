#ifndef PROJECT_1_AES_ALGORITHM_H
#define PROJECT_1_AES_ALGORITHM_H

#include "key.h"
#include "block.h"
#include <random>
#include <iostream>

template <KEY_SIZES size>
class AES_Algorithm {
private:
    Key<size> key;
    BlockConverter blockConverter;

    std::string inputFile;
    int numProcessingSteps = 0; //numProcessingSteps is the number of times that step 3 is repeated

public:
    void init() {
        std::cout << "Enter file name: " << std::endl;
        std::cin >> inputFile;
        blockConverter.convertBlocks(inputFile);
        std::cout << "Done! Printing:" << std::endl;
        std::cout << blockConverter.print();
    }

    void keyExpansion() {

    }

    void addRoundKey() {

    }

    void subBytes() {

    }

    void shiftRows() {
        char carry = 0;
    }

    void mixColumns() {
        char carry[4] = {0, 0, 0, 0};
    }

    void encrypt() {
        addRoundKey();

        for (int i = 0; i < numProcessingSteps; i++) {   //9-13 rounds depending on key size
            subBytes();
            shiftRows();
            mixColumns();
            addRoundKey();
        }

        subBytes();
        shiftRows();
        addRoundKey();
    }

    void flipRandomBit() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, size * 8 - 1);   //Between 0 and bit size - 1

        key.flipBit(distrib(gen));  //Random bit flipped
    }

    AES_Algorithm() {
        if (size == KEY_128) { numProcessingSteps = 9; }
        if (size == KEY_192) { numProcessingSteps = 11; }
        if (size == KEY_256) { numProcessingSteps = 13; }
    };
};


#endif //PROJECT_1_AES_ALGORITHM_H