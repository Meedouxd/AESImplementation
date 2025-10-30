#include "key.h"
#include "block.h"
#include <random>
#include <iostream>
#include <fstream>
#include <map>

class EntropyRandomness{
    private:

    public:
        
    // https://cryptographyacademy.com/entropy/
        static double calculateEntropy(BlockConverter cipherBlock){

            double entropy = 0.0;

            std::map<unsigned char, int> byteCount;

            for(int i = 0; i < cipherBlock.getNumOfBlocks(); i++){
                for(int a = 0; a < 4; a++){
                    for(int b = 0; b < 4; b++){
                        unsigned char val = cipherBlock.getBlockAt(i)->state[a][b];
                        std::map<unsigned char, int>::iterator it = byteCount.find(val);

                        if(it != byteCount.end()){
                            it->second= it->second + 1;
                            continue;
                        }

                        std::pair<unsigned char, int>newPair = std::pair(val, 1);

                        byteCount.insert(newPair);

                    }
                }
            }

            // for(auto it : byteCount){
            //     std::cout << "Pair: ";
            //     std::cout << (int) it.first << ": ";
            //     std::cout << it.second << "\n";
            // }

            double byteSize = cipherBlock.getNumOfBlocks() * 16; // each block is 16 bytes (128 bits)
            for(auto it: byteCount){
                double newestEntropy = it.second / byteSize; // P(x)
                newestEntropy = newestEntropy * (std::log2(newestEntropy)); // P(x) * log2(P(x))
                entropy += newestEntropy;
            }

            return entropy * -1;
        }

        // statistical randomness frequency test
        // https://www.itl.nist.gov/div898/software/dataplot/refman1/auxillar/freqtest.htm
        static double calculateRandomness(BlockConverter cipherBlock){

            double randomness = 0.0;

            int oneCount = 0;
            int zeroCount = 0;
            for(int i = 0; i < cipherBlock.getNumOfBlocks(); i++){
                for(int a = 0; a < 4; a++){
                    for(int b = 0; b < 4; b++){
                        
                        unsigned char val = cipherBlock.getBlockAt(i)->state[a][b];

                        int one = getNumberOfOnes(val);
                        int zero = 8 - one;

                        oneCount += one;
                        zeroCount += zero;
                    }
                }
            }

            randomness = abs(oneCount - zeroCount);
            


            return pow(randomness, 0.5); // average frequency 
        }

        static int getNumberOfOnes(unsigned char c){

            int oneCount = 0;

            int abs = (int) c;

            while(abs > 0){
                if(abs % 2 == 1){
                    oneCount++;
                }
                abs /= 2;
            }

            return oneCount;
        }
};