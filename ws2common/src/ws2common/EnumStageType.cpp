#include "ws2common/EnumStageType.hpp"

namespace WS2Common {
    namespace StageType {
        QString toString(EnumStageType type) {
            switch (type) {
                case MAIN_GAME:
                    return "MAIN_GAME";
                case MONKEY_RACE_2:
                    return "MONKEY_RACE_2";
                case MONKEY_FIGHT_2:
                    return "MONKEY_FIGHT_2";
                case MONKEY_TARGET_2:
                    return "MONKEY_TARGET_2";
                case MONKEY_BILLIARDS_2:
                    return "MONKEY_BILLIARDS_2";
                case MONKEY_BOWLING_2:
                    return "MONKEY_BOWLING_2";
                case MONKEY_GOLF_2:
                    return "MONKEY_GOLF_2";
                case MONKEY_BOAT:
                    return "MONKEY_BOAT";
                case MONKEY_SHOT:
                    return "MONKEY_SHOT";
                case MONKEY_DOGFIGHT:
                    return "MONKEY_DOGFIGHT";
                case MONKEY_SOCCER:
                    return "MONKEY_SOCCER";
                case MONKEY_BASEBALL:
                    return "MONKEY_BASEBALL";
                case CREDITS_GAME:
                    return "CREDITS_GAME";
                default:
                    return "MAIN_GAME";
            }
        }

		EnumStageType fromString(QString str) {
			if (str == "MAIN_GAME") {
				return MAIN_GAME;
			} else if (str == "MONKEY_RACE_2") {
				return MONKEY_RACE_2;
			} else if (str == "MONKEY_FIGHT_2") {
				return MONKEY_FIGHT_2;
			} else if (str == "MONKEY_TARGET_2") {
				return MONKEY_TARGET_2;
			} else if (str == "MONKEY_BILLIARDS_2") {
				return MONKEY_BILLIARDS_2;
			} else if (str == "MONKEY_BOWLING_2") {
				return MONKEY_BOWLING_2;
			} else if (str == "MONKEY_GOLF_2") {
				return MONKEY_GOLF_2;
			} else if (str == "MONKEY_BOAT") {
				return MONKEY_BOAT;
			} else if (str == "MONKEY_SHOT") {
				return MONKEY_SHOT;
			} else if (str == "MONKEY_DOGFIGHT") {
				return MONKEY_DOGFIGHT;
			} else if (str == "MONKEY_SOCCER") {
				return MONKEY_SOCCER;
			} else if (str == "MONKEY_BASEBALL") {
				return MONKEY_BASEBALL;
			} else if (str == "CREDITS_GAME") {
				return CREDITS_GAME;
			} else {
				return MAIN_GAME;
			}
		}

		unsigned int toStageTypeFlag(EnumStageType type) {
		    switch (type) {
		        case MAIN_GAME:
		        case MONKEY_TARGET_2:
		        case MONKEY_GOLF_2:
		        case MONKEY_BOWLING_2:
		        case MONKEY_BILLIARDS_2:
		        case MONKEY_BASEBALL:
		        case MONKEY_DOGFIGHT:
		        case MONKEY_SOCCER:
		        case MONKEY_SHOT:
		        case CREDITS_GAME:
		            return 0x1;
		        case MONKEY_FIGHT_2:
		            return 0x4;
		        case MONKEY_RACE_2:
		        case MONKEY_BOAT:
		            return 0x9;
		        default:
		            return 0x1;
		    }
		}
    }
}
