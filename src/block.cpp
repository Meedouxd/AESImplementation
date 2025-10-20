#include "block.h"
#include <fstream>
#include <iostream>

std::string Block::printBlock() {   //Prints in a way that restores input text block
    std::string output;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){

            output += state[j][i];
        }
    }

    return output;
}

void Block::hexPrint(){ //prints a block 
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            std::cout << std::hex << (unsigned int) state[i][j];
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

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                char c = 0;
                inputFile.get(c);
                block.state[j][i] = c; // fill it column by column
            }
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

int BlockConverter::getNumOfBlocks(){
    return blocks.size();
}

void BlockConverter::writeToFile(std::string outputLocation){
    std::ofstream cipherfile(outputLocation);

        for(int i = 0; i < getNumOfBlocks(); i++){
            cipherfile << (getBlockAt(i)->printBlock());
        }

        cipherfile.close();
}

