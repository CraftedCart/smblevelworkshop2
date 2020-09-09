#include "ws2common/EnumFogType.hpp"

namespace WS2Common {
    namespace FogType {
        QString toString(EnumFogType type) {
            switch (type) {
                case GX_FOG_NONE:
                    return "GX_FOG_NONE";
                case GX_FOG_LIN:
                    return "GX_FOG_LIN";
                case GX_FOG_EXP:
                    return "GX_FOG_EXP";
                case GX_FOG_EXP2:
                    return "GX_FOG_EXP2";
                case GX_FOG_REVEXP:
                    return "GX_FOG_REVEXP";
                case GX_FOG_REVEXP2:
                    return "GX_FOG_REVEXP2";
                default:
                    return "GX_FOG_NONE";
            }
        }

        EnumFogType fromString(QString str) {
            if (str == "GX_FOG_NONE") {
                return GX_FOG_NONE;
            } else if (str == "GX_FOG_LIN") {
                return GX_FOG_LIN;
            } else if (str == "GX_FOG_EXP") {
                return GX_FOG_EXP;
            } else if (str == "GX_FOG_EXP2") {
                return GX_FOG_EXP2;
            } else if (str == "GX_FOG_REVEXP") {
                return GX_FOG_REVEXP;
            } else if (str == "GX_FOG_REVEXP2") {
                return GX_FOG_REVEXP2;
            } else {
                return GX_FOG_NONE;
            }
        }
    }
}
