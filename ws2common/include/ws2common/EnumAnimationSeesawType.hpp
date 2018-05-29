/**
 * @file
 * @brief Header for the EnumAnimationSeesawType enum
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ENUMANIMATIONSEESAWTYPE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ENUMANIMATIONSEESAWTYPE_HPP

#include "ws2common_export.h"
#include <QString>

namespace WS2Common {

    /**
     * @brief An enum to differentiate between animated collision headers and seesaw collision headers types
     *
     * Animated means that it can have an animation, but not necessarily
     */
    enum EnumAnimationSeesawType {
        ANIMATION,
        SEESAW
    };

    namespace AnimationSeesawType {
        WS2COMMON_EXPORT QString toString(EnumAnimationSeesawType type);

        /**
         * @brief Converts a string to an EnumAnimationSeesawType
         *
         * @param str The string
         *
         * @return The EnumAnimationSeesawTyoe for the string. If the string is invalid, ANIMATION will be returned.
         */
        WS2COMMON_EXPORT EnumAnimationSeesawType fromString(QString str);
    }
}

#endif

