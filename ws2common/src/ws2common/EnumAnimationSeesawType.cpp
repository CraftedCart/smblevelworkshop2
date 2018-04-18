#include "ws2common/EnumAnimationSeesawType.hpp"
#include <QDebug>

namespace WS2Common {
    namespace AnimationSeesawType {
        QString toString(EnumAnimationSeesawType type) {
            switch (type) {
                case ANIMATION:
                    return "ANIMATION";
                case SEESAW:
                    return "SEESAW";
                default:
                    //This should never happen!
                    qCritical() << "Invalid type passed to AnimationSeesawType::toString - defaulting to ANIMATION";
                    return "ANIMATION";
            }
        }

        EnumAnimationSeesawType fromString(QString str) {
            if (str == "SEESAW") {
                return SEESAW;
            } else {
                return ANIMATION;
            }
        }
    }
}

