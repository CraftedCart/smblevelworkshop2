#include "ws2common/animation/TransformAnimation.hpp"

namespace WS2Common {
    namespace Animation {
        //Comparison function for keyframe lookup - to ensure that keyframes are ordered by time
        bool KeyframeCompare::operator() (const KeyframeF* lhs, const KeyframeF* rhs) const {
            return lhs->getValue().first < rhs->getValue().first;
        }

        EnumPlaybackState TransformAnimation::getInitialState() const {
            return initialState;
        }

        void TransformAnimation::setInitialState(EnumPlaybackState initialState) {
            this->initialState = initialState;
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

        const std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosXKeyframes() const {
            return posXKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosYKeyframes() {
            return posYKeyframes;
        }

        const std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosYKeyframes() const {
            return posYKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosZKeyframes() {
            return posZKeyframes;
        }

        const std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getPosZKeyframes() const {
            return posZKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotXKeyframes() {
            return rotXKeyframes;
        }

        const std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotXKeyframes() const {
            return rotXKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotYKeyframes() {
            return rotYKeyframes;
        }

        const std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotYKeyframes() const {
            return rotYKeyframes;
        }

        std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotZKeyframes() {
            return rotZKeyframes;
        }

        const std::set<KeyframeF*, KeyframeCompare>& TransformAnimation::getRotZKeyframes() const {
            return rotZKeyframes;
        }
    }
}

