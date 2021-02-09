#include "ws2common/Fog.hpp"

namespace WS2Common {
    EnumFogType Fog::getFogType() const
    {
        return fogType;
    }

    void Fog::setFogType(const EnumFogType &value)
    {
        fogType = value;
    }

    float Fog::getRedValue() const
    {
        return redValue;
    }

    void Fog::setRedValue(float value)
    {
        redValue = value;
    }

    float Fog::getBlueValue() const
    {
        return blueValue;
    }

    void Fog::setBlueValue(float value)
    {
        blueValue = value;
    }

    float Fog::getGreenValue() const
    {
        return greenValue;
    }

    void Fog::setGreenValue(float value)
    {
        greenValue = value;
    }

    float Fog::getStartDistance() const
    {
        return startDistance;
    }

    void Fog::setStartDistance(float value)
    {
        startDistance = value;
    }

    float Fog::getEndDistance() const
    {
        return endDistance;
    }

    void Fog::setEndDistance(float value)
    {
        endDistance = value;
    }
}
