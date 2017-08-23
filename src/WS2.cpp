#include "WS2.hpp"

namespace WS2 {
    //Define extern members
    bool qAppRunning = false;
    std::mt19937 *randGen;

    void ws2Init() {
        std::random_device rd;
        randGen = new std::mt19937(rd());
    }
}

