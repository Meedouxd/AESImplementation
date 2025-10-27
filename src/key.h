#ifndef PROJECT_1_KEY_H
#define PROJECT_1_KEY_H

#include <string>
#include <cassert>
#include <vector>
#include <iostream>

//The three supported sized of keys in AES
enum KEY_SIZES {
    KEY_128 = 16,   //16 bytes, 128 bits
    KEY_192 = 24,   //24 bytes, 192 bits
    KEY_256 = 32,   //32 bytes, 256 bits
};

constexpr unsigned char substitutionBox[256] = {
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

    std::string keyFileName() {
        std::string output;
        for (const unsigned char & i : key)
            output += std::to_string((int)i) + "_";
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

    explicit Key(unsigned char* arr) {
        for(int i = 0; i < 16; i++)
            key[i] = arr[i];
    }

    static unsigned char* XORWords(const unsigned char* wordArrOne, const unsigned char* wordArrTwo) {
        const auto returnVal = new unsigned char[4];
        for(int i = 0; i < 4; i++)
            returnVal[i] = (wordArrOne[i] ^ wordArrTwo[i]);

        return returnVal;
    }
    // for key expansion
    static void XORRcon(unsigned char* wordArray, const int round) {

        // table found at: https://en.wikipedia.org/wiki/AES_key_schedule#Rcon
        const unsigned char RCON[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

        wordArray[0] ^= RCON[round];

        for(int i = 1; i < 4; i++)
            wordArray[i] ^= 0x00; // last 3 bytes are XOR'ed with 0b00000000
    }

    // for key expansion
    static void subWords(unsigned char* wordArray) {
        for(int i = 0; i < 4; i++)
            wordArray[i] = substitutionBox[wordArray[i]];
    }

    // for key expansion
    static void rotWord(unsigned char* wordArray) { // left shift each byte once
        const unsigned char copy[4] = {wordArray[0], wordArray[1], wordArray[2], wordArray[3]};

        for(int i = 0; i < 4; i++)
            wordArray[(i+3) % 4] = copy[i];
    }

    static unsigned char* getWordFromColumn(Key k, const int columnIndex) {
        const std::string kStr = k.toString();

        const int offset = columnIndex * 4;

        unsigned char* returnVal = new unsigned char[4];

        for(int i = offset; i < offset+4; i++)
            returnVal[i-offset] = kStr.at(i);

        return returnVal;

    }
    // https://www.kavaliro.com/wp-content/uploads/2014/03/AES.pdf
        static std::vector<Key<KEY_128>> getExpandedKeys(const Key<KEY_128> firstKey) {  // expand the keys to 10 since we are doing 128 bit keys.
        std::vector<Key<KEY_128>> expandedKeyList;

        // std::cout << "First KEy:\n";
        // firstKey.printHex();
        
        expandedKeyList.push_back(firstKey); // round 0 key

        for(int i = 0; i < 10; i++) {
            unsigned char newKeyBytes[16] = {};
            Key<KEY_128> previousKey = expandedKeyList.at(i);

            unsigned char* gWord = getWordFromColumn(previousKey, 3);

            rotWord(gWord);
            subWords(gWord);
            XORRcon(gWord, i);

            unsigned char* wordToAdd = XORWords(gWord, getWordFromColumn(previousKey, 0));
            int keyByteIndex = 0;

            for(int j = 1; j < 4; j++) {
                for(int k = 0; k < 4; k++)
                    newKeyBytes[keyByteIndex++] = wordToAdd[j];
                wordToAdd = XORWords(wordToAdd, getWordFromColumn(previousKey, i));
            }

            for(int j = 0; j < 4; j++)
                    newKeyBytes[keyByteIndex++] = wordToAdd[j];

            Key<KEY_128> nextKey = Key(newKeyBytes);
            expandedKeyList.push_back(nextKey);
        }

        // std::cout <<"Printing out expanded key list\n";

        // for(Key<KEY_128> key : expandedKeyList) {
        //     key.printHex();
        // }

        return expandedKeyList;
    }

    std::string toString() {
        std::string returnValue = "";
        for(int i = 0; i < 16; i++)
            returnValue += key[i];

        return returnValue;
    }

    void printHex() {
        for(int i = 0; i < 16; i++) {
            std::cout << std::hex << static_cast<int>(key[i]);
            std::cout << " ";
        }
        std::cout << "\n";
    }
};

#endif //PROJECT_1_KEY_H