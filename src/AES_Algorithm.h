#ifndef PROJECT_1_AES_ALGORITHM_H
#define PROJECT_1_AES_ALGORITHM_H

#include "key.h"
#include "block.h"
#include <random>
#include <iostream>
#include <fstream>

template <KEY_SIZES size>   //Defined differently depending on the size of the key
class AES_Algorithm {
private:
    int numProcessingSteps = 0; //numProcessingSteps is the number of times that step 3 is repeated

public:

    void subBytes(Block* b) {
        unsigned char copyState[4][4] = {};

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                copyState[i][j] = b->state[i][j];
            }
        }  

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    b->state[i][j] = Key<KEY_128>::substitutionBox[copyState[i][j]];
                }
            }
        }

        unsigned char getResultantValue(unsigned char* stateRow, unsigned char* mixMatrixColumn){
            unsigned char result = 0;

            for(int i = 0; i < 4; i++){
                unsigned char multiplier = mixMatrixColumn[i];
                unsigned char stateByte = stateRow[i];

                if(multiplier < 1 || multiplier > 3){
                    std::cerr << "Error: mix matrix must have values between 1 and 3\n";
                    exit(1);
                }

                unsigned int partialSum = 0;
                // https://www.tutorialspoint.com/cryptography/cryptography_mixcolumns_transformation.htm
                for (int i = 0; i < 8; i++) {
                    if ((stateByte & 1) != 0) {
                        partialSum ^= multiplier;
                    }
                    int hiBitSet = multiplier & 0x80;
                    multiplier <<= 1;
                    if (hiBitSet != 0) {
                        multiplier ^= 0x1B;
                    }
                    stateByte = stateByte >> 1;
                }
                result ^= partialSum;
            }
            return result;
        }

    void shiftRows(Block *b) {
        char stateCopy[4][4] = {};

        for(int i = 0; i < 4; i++){ // copying block state
            for(int j = 0; j < 4; j++){
                stateCopy[i][j] = b->state[i][j];
            }
        }

        for(int i = 0; i < 4; i++){ // for every row besides first
            for(int j = 0; j < 4; j++){
                int newpos = (j + (4-i)) % 4; // left shifting
                b->state[i][newpos] = stateCopy[i][j];
            }

        }

    }

    void addRoundKey(Block *b, Key<KEY_128> k) {

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                b->state[i][j] ^= k.toString().at((j*4)+i);
            }
        }

    }

    // tested :D
    void mixColumns(Block* b) {
        char carry[4] = {0, 0, 0, 0};

        unsigned char copyState[4][4] = {};

        
        for(int i = 0; i < 4; i++){ // copy original state matrix
            for(int j = 0; j < 4; j++){
                copyState[i][j] = b->state[i][j];
            }
        }

        unsigned char mixMatrix[4][4] = {2, 3, 1, 1,
                                         1, 2, 3, 1,
                                         1, 1, 2, 3,
                                         3, 1, 1, 2};
        int resultantIndex = 0; 
        for(int i = 0; i < 4; i++){
            unsigned char stateRow[4] = {copyState[0][i], copyState[1][i], copyState[2][i], copyState[3][i]};
            for(int j = 0; j < 4; j++){
                unsigned char resultantValue = 0;
                unsigned char mixMatrixColumn[4] = {mixMatrix[j][0], mixMatrix[j][1], mixMatrix[j][2], mixMatrix[j][3]};

                resultantValue = getResultantValue(stateRow, mixMatrixColumn);

                int rowIndex = resultantIndex % 4;
                int columnIndex = resultantIndex / 4;
                b->state[rowIndex][columnIndex] = resultantValue;
                resultantIndex++;
            }
        }
    }

    BlockConverter encrypt(BlockConverter dataBlocks, Key<KEY_128> k) {    //Following AES protocols
        

        std::vector<Key<KEY_128>> expandedKeys = Key<KEY_128>::getExpandedKeys(k);

        for(int a = 0; a < dataBlocks.getNumOfBlocks(); a++){

            Block* currentBlock = dataBlocks.getBlockAt(a);

            // std::cout << "Current Block:\n";
            // currentBlock->hexPrint();

            // std::cout << "Current Key:\n";

            // expandedKeys.at(0).printHex();

            addRoundKey(currentBlock, expandedKeys.at(0)); // 0th round only add round key
            // std::cout << "AFter adding round key:\n";
            // currentBlock->hexPrint();
            for (int i = 1; i <= numProcessingSteps; i++) {   //9-13 rounds depending on key size
                // std::cout << "Round " << i << std::endl;
                Key currentKey = expandedKeys.at(i);
                

                // std::cout << "Current Block:\n";
                // currentBlock->hexPrint();

                // std::cout << "Current Key:\n";

                // currentKey.printHex();

                subBytes(currentBlock);
                // std::cout << "AFter sub bytes:\n";
                // currentBlock->hexPrint();

                shiftRows(currentBlock);
                // std::cout << "AFter shifting rows:\n";
                // currentBlock->hexPrint();

                mixColumns(currentBlock);
                // std::cout << "AFter mixing columns:\n";
                // currentBlock->hexPrint();

                addRoundKey(currentBlock, currentKey);
                // std::cout << "AFter adding round key:\n";
                // currentBlock->hexPrint();
        }
        // std::cout << "Round " << numProcessingSteps+1 << " (Last Round)\n";
        // last round skips mix columns
        subBytes(currentBlock);
        // std::cout << "AFter sub bytes:\n";
        // currentBlock->hexPrint();

        shiftRows(currentBlock);
        // std::cout << "AFter shifting rows:\n";
        // currentBlock->hexPrint();

        addRoundKey(currentBlock, expandedKeys.at(10));
        // std::cout << "AFter adding round key:\n";
        // currentBlock->hexPrint();
        }
        return dataBlocks;
        
    }

    AES_Algorithm() {   //Default constructor
        if (size == KEY_128) { numProcessingSteps = 9; }
        if (size == KEY_192) { numProcessingSteps = 11; }
        if (size == KEY_256) { numProcessingSteps = 13; }
    };

      void init() {   //Gets input from file and processes it into blocks
        // std::cout << "Enter file name: " << std::endl;
        // std::cin >> inputFile;
        // blockConverter.convertBlocks(inputFile);

        // unsigned char keybits[16] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
        // key = Key<KEY_128>(keybits);        
    }
};

#endif //PROJECT_1_AES_ALGORITHM_H
