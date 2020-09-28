#include "ws2common/scene/RaceTrackPathSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        RaceTrackPathSceneNode::RaceTrackPathSceneNode(const QString name) : SceneNode(name) {}

        const QString RaceTrackPathSceneNode::getSerializableName() const {
            return "raceTrackPathSceneNode";
        }

		Animation::RaceTrackPath *RaceTrackPathSceneNode::getTrackPath() const
		{
			return trackPath;
		}

		void RaceTrackPathSceneNode::setTrackPath(Animation::RaceTrackPath *value)
		{
			trackPath = value;
		}
    }
}
