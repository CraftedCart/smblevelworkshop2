#ifndef EFFECTANIMATION_HPP
#define EFFECTANIMATION_HPP
#include "ws2common_export.h"
#include "ws2common/animation/KeyframeEffect1.hpp"
#include "ws2common/animation/KeyframeEffect2.hpp"
#include <QVector>

namespace WS2Common {
    namespace Animation {
        class WS2COMMON_EXPORT EffectAnimation {
            protected:
                QVector<KeyframeEffect1*> effect1Keyframes;
                QVector<KeyframeEffect2*> effect2Keyframes;

            public:
                const QVector<KeyframeEffect1*>& getEffect1Keyframes() const;
                QVector<KeyframeEffect1*>& getEffect1Keyframes();
                void setEffect1Keyframes(QVector<KeyframeEffect1*> &value);
                const QVector<KeyframeEffect2*>& getEffect2Keyframes() const;
                QVector<KeyframeEffect2*>& getEffect2Keyframes();
                void setEffect2Keyframes(QVector<KeyframeEffect2*> &value);
        };
    }
}

#endif // EFFECTANIMATION_HPP
