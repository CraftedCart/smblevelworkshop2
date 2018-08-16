/**
 * @file
 * @brief Header for the EnumGoalType enum
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ANIMATION_ENUMLOOPTYPE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ANIMATION_ENUMLOOPTYPE_HPP

#include "ws2common_export.h"
#include <QString>

namespace WS2Common {
    namespace Animation {
        /**
         * @brief An enum to differentiate loop types
         *
         * @note The values given here are for SMB 2
         */
        enum EnumLoopType {
            LOOPING = 0x0000,
            PLAY_ONCE = 0x0001
        };

        namespace LoopType {
            /**
             * @brief Converts a string to an EnumLoopType
             *
             * @param str The string
             *
             * @return The EnumLoopType for the string. If the string is invalid, LOOPING will be returned.
             */
            WS2COMMON_EXPORT EnumLoopType fromString(QString str);
        }
    }
}

#endif


