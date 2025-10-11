#ifndef PROJECT_1_AES_ALGORITHM_H
#define PROJECT_1_AES_ALGORITHM_H

#include "key.h"
#include "block.h"
#include <random>
#include <iostream>

template <KEY_SIZES size>   //Defined differently depending on the size of the key
class AES_Algorithm {
private:
    Key<size> key;
    BlockConverter blockConverter;

    std::string inputFile;  //Input file name
    int numProcessingSteps = 0; //numProcessingSteps is the number of times that step 3 is repeated

public:

        void shiftRows(Block b) {
        std::cout << "BEFORE SHIFTING ROWS\n";
        std::cout << b.printBlock();
        char stateCopy[4][4] = {};

        for(int i = 0; i < 4; i++){ // copying block state
            for(int j = 0; j < 4; j++){
                stateCopy[i][j] = b.state[i][j];
            }
        }

        std::cout << std::endl;

        for(int i = 1; i < 4; i++){ // for every row besides first
            for(int j = 0; j < 4; j++){
                int newpos = (j + (4-i)) % 4; // left shifting
                b.state[i][newpos] = stateCopy[i][j];
            }

        }

        std::cout << b.printBlock();

    }

    void init() {   //Gets input from file and processes it into blocks
        std::cout << "Enter file name: " << std::endl;
        std::cin >> inputFile;
        blockConverter.convertBlocks(inputFile);
        std::cout << "Done! Printing:" << std::endl;
        std::cout << blockConverter.print();
        shiftRows(blockConverter.getBlockAt(0));
    }

    // void keyExpansion() {}

    void addRoundKey() {}

    void subBytes() {}



    void mixColumns() {
        char carry[4] = {0, 0, 0, 0};
    }

    void encrypt() {    //Following AES protocols
        // addRoundKey();
        std::vector<Key<KEY_128>> expandedKeys = key.getExpandedKeys();
        for(int a = 0; a <blockConverter.blocks.size(); a++){

            for (int i = 0; i < numProcessingSteps; i++) {   //9-13 rounds depending on key size

                Key currentKey = expandedKeys.at(numProcessingSteps);
                Block currentBlock = blockConverter.blocks.at(a);

                subBytes(currentBlock);
                shiftRows(currentBlock);
                mixColumns();
                addRoundKey();
        }

        subBytes();
        shiftRows();
        addRoundKey();
        }
        
    }

    void flipRandomBit() {  //Gets a random index and flips the associated bit
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, size * 8 - 1);   //Between 0 and bit size - 1

        key.flipBit(distrib(gen));  //Random bit flipped
    }

    AES_Algorithm() {   //Default constructor
        if (size == KEY_128) { numProcessingSteps = 9; }
        if (size == KEY_192) { numProcessingSteps = 11; }
        if (size == KEY_256) { numProcessingSteps = 13; }
    };
};

#endif //PROJECT_1_AES_ALGORITHM_H
