/**
 * @file
 * @brief Implementation for templated functions of Keyframe
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_KEYFRAME_IPP
#define SMBLEVELWORKSHOP2_WS2COMMON_KEYFRAME_IPP

#include "ws2common/animation/Keyframe.hpp"

namespace WS2Common {
    namespace Animation {
        template<typename T>
        Keyframe<T>::Keyframe(float time, T value, EnumEasing easing) {
            this->value = QPair<float, T>(time, value);
            this->easing = easing;
        }

        template<typename T>
        void Keyframe<T>::setValue(QPair<float, T> value) {
            this->value = value;
        }

        template<typename T>
        const QPair<float, T> Keyframe<T>::getValue() const {
            return value;
        }

        template<typename T>
        void Keyframe<T>::setHandleAValue(QPair<float, T> value) {
            this->handleAValue = value;
        }

        template<typename T>
        const QPair<float, T> Keyframe<T>::getHandleAValue() const {
            return handleAValue;
        }

        template<typename T>
        void Keyframe<T>::setHandleBValue(QPair<float, T> value) {
            this->handleBValue = value;
        }

        template<typename T>
        const QPair<float, T> Keyframe<T>::getHandleBValue() const {
            return handleBValue;
        }

        template<typename T>
        EnumEasing Keyframe<T>::getEasing() const {
            return easing;
        }
    }
}

#endif

