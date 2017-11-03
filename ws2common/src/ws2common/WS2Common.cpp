#include "ws2common/WS2Common.hpp"

namespace WS2Common {
    std::mt19937* getRandGen() {
        static std::mt19937 *randGen = nullptr;

        if (randGen == nullptr) {
            std::random_device rd;
            randGen = new std::mt19937(rd());
        }

        return randGen;
    }
}

