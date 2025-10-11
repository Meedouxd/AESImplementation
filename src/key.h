#ifndef PROJECT_1_KEY_H
#define PROJECT_1_KEY_H

#include <string>
#include <cassert>
#include <vector>

//The three supported sized of keys in AES
enum KEY_SIZES {
    KEY_128 = 16,   //16 bytes, 128 bits
    KEY_192 = 24,   //24 bytes, 192 bits
    KEY_256 = 32,   //32 bytes, 256 bits
};

template<KEY_SIZES size>    //Defined differently depending on the size of the key
class Key {
private:
    unsigned char key[size]{};  //Array of bytes

public:
    std::string printKey() {    //Outputs key in big endian form
        std::string output;
        for (const unsigned char & i : key)
            output += std::to_string(i) + " ";
        output += "\n";
        return output;
    }

    void flipBit(const int index) {     //Flips a bit at a given index
        assert(index >= 0 && index < size * 8);
        const int byteNo = index / 8;
        const int bitNo = index % 8;

        key[byteNo] ^= 1 << bitNo;  //Bit-shifting to the desired index and flipping
    }

    Key() {     //Default constructor
        for (unsigned char & i : key) { i = 0; }
    }

    std::vector<Key<KEY_128>> getExpandedKeys(){  // expand the keys to 10 since we are doing 128 bit keys.
        std::vector<Key<KEY_128>> expandedKeyList;
        return expandedKeyList;
    }
};

#endif //PROJECT_1_KEY_H
