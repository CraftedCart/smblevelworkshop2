#include "ws2common/animation/EnumEasing.hpp"

namespace WS2Common {
    namespace Animation {
        namespace Easing {
            EnumEasing fromString(QString str) {
                if (str == "EASED" || str == "CUBIC") {
                    return CUBIC;
                } else if (str == "QUADRATIC") {
                    return QUADRATIC;
                } else if (str == "BOUNCE") {
                    return BOUNCE;
                } else if (str == "ELASTIC") {
                    return ELASTIC;
                } else if (str == "FREE") {
                    return FREE;
                } else {
                    return LINEAR;
                }
            }
        }
    }
}

