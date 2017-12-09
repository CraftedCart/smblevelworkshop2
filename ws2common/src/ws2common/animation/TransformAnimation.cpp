#include "ws2common/animation/TransformAnimation.hpp"

namespace WS2Common {
    namespace Animation {
        //Comparison function for keyframe lookup - to ensure that keyframes are ordered by time
        bool KeyframeCompare::operator() (const KeyframeF* lhs, const KeyframeF* rhs) const {
            return lhs->getValue().first < rhs->getValue().second;
        }

        EnumLoopType TransformAnimation::getLoopType() const {
            return loopType;
        }

        void TransformAnimation::setLoopType(EnumLoopType loopType) {
            this->loopType = loopType;
        }

        float TransformAnimation::getLoopTime() const {
            return loopTime;
        }

        void TransformAnimation::setLoopTime(float loopTime) {
            this->loopTime = loopTime;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosXKeyframes() {
            return posXKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosYKeyframes() {
            return posYKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosZKeyframes() {
            return posZKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotXKeyframes() {
            return rotXKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotYKeyframes() {
            return rotYKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotZKeyframes() {
            return rotZKeyframes;
        }
    }
}

