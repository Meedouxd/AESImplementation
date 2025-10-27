#include "block.h"
#include <fstream>
#include <iostream>

std::string Block::printBlock() const {   //Prints in a way that restores input text block
    std::string output;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++)
            output += state[j][i];
    }
    return output;
}

void Block::hexPrint() const { //prints a block
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << std::hex << static_cast<unsigned int>(state[i][j]);
            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


void BlockConverter::convertBlocks(const std::string& fileName) {
    std::ifstream inputFile;
    inputFile.open(fileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    while (!inputFile.eof()) {
        Block block;

        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                char c = 0;
                inputFile.get(c);
                block.state[j][i] = c; // fill it column by column
            }
        }
        blocks.push_back(block);
    }

    inputFile.close();
}

std::string BlockConverter::print() const {   //Outputs blocks seperated by newlines and bars
    std::string output;

    for (auto & block : blocks)
        output += "|" + block.printBlock() + "|\n";

    return output;
}

Block* BlockConverter::getBlockAt(const int index) {
    if(index >= blocks.size()) {
        std::cerr << "Error: invalid index selected" << std::endl;
        exit(1);
    }
    return &blocks.at(index);
    // my cat stepped on my keyboard so Im leaving this here
    //     '[
        
    //     9uuuuuuuuuu
    // ]]]]]]]]g9nm]'
}

int BlockConverter::getNumOfBlocks() const {
    return blocks.size();
}

void BlockConverter::writeToFile(const std::string &outputLocation) {
    std::ofstream cipherFile(outputLocation);
    for (int i = 0; i < getNumOfBlocks(); i++)
        cipherFile << (getBlockAt(i)->printBlock());

    cipherFile.close();
}

void BlockConverter::flipBitAtIndex(const int index) {
    const int blockNumber = index / 128;
    const int byteNumber = index % 16;

    const int stateColumnNumber = byteNumber % 4;
    const int stateRowNumber = byteNumber / 4;

    const int bitNumber = index % 8;
    
    // std::cout << "Index: " << index << std::endl;
    // std::cout << "Block Number: " << blockNumber << std::endl;
    // std::cout << "Byte Number: " << byteNumber << std::endl;
    // std::cout << "State Column Number: " << stateColumnNumber << std::endl;
    // std::cout << "State Row Number: " << stateRowNumber << std::endl;
    // std::cout << "Bit Number: " << bitNumber << std::endl;

    unsigned char newByte = getBlockAt(blockNumber)->state[stateColumnNumber][stateRowNumber];

    newByte ^= 1 << bitNumber; //thanks ambrose for this code snippet

    getBlockAt(blockNumber)->state[stateColumnNumber][stateRowNumber] = newByte;

}
