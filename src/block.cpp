#include "block.h"
#include <fstream>
#include <iostream>

std::string Block::printBlock() {   //Prints in a way that restores input text block
    std::string output;
    for (auto & row : state) {
        for (const char & col : row)
            output += col;
    }

    return output;
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

        for (auto & row : block.state) {    //Fill block
            for (char & col : row)
                inputFile.get(col);
        }
        blocks.push_back(block);
    }

    inputFile.close();
}

std::string BlockConverter::print() {   //Outputs blocks seperated by newlines and bars
    std::string output;

    for (auto & block : blocks)
        output += "|" + block.printBlock() + "|\n";

    return output;
}

Block* BlockConverter::getBlockAt(int index){
    if(index >= blocks.size()){
        std::cerr << "Error: invalid index selected" << std::endl;
        exit(1);
    }
    return &blocks.at(index);
// my cat stepped on my keyboard so Im leaving this here
//     '[
    
//     9uuuuuuuuuu
// ]]]]]]]]g9nm]'
}

char* BlockConverter::blockToBytes(int index){
    char* byteArray = new char[16]; // enter array

    if(blocks.size() >= index){
        std::cerr << "Error: invalid index selected" << std::endl;
        exit(1);
    }

    Block targetBlock = blocks.at(index);

    for(int a = 0; a < 4; a++){
        for(int b = 0; b < 4; b++){
            int currentIndex = (a*4) + b;
            byteArray[currentIndex] = targetBlock.state[a][b];
        }
    }
    return &byteArray[0];
}

int BlockConverter::getNumOfBlocks(){
    return blocks.size();
}