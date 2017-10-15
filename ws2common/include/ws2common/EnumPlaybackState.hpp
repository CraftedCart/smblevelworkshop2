/**
 * @file
 * @brief Header for the EnumPlaybackState enum
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ENUMPLAYBACKSTATE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ENUMPLAYBACKSTATE_HPP

#include <QString>

namespace WS2Common {

    /**
     * @brief An enum to differentiate playback states
     */
    enum EnumPlaybackState {
        PLAY = 0x0000,
        PAUSE = 0x0001,
        PLAY_BACKWARDS = 0x0002,
        FAST_FORWARD = 0x0003,
        REWIND = 0x0004
    };

    namespace PlaybackState {
        /**
         * @brief Converts a string to an EnumPlaybackState
         *
         * @param str The string
         *
         * @return The EnumPlaybackState for the string. If the string is invalid, PLAY will be returned.
         */
        EnumPlaybackState fromString(QString str);
    }
}

#endif

