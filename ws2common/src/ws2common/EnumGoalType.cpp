#include "ws2common/EnumGoalType.hpp"

namespace WS2Common {
    namespace GoalType {
        EnumGoalType fromString(QString str) {
            if (str == "GREEN") {
                return GREEN;
            } else if (str == "RED") {
                return RED;
            } else {
                return BLUE;
            }
        }
    }
}

