/**
 * @file
 * @brief Misc convenience functions and what not go in here
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_WS2COMMON_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_WS2COMMON_HPP

#include "ws2common_export.h"
#include <random>

namespace WS2Common {
    WS2COMMON_EXPORT std::mt19937* getRandGen();
}

#endif

