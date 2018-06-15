/**
 * @file
 * @brief Header for the EnumGoalType enum
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ENUMGOALTYPE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ENUMGOALTYPE_HPP

#include "ws2common_export.h"
#include <QString>

namespace WS2Common {

    /**
     * @brief An enum to differentiate goal types
     *
     * @note The values given for the goals here are for SMB 2 - SMB 1 uses different values in the LZ
     */
    enum EnumGoalType {
        BLUE = 0x0001,
        GREEN = 0x0101,
        RED = 0x0201
    };

    namespace GoalType {
        WS2COMMON_EXPORT QString toString(EnumGoalType type);

        /**
         * @brief Converts a string to an EnumGoalType
         *
         * @param str The string
         *
         * @return The EnumGoalType for the string. If the string is invalid, BLUE will be returned.
         */
        WS2COMMON_EXPORT EnumGoalType fromString(QString str);
    }
}

#endif

