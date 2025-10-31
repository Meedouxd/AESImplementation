#include "AES_Algorithm.h"
#include <iostream>
#include <array>
#include <fstream>
// #include <filesystem>

// clear keyvar and plaintextvar/ on every run
// void clearFolder(std::string folderPath)
// {
//     for (const auto& content : std::filesystem::directory_iterator(folderPath)) 
//         std::filesystem::remove_all(content.path());
// }

//given a list of data bits to be flipped print out each one that has been flipped so far
std::string getFlippedDataBitsFileName(std::vector<int> v, int iterator_index){

    std::string rv = "";

    for(int i = 0; i <= iterator_index; i++){
        rv += std::to_string(v.at(i));
        rv += "_";
    }

    return rv;
}

// Save histogram of byte frequencies to CSV
void saveHistogram(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    std::array<int, 256> freq = {0};
    char c;
    while (in.get(c))
        freq[(unsigned char)c]++;

    std::string csvName = filename + "_histogram.csv";
    std::ofstream out(csvName);
    for (int i = 0; i < 256; ++i)
        out << i << "," << freq[i] << "\n";

    std::cout << "Histogram saved to " << csvName << std::endl;
}

int main() {
    
    AES_Algorithm<KEY_128> AES; // AES object for encrypting

    std::string plainTextLocation; // location of the plaintext file

    // creation of the two keys to keep track of: key-variable key (changes every iteration)
    // and keyVarPlain (does not change)
    unsigned char zeroKeyBits[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Key<KEY_128> keyVarKey(zeroKeyBits);
    Key<KEY_128> keyVarPlain(zeroKeyBits);

    // keep track of which bits to flip within the keyVarKey and keyVarPlain
    std::vector<int>keyBitsFlipped;
    std::vector<int>plainTextBitsFlipped;

    // should probably delete
    AES.init();

    // get user input
    std::cout << "Enter file name: " << std::endl;
    std::cin >> plainTextLocation;

    // create directories to store our ciphertexts if they  dont already exist
    // std::filesystem::create_directories("keyvar");
    // std::filesystem::create_directories("plaintextvar");

    // clear the folders of any left-over ciphertexts.
    // clearFolder("keyvar");
    // clearFolder("plaintextvar");


    std::srand(time(0));

    // Load the plain text file into memory
    BlockConverter dataBlocks;
    dataBlocks.convertBlocks(plainTextLocation);

    // calculate number of bits within the plain text
    int numBitsDataBlocks = dataBlocks.getNumOfBlocks() * 16 * 8;

    // generate bits to flip for the keyVarKey
    while(keyBitsFlipped.size() < 20){
        int next = std::rand() % 128;

        for(int i : keyBitsFlipped){
            if(i == next){
                continue;
            }
        }
        keyBitsFlipped.push_back(next);
    }

    // generating bits to flip in plain text
    while(plainTextBitsFlipped.size() < 20){
        int next = std::rand() % numBitsDataBlocks;

        for(int i : plainTextBitsFlipped){
            if(i == next){
                continue;
            }
        }
        plainTextBitsFlipped.push_back(next);
    }

    //key differential
    for(int i = 0; i < 20; i++){

        BlockConverter newDataBlocks = AES.encrypt(dataBlocks, keyVarKey);

        std::string outputName = "keyvar/";
        outputName += keyVarKey.keyFileName();
        outputName += ".txt";

        std::cout << "Output Name: " << outputName << std::endl;

        newDataBlocks.writeToFile(outputName);
        saveHistogram(outputName);

        keyVarKey.flipBit(keyBitsFlipped.at(i));
    }


    // plain text data differential
    BlockConverter plainTextCopy = dataBlocks; //shallow copy block converter of OG data

    for(int i = 0; i < 20; i++){
        plainTextCopy.flipBitAtIndex(plainTextBitsFlipped.at(i));

        BlockConverter newDataBlocks = AES.encrypt(plainTextCopy, keyVarKey);

        std::string outputName = "plaintextvar/";
        outputName += getFlippedDataBitsFileName(plainTextBitsFlipped, i);
        outputName += ".txt";

        std::cout << "Output Name: " << outputName << std::endl;

        newDataBlocks.writeToFile(outputName);
        saveHistogram(outputName);

        keyVarKey.flipBit(keyBitsFlipped.at(i));
    }
    
    return 0;
}
