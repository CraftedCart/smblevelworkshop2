#include "ws2common/animation/EffectAnimation.hpp"

namespace WS2Common {
    namespace Animation {
        QVector<KeyframeEffect2*>& EffectAnimation::getEffect2Keyframes()
        {
            return effect2Keyframes;
        }

        const QVector<KeyframeEffect2*>& EffectAnimation::getEffect2Keyframes() const
        {
            return effect2Keyframes;
        }

        void EffectAnimation::setEffect2Keyframes(QVector<KeyframeEffect2*> &value)
        {
            effect2Keyframes = value;
        }

        const QVector<KeyframeEffect1*>& EffectAnimation::getEffect1Keyframes() const
        {
            return effect1Keyframes;
        }

        QVector<KeyframeEffect1*>& EffectAnimation::getEffect1Keyframes()
        {
            return effect1Keyframes;
        }

        void EffectAnimation::setEffect1Keyframes(QVector<KeyframeEffect1*> &value)
        {
            effect1Keyframes = value;
        }
    }
}
