#include "ws2common/EnumGoalType.hpp"

namespace WS2Common {
    namespace GoalType {
        QString toString(EnumGoalType type) {
            switch (type) {
                case BLUE:
                    return "BLUE";
                case GREEN:
                    return "GREEN";
                case RED:
                    return "RED";
            }
        }

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

