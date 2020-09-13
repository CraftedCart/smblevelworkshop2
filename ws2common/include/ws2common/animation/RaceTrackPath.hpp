#ifndef RACETRACKPATH_HPP
#define RACETRACKPATH_HPP
#include "ws2common/animation/TransformAnimation.hpp"
#include "ws2common_export.h"

namespace WS2Common {
    namespace Animation {
        class WS2COMMON_EXPORT RaceTrackPath : public TransformAnimation {
            public:
                unsigned int getPlayerID() const;
                void setPlayerID(unsigned int value);

            protected:
                unsigned int playerID;
        };

    }
}

#endif // RACETRACKPATH_HPP
