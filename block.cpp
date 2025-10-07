#include "block.h"
#include <fstream>
#include <iostream>

std::string Block::printBlock() {
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


        for (auto & row : block.state) {
            for (char & col : row)
                inputFile.get(col);
        }
        blocks.push_back(block);
    }

    inputFile.close();
}

std::string BlockConverter::print() {
    std::string output;

    for (auto & block : blocks)
        output += "|" + block.printBlock() + "|\n";

    return output;
}