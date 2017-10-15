#include "ws2common/EnumBananaType.hpp"

namespace WS2Common {
    namespace BananaType {
        EnumBananaType fromString(QString str) {
            if (str == "BUNCH") {
                return BUNCH;
            } else {
                return SINGLE;
            }
        }
    }
}

