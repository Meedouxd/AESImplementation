#ifndef PROJECT_1_BLOCK_H
#define PROJECT_1_BLOCK_H

#include <string>
#include <vector>

struct Block {
    char state[4][4]{};     //4x4 array represents 16 bytes, column-major order
                            //2d arrays are [row][column]
    std::string printBlock();
    void hexPrint();
    Block() { for (auto & i : state) for (char & j : i) j = 0; }    //Default constructor
};

class BlockConverter {
private:
    std::vector<Block> blocks;  //Holds blocks

public:
    void convertBlocks(const std::string& fileName);    //Converts input from a text file to blocks

    std::string print();

    //char* blockToBytes(int blockIndex); // convert 4 x 4 byte matrix to 16 bytes

    BlockConverter() { blocks.clear(); }    //Default constructor

    Block* getBlockAt(int index);

    int getNumOfBlocks();

    void writeToFile(std::string ol);

    void flipBitAtIndex(int i);
};

#endif //PROJECT_1_BLOCK_H
