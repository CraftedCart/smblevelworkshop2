#include "ws2common/animation/FogAnimation.hpp"
namespace WS2Common {
    namespace Animation {
        std::set<KeyframeF *, KeyframeCompare>& FogAnimation::getRedKeyframes()
        {
            return redKeyframes;
        }

        std::set<KeyframeF *, KeyframeCompare>& FogAnimation::getGreenKeyframes()
        {
            return greenKeyframes;
        }

        std::set<KeyframeF *, KeyframeCompare>& FogAnimation::getBlueKeyframes()
        {
            return blueKeyframes;
        }

        std::set<KeyframeF *, KeyframeCompare>& FogAnimation::getStartDistanceKeyframes()
        {
            return startDistanceKeyframes;
        }

        std::set<KeyframeF *, KeyframeCompare>& FogAnimation::getEndDistanceKeyframes()
        {
            return endDistanceKeyframes;
        }

        std::set<KeyframeF *, KeyframeCompare>& FogAnimation::getUnknownKeyframes()
        {
            return unknownKeyframes;
        }

        const std::set<KeyframeF *, KeyframeCompare> &FogAnimation::getRedKeyframes() const
        {
            return redKeyframes;
        }

        const std::set<KeyframeF *, KeyframeCompare> &FogAnimation::getGreenKeyframes() const
        {
            return greenKeyframes;
        }

        const std::set<KeyframeF *, KeyframeCompare> &FogAnimation::getBlueKeyframes() const
        {
            return blueKeyframes;
        }

        const std::set<KeyframeF *, KeyframeCompare> &FogAnimation::getStartDistanceKeyframes() const
        {
            return startDistanceKeyframes;
        }

        const std::set<KeyframeF *, KeyframeCompare> &FogAnimation::getEndDistanceKeyframes() const
        {
            return endDistanceKeyframes;
        }

        const std::set<KeyframeF *, KeyframeCompare> &FogAnimation::getUnknownKeyframes() const
        {
            return unknownKeyframes;
        }

    }
}
