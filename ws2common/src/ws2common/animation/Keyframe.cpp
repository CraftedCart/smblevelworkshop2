#include "ws2common/animation/Keyframe.hpp"

namespace WS2Common {
    namespace Animation {
        template<typename T>
        EnumEasing Keyframe<T>::getEasing() const {
            return easing;
        }
    }
}

