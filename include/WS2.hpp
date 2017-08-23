/**
 * @file
 * @brief Header for global WS2 members
 */

#ifndef SMBLEVELWORKSHOP2_WS2_HPP
#define SMBLEVELWORKSHOP2_WS2_HPP

#include <random>

/**
 * @brief Namespace where all SMB Level Workshop 2 code resides in
 *
 * WS2 is short for Workshop 2
 */
namespace WS2 {
    extern bool qAppRunning;
    extern std::mt19937 *randGen;

    //Prefixed with ws2 to avoid confusion with other functions that are within the WS2 namespace
    void ws2Init();
    void ws2Destroy();
}

#endif

