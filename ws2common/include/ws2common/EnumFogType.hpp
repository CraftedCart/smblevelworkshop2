#ifndef ENUMFOGTYPE_HPP
#define ENUMFOGTYPE_HPP

#include "ws2common_export.h"
#include <QString>

namespace WS2Common {

    /**
     * @brief An enum to differentiate fog types
     */
    enum EnumFogType {
        GX_FOG_NONE = 0,
        GX_FOG_LIN = 2,
        GX_FOG_EXP = 4,
        GX_FOG_EXP2 = 5,
        GX_FOG_REVEXP = 6,
        GX_FOG_REVEXP2 = 7
    };

    namespace FogType {
        WS2COMMON_EXPORT QString toString(EnumFogType type);

        /**
         * @brief Converts a string to an EnumFogType
         *
         * @param str The string
         *
         * @return The EnumFogType for the string. If the string is invalid, GX_FOG_NONE will be returned.
         */
        WS2COMMON_EXPORT EnumFogType fromString(QString str);
    }
}

#endif // ENUMFOGTYPE_HPP
