/**
 * @file
 * @brief Header for the SMB2LzExporter class
 */

#include "ws2common/Stage.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include <QDataStream>
#include <QMap>

namespace WS2Lz {
    /**
     * @brief Class for generating an uncompressed LZ for Super Monkey Ball 2
     *
     * LZ Specs: https://craftedcart.github.io/SMBLevelWorkshop/documentation/index.html?page=lzFormat2
     *
     * The order in which stuff is written:
     * - File header
     * - Start
     * - Fallout
     * - Collision triangles
     * - Collision triangle index list
     * - Goals
     * - Bumpers
     * - Jamabars
     * - Bananas
     */
    class SMB2LzExporter {
        protected:
            //Constants
            const unsigned int FILE_HEADER_LENGTH = 2204;
            const unsigned int START_LENGTH = 20;
            const unsigned int FALLOUT_LENGTH = 4;
            const unsigned int COLLISION_HEADER_LENGTH = 1180;
            const unsigned int COLLISION_TRIANGLE_LENGTH = 64;
            const unsigned int COLLISION_TRIANGLE_INDEX_LENGTH = 2;
            const unsigned int COLLISION_TRIANGLE_LIST_POINTER_LENGTH = 4;
            const unsigned int LEVEL_MODEL_OFFSET_TYPE_A_LENGTH = 12;
            const unsigned int LEVEL_MODEL_OFFSET_TYPE_B_LENGTH = 4;
            const unsigned int LEVEL_MODEL_TYPE_A_LENGTH = 16;
            const unsigned int GOAL_LENGTH = 20;
            const unsigned int BUMPER_LENGTH = 32;
            const unsigned int JAMABAR_LENGTH = 32;
            const unsigned int BANANA_LENGTH = 16;
            const unsigned int WORMHOLE_LENGTH = 28;
            const unsigned int BACKGROUND_MODEL_LENGTH = 56;

            //Offsets and counts
            //Key: Offset, Value: What the offset points to
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> collisionHeaderOffsetMap;
            quint32 startOffset;
            quint32 falloutOffset;
            //Offsets and counts for goals, bumpers, etc per collision header
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> goalOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> goalCountMap;
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> bumperOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> bumperCountMap;
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> jamabarOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> jamabarCountMap;
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> bananaOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> bananaCountMap;
            quint32 coneCollisionObjectCount;
            quint32 coneCollisionObjectListOffset;
            quint32 sphereCollisionObjectCount;
            quint32 sphereCollisionObjectListOffset;
            quint32 cylinderCollisionObjectCount;
            quint32 cylinderCollisionObjectListOffset;
            quint32 falloutVolumeCount;
            quint32 falloutVolumeListOffset;
            quint32 backgroundModelCount;
            quint32 backgroundModelListOffset;
            //TODO: Mystery 8
            //TODO: Reflective level models
            //TODO: Level model instances
            quint32 levelModelCount;
            quint32 levelModelPointerAListOffset;
            quint32 levelModelPointerBListOffset;
            quint32 switchCount;
            quint32 switchListOffset;
            //TODO: Fog anim header
            quint32 wormholeCount;
            quint32 wormholeListOffset;
            //TODO: Fog
            //TODO: Mystery 3

        public:
            /**
             * @brief Generates an uncompressed LZ for SMB 2, and writes it to dev
             *
             * @param dev The QDataStream to write bytes to
             * @param stage The stage to generate an uncompressed LZ for
             */
            void generate(QDataStream &dev, const WS2Common::Stage &stage);

        protected:
            /**
             * @brief Calculates offsets and item counts and writes it to class scoped variables
             *
             * @param stage The stage to calculate offsets for
             */
            void calculateOffsets(const WS2Common::Stage &stage);

            void writeFileHeader(QDataStream &dev);
            void writeStart(QDataStream &dev, const WS2Common::Stage &stage);
            void writeFallout(QDataStream &dev, const WS2Common::Stage &stage);
            void writeCollisionHeader(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeGoal(QDataStream &dev, const WS2Common::Scene::GoalSceneNode *node);
            void writeBumper(QDataStream &dev, const WS2Common::Scene::BumperSceneNode *node);
            void writeJamabar(QDataStream &dev, const WS2Common::Scene::JamabarSceneNode *node);
            void writeBanana(QDataStream &dev, const WS2Common::Scene::BananaSceneNode *node);

            void writeNull(QDataStream &dev, const unsigned int count);

            /**
             * @brief Converts 0-360 deg rotation over to 0x0000 to 0xFFFF rotation
             *
             * @param rot The degrees rotation vector
             *
             * @return The rotation on a scale from 0x0000 to 0xFFFF
             */
            glm::tvec3<quint16> convertRotation(glm::vec3 rot);

            /**
             * @brief Adds all the values in a map together
             *
             * @param m The map
             *
             * @return All values summed
             */
            quint32 addAllCounts(QMap<const WS2Common::Scene::GroupSceneNode*, quint32> &m);
    };
}

