#ifndef RACETRACKPATHSCENENODE_HPP
#define RACETRACKPATHSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/animation/RaceTrackPath.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT RaceTrackPathSceneNode : public SceneNode {
            protected:
                virtual const QString getSerializableName() const;

                Animation::RaceTrackPath *trackPath = nullptr;

            public:
                RaceTrackPathSceneNode() = default;
                RaceTrackPathSceneNode(const QString name);
				Animation::RaceTrackPath *getTrackPath() const;
				void setTrackPath(Animation::RaceTrackPath *value);
		};


	}
}

#endif // RACETRACKPATHSCENENODE_HPP
