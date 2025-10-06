#ifndef PROJECT_1_KEY_H
#define PROJECT_1_KEY_H

#include <string>

constexpr int KEY_SIZE = 16;

class Key {
private:
    unsigned char key[KEY_SIZE]{};

public:
    std::string printKey();
    void flipBit(int index);

    Key() {
        for (unsigned char & i : key) { i = 0; }
    }
};

#endif //PROJECT_1_KEY_H