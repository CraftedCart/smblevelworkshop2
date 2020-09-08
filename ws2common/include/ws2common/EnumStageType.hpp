/**
 * @file
 * @brief Header for the EnumStageType enum
 */

#ifndef ENUMSTAGETYPE_HPP
#define ENUMSTAGETYPE_HPP

#include "ws2common_export.h"
#include <QString>

namespace WS2Common {

    /**
     * @brief An enum to differentiate between stage types
     */
    enum EnumStageType {
        MAIN_GAME,
        MONKEY_RACE_2,
        MONKEY_FIGHT_2,
        MONKEY_TARGET_2,
        MONKEY_BILLIARDS_2,
        MONKEY_BOWLING_2,
        MONKEY_GOLF_2,
        MONKEY_BOAT,
        MONKEY_SHOT,
        MONKEY_DOGFIGHT,
        MONKEY_SOCCER,
        MONKEY_BASEBALL,
        CREDITS_GAME
    };

    namespace StageType {
        WS2COMMON_EXPORT QString toString(EnumStageType type);

        /**
         * @brief Converts a string to an EnumStageType
         *
         * @param str The string
         *
         * @return The EnumStageType for the string.
         */
        WS2COMMON_EXPORT EnumStageType fromString(QString str);

        WS2COMMON_EXPORT unsigned int toStageTypeFlag(EnumStageType type);
    }
}

#endif // ENUMSTAGETYPE_HPP
