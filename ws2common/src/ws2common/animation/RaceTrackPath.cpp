#include "ws2common/animation/RaceTrackPath.hpp"

namespace WS2Common {
    namespace Animation {
        unsigned int RaceTrackPath::getPlayerID() const
        {
            return playerID;
        }

        void RaceTrackPath::setPlayerID(unsigned int value)
        {
            playerID = value;
        }
    }
}
