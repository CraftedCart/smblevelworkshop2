#ifndef FOG_HPP
#define FOG_HPP

#include "ws2common_export.h"
#include "ws2common/EnumFogType.hpp"

namespace WS2Common {
    class WS2COMMON_EXPORT Fog {
    public:
        EnumFogType getFogType() const;
        void setFogType(const EnumFogType &value);

        float getRedValue() const;
        void setRedValue(float value);

        float getBlueValue() const;
        void setBlueValue(float value);

        float getGreenValue() const;
        void setGreenValue(float value);

        float getStartDistance() const;
        void setStartDistance(float value);

        float getEndDistance() const;
        void setEndDistance(float value);

    protected:
        EnumFogType fogType;
        float redValue;
        float blueValue;
        float greenValue;
        float startDistance;
        float endDistance;
    };
}

#endif // FOG_HPP
