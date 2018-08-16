#include "ws2common/EnumBananaType.hpp"

namespace WS2Common {
    namespace BananaType {
        QString toString(EnumBananaType type) {
            switch (type) {
                case SINGLE:
                    return "SINGLE";
                case BUNCH:
                    return "BUNCH";
            }
        }

        EnumBananaType fromString(QString str) {
            if (str == "BUNCH") {
                return BUNCH;
            } else {
                return SINGLE;
            }
        }
    }
}

