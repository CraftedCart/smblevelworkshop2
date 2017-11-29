#include "ws2common/animation/EnumLoopType.hpp"

namespace WS2Common {
    namespace Animation {
        namespace LoopType {
            EnumLoopType fromString(QString str) {
                if (str == "PLAY_ONCE") {
                    return PLAY_ONCE;
                } else {
                    return LOOPING;
                }
            }
        }
    }
}

