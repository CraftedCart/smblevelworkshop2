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
#include "ws2common/scene/MeshSceneNode.hpp"
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
     * - Level model pointer type A
     * - Level model pointer type B
     * - Level model
     * - Level model name
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
            const unsigned int LEVEL_MODEL_POINTER_TYPE_A_LENGTH = 12;
            const unsigned int LEVEL_MODEL_POINTER_TYPE_B_LENGTH = 4;
            const unsigned int LEVEL_MODEL_LENGTH = 16;
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
            //TODO: Replace all this with maps \/
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
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerAOffsetMap;
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerBOffsetMap;
            QMap<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> levelModelCountMap;
            QMap<quint32, QString> levelModelNameOffsetMap;
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
            void writeLevelModelPointerAList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeLevelModelPointerBList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeLevelModelList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeLevelModelNameList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);

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

            /**
             * @brief Rounds up a value to the nearest multiple of 4
             *
             * @param n The number to round
             *
             * @return The rounded up value
             */
            quint32 roundUpNearest4(quint32 n);
    };
}
