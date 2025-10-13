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
                // begin code that I didnt write lol
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
            // end code that I didnt write lol
            return result;
        }

        void shiftRows(Block *b) {
        std::cout << "BEFORE SHIFTING ROWS\n";
        std::cout << b->printBlock();
        char stateCopy[4][4] = {};

        for(int i = 0; i < 4; i++){ // copying block state
            for(int j = 0; j < 4; j++){
                stateCopy[i][j] = b->state[i][j];
            }
        }

        std::cout << std::endl;

        for(int i = 1; i < 4; i++){ // for every row besides first
            for(int j = 0; j < 4; j++){
                int newpos = (j + (4-i)) % 4; // left shifting
                b->state[i][newpos] = stateCopy[i][j];
            }

        }

        // std::cout << b.printBlock();

    }
    // needs testing
    void addRoundKey(Block *b, Key<KEY_128> k) {
        std::string byteStr = b->printBlock();
        std::cout << "Adding Round Key...";

        if(byteStr.length() != k.toString().length()){

            std::cout << byteStr.length() << ": " << key.printKey().length();
            std::cerr << "Invalid block or key length\n";
            exit(1);
        }

        for(int i = 0; i < byteStr.length(); i++){
            unsigned char chunk = byteStr.at(i);
            unsigned char keyByte = k.toString().at(i);
            std::cout << "Block byte: " << chunk << std::endl;
            std::cout << "Key Byte: " << keyByte << std::endl;
            chunk ^= keyByte;   // XOR round key with data
            byteStr[i] = chunk;
            std::cout << "XORED: " << chunk << std::endl;
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

        std::cout << "printing out the corresponding state:\n";
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                std::cout << std::hex << (int) b->state[i][j];
                std:: cout << " ";
            }
            std::cout << std::endl;
        }
        


    }

    void init() {   //Gets input from file and processes it into blocks
        std::cout << "Enter file name: " << std::endl;
        std::cin >> inputFile;
        blockConverter.convertBlocks(inputFile);
        std::cout << "Done! Printing:" << std::endl;
        std::cout << blockConverter.print();
        unsigned char keybits[16] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
        Key<KEY_128> testKey(keybits);
        // addRoundKey(blockConverter.getBlockAt(0), testKey);
        // subBytes(blockConverter.getBlockAt(0));
        // std::cout << blockConverter.getBlockAt(0)->printBlock() << std::endl;
        // mixColumns(blockConverter.getBlockAt(0));
        // std::cout << blockConverter.getBlockAt(0)->printBlock() << std::endl;
        // shiftRows(blockConverter.getBlockAt(0));
        // std::cout << blockConverter.getBlockAt(0)->printBlock();

        std::vector<Key<KEY_128>> expandedKeyLists = testKey.getExpandedKeys();

        std::cout << "**** KEY TESTING ****\n";
        for(Key<KEY_128> k : expandedKeyLists){
            k.printHex();
        }
    }

    // void keyExpansion() {}

    // needs testing
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





    void encrypt() {    //Following AES protocols
        // addRoundKey();
        std::vector<Key<KEY_128>> expandedKeys = key.getExpandedKeys();
        for(int a = 0; a < blockConverter.blocks.size(); a++){

            for (int i = 0; i < numProcessingSteps; i++) {   //9-13 rounds depending on key size

                Key currentKey = expandedKeys.at(numProcessingSteps);
                Block* currentBlock = blockConverter.getBlockAt(a);

                subBytes(currentBlock);
                shiftRows(currentBlock);
                mixColumns();
                addRoundKey(currentBlock, currentKey);
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
