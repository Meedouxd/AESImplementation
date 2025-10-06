#ifndef PROJECT_1_KEY_H
#define PROJECT_1_KEY_H

#include <string>
#include <cassert>

//The three supported sized of keys in AES
enum KEY_SIZES {
    KEY_128 = 16,   //16 bytes, 128 bits
    KEY_192 = 24,   //24 bytes, 192 bits
    KEY_256 = 32,   //32 bytes, 256 bits
};

template<KEY_SIZES size>
class Key {
private:
    unsigned char key[size]{};

public:
    std::string printKey() {    //Outputs key in big endian form
        std::string output;
        for (const unsigned char & i : key)
            output += std::to_string(i) + " ";
        output += "\n";
        return output;
    }

    void flipBit(const int index) {
        assert(index >= 0 && index < size);
        const int byteNo = index / 8;
        const int bitNo = index % 8;

        key[byteNo] ^= 1 << bitNo;
    }

    Key() {
        for (unsigned char & i : key) { i = 0; }
    }
};

#endif //PROJECT_1_KEY_H

