#include "AES_Algorithm.h"
#include <iostream>

int main() {
    AES_Algorithm<KEY_128> AES;
    AES.init();
    return 0;
}