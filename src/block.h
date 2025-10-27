#ifndef PROJECT_1_BLOCK_H
#define PROJECT_1_BLOCK_H

#include <string>
#include <vector>

struct Block {
    char state[4][4]{};     //4x4 array represents 16 bytes, column-major order
                            //2d arrays are [row][column]
    std::string printBlock() const;
    void hexPrint() const;
    Block() { for (auto & i : state) for (char & j : i) j = 0; }    //Default constructor
};

class BlockConverter {
private:
    std::vector<Block> blocks;  //Holds blocks

public:
    void convertBlocks(const std::string& fileName);    //Converts input from a text file to blocks

    std::string print() const;

    //char* blockToBytes(int blockIndex); // convert 4 x 4 byte matrix to 16 bytes

    BlockConverter() { blocks.clear(); }    //Default constructor

    Block* getBlockAt(int index);

    int getNumOfBlocks() const;

    void writeToFile(const std::string &outputLocation);

    void flipBitAtIndex(int index);
};

#endif //PROJECT_1_BLOCK_H
