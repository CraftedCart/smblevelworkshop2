/**
 * @file
 * @brief Header for the Keyframe template class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ANIMATION_KEYFRAME_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ANIMATION_KEYFRAME_HPP

#include "ws2common_export.h"
#include "ws2common/animation/EnumEasing.hpp"
#include <QPair>

namespace WS2Common {
    namespace Animation {
        template<typename T>
        class WS2COMMON_EXPORT Keyframe {
            protected:
                //float: Time, T: Value
                QPair<float, T> value;
                QPair<float, T> handleAValue;
                QPair<float, T> handleBValue;
                EnumEasing easing;

            public:
                Keyframe(float time, T value, EnumEasing easing);

                void setValue(QPair<float, T> value);
                const QPair<float, T> getValue() const;
                void setHandleAValue(QPair<float, T> value);
                const QPair<float, T> getHandleAValue() const;
                void setHandleBValue(QPair<float, T> value);
                const QPair<float, T> getHandleBValue() const;
                void setEasing(EnumEasing easing);
                EnumEasing getEasing() const;
        };

        //Typedefs for convenience
        typedef Keyframe<float> KeyframeF;
    }
}

#endif

#include "ws2common/animation/Keyframe.ipp"

