#include "ws2common/EnumPlaybackState.hpp"

namespace WS2Common {
    namespace PlaybackState {
        EnumPlaybackState fromString(QString str) {
            if (str == "PAUSE") {
                return PAUSE;
            } else if (str == "PLAY_BACKWARDS") {
                return PLAY_BACKWARDS;
            } else if (str == "FAST_FORWARD") {
                return FAST_FORWARD;
            } else if (str == "PLAY_BACKWARDS") {
                return PLAY_BACKWARDS;
            } else if (str == "REWIND") {
                return REWIND;
            } else {
                return PLAY;
            }
        }
    }
}

