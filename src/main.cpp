#include "AES_Algorithm.h"
#include <iostream>
#include <bits/stdc++.h>

int main() {
    AES_Algorithm<KEY_128> AES;
    std::string plainTextLocation;
    unsigned char zeroKeyBits[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Key<KEY_128> keyVarKey(zeroKeyBits);

    AES.init();

    std::cout << "Enter file name: " << std::endl;
    std::cin >> plainTextLocation;
    std::filesystem::create_directories("keyvar");
    std::srand(time(0));

    std::vector<int>keyBitsFlipped;

    while(keyBitsFlipped.size() < 20){
        int next = std::rand() % 128;

        for(int i : keyBitsFlipped){
            if(i == next){
                continue;
            }
        }
        std::cout << "Pushing this number onto the vector: " << next;
        keyBitsFlipped.push_back(next);
    }


    //key differential
    for(int i = 0; i < 20; i++){
        BlockConverter dataBlocks = AES.encrypt(plainTextLocation, keyVarKey);

        std::string outputName = "keyvar/";
        outputName += keyVarKey.keyFileName();
        outputName += ".txt";

        std::cout << "Output Name: " << outputName << std::endl;

        dataBlocks.writeToFile(outputName);

        keyVarKey.flipBit(keyBitsFlipped.at(i));
    }


    // plain text data differential

    for(int i = 0; i < 20; i++){

    }

    // AES.encrypt();
    // AES.writeToFile();
    return 0;
}