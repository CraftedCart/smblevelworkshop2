#ifndef FOGANIMATION_HPP
#define FOGANIMATION_HPP
#include "ws2common_export.h"
#include "ws2common/animation/TransformAnimation.hpp"
#include <set>

namespace WS2Common {
    namespace Animation {
        class WS2COMMON_EXPORT FogAnimation {
            protected:
                //Not using a Qt container as none of them seem to have the ability to take compare functors
                std::set<KeyframeF*, KeyframeCompare> redKeyframes;
                std::set<KeyframeF*, KeyframeCompare> greenKeyframes;
                std::set<KeyframeF*, KeyframeCompare> blueKeyframes;

                std::set<KeyframeF*, KeyframeCompare> startDistanceKeyframes;
                std::set<KeyframeF*, KeyframeCompare> endDistanceKeyframes;

                std::set<KeyframeF*, KeyframeCompare> unknownKeyframes;

            public:
                std::set<KeyframeF *, KeyframeCompare>& getRedKeyframes();
                std::set<KeyframeF *, KeyframeCompare>& getGreenKeyframes();
                std::set<KeyframeF *, KeyframeCompare>& getBlueKeyframes();

                std::set<KeyframeF *, KeyframeCompare>& getStartDistanceKeyframes();
                std::set<KeyframeF *, KeyframeCompare>& getEndDistanceKeyframes();

                std::set<KeyframeF *, KeyframeCompare>& getUnknownKeyframes();

                const std::set<KeyframeF *, KeyframeCompare>& getRedKeyframes() const;
                const std::set<KeyframeF *, KeyframeCompare>& getGreenKeyframes() const;
                const std::set<KeyframeF *, KeyframeCompare>& getBlueKeyframes() const;

                const std::set<KeyframeF *, KeyframeCompare>& getStartDistanceKeyframes() const;
                const std::set<KeyframeF *, KeyframeCompare>& getEndDistanceKeyframes() const;

                const std::set<KeyframeF *, KeyframeCompare>& getUnknownKeyframes() const;
        };
    }
}
#endif // FOGANIMATION_HPP
