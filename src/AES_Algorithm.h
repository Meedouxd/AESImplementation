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
            std::cout << "Get resultant value called\n";

            for(int i = 0; i < 4; i++){
                unsigned char multiplier = mixMatrixColumn[i];
                unsigned char stateByte = stateRow[i];

                std::cout << "multiplier: " << std::hex << (int) multiplier << std::endl;
                std::cout << "stateByte: " << stateByte << std::endl;

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
        unsigned char keybits[16] = {1,1,1,1,
            1,1,1,1,
            1,1,1,1,
            1,1,1,1};
        Key<KEY_128> testKey(keybits);
        // addRoundKey(blockConverter.getBlockAt(0), testKey);
        // subBytes(blockConverter.getBlockAt(0));
        std::cout << blockConverter.getBlockAt(0)->printBlock() << std::endl;
        mixColumns(blockConverter.getBlockAt(0));
        // std::cout << blockConverter.getBlockAt(0)->printBlock() << std::endl;
        // shiftRows(blockConverter.getBlockAt(0));
        // std::cout << blockConverter.getBlockAt(0)->printBlock();
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


        unsigned char substitutionBox[256] = {
   0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
   0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
   0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
   0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
   0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
   0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
   0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
   0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
   0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
   0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
   0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
   0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
   0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
   0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
   0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
   0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
            };    

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    std::cout << "Was " << copyState[i][j] << std::endl;
                    b->state[i][j] = substitutionBox[copyState[i][j]];
                    std::cout << "Now: " << b->state[i][j] << std::endl;
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
