/**
 * @file
 * @brief Misc convenience functions and what not go in here
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_WS2COMMON_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_WS2COMMON_HPP

#include <random>

namespace WS2Common {
    std::mt19937* getRandGen();

    template<typename Base, typename T>
    bool instanceOf(const T *ptr);
}

#include "ws2common/WS2Common.ipp"

#endif

