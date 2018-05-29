/**
 * @file
 * @brief Header for the EnumBananaType enum
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ENUMBANANATYPE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ENUMBANANATYPE_HPP

#include "ws2common_export.h"
#include <QString>

namespace WS2Common {

    /**
     * @brief An enum to differentiate banana types
     */
    enum EnumBananaType {
        SINGLE = 0x00000000,
        BUNCH = 0x00000001
    };

    namespace BananaType {
        /**
         * @brief Converts a string to an EnumBananaType
         *
         * @param str The string
         *
         * @return The EnumGoalType for the string. If the string is invalid, SINGLE will be returned.
         */
        WS2COMMON_EXPORT EnumBananaType fromString(QString str);
    }
}

#endif

