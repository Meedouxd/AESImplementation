#ifndef PROJECT_1_BLOCK_H
#define PROJECT_1_BLOCK_H

#include <string>
#include <vector>

struct Block {
    char state[4][4]{};     //4x4 array represents 16 bytes, column-major order
                            //2d arrays are [row][column]
    std::string printBlock();

    Block() { for (auto & i : state) for (char & j : i) j = 0; }
};

class BlockConverter {
private:
    std::vector<Block> blocks;

public:
    void convertBlocks(const std::string& fileName);

    std::string print();

    BlockConverter() { blocks.clear(); }
};

#endif //PROJECT_1_BLOCK_H