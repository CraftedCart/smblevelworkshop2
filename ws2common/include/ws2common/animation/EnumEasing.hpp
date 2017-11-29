/**
 * @file
 * @brief Header for EnumEasing
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ANIMATION_ENUMEASING_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ANIMATION_ENUMEASING_HPP

#include <QString>

namespace WS2Common {
    namespace Animation {
        enum EnumEasing {
            LINEAR,
            CUBIC,
            QUADRATIC,
            BOUNCE,
            ELASTIC,
            FREE
        };

        namespace Easing {
            /**
             * @brief Converts a string to an EnumEasing
             *
             * If EASED is passed, it will be converted to CUBIC
             *
             * @param str The string
             *
             * @return The EnumEasing for the string. If the string is invalid, LINEAR will be returned.
             */
            EnumEasing fromString(QString str);
        }
    }
}

#endif

