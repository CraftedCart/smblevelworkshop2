/**
 * @file
 * @brief Header for the SMB2LzExporter class
 */

#include "ws2lz/TriangleIntersectionGrid.hpp"
#include "ws2common/Stage.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QDataStream>
#include <QMap>
#include <QHash>

namespace WS2Lz {
    using namespace WS2Common;

    /**
     * @brief Class for generating an uncompressed LZ for Super Monkey Ball 2
     *
     * LZ Specs: https://craftedcart.github.io/SMBLevelWorkshop/documentation/index.html?page=lzFormat2
     *
     * The order in which stuff is written:
     * - File header
     * - Start
     * - Fallout
     * - Collision headers
     * - Collision triangles
     * - Collision triangle index list pointers
     * - Collision triangle index lists
     * - Goals
     * - Bumpers
     * - Jamabars
     * - Bananas
     * - Level model pointer type A
     * - Level model pointer type B
     * - Level model
     * - Level model name
     * - Animaton headers
     * - Animation keyframes
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

            //Other guff
            /**
             * @brief The TriangleIntersectionGrid per collision header - The TriangleIntersectionGrid stores which
             *        triangles should be checked for collision in each grid tile
             */
            QHash<const Scene::GroupSceneNode*, TriangleIntersectionGrid*> triangleIntGridMap;

            //Offsets and counts
            //Key: Offset, Value: What the offset points to
            QMap<quint32, const Scene::GroupSceneNode*> collisionHeaderOffsetMap;
            quint32 startOffset;
            quint32 falloutOffset;
            QMap<quint32, const Scene::GroupSceneNode*> gridTriangleListPointersOffsetMap;
            QMap<const Scene::GroupSceneNode*, QVector<quint32>> gridTriangleIndexListOffsetMap;
            QMap<quint32, const Scene::GroupSceneNode*> gridTriangleListOffsetMap;
            //Offsets and counts for goals, bumpers, etc per collision header
            QMap<quint32, const Scene::GroupSceneNode*> goalOffsetMap;
            QMap<const Scene::GroupSceneNode*, quint32> goalCountMap;
            QMap<quint32, const Scene::GroupSceneNode*> bumperOffsetMap;
            QMap<const Scene::GroupSceneNode*, quint32> bumperCountMap;
            QMap<quint32, const Scene::GroupSceneNode*> jamabarOffsetMap;
            QMap<const Scene::GroupSceneNode*, quint32> jamabarCountMap;
            QMap<quint32, const Scene::GroupSceneNode*> bananaOffsetMap;
            QMap<const Scene::GroupSceneNode*, quint32> bananaCountMap;
            //TODO: Replace all this with maps \/
            quint32 coneCollisionObjectCount;
            quint32 coneCollisionObjectListOffset;
            quint32 sphereCollisionObjectCount;
            quint32 sphereCollisionObjectListOffset;
            quint32 cylinderCollisionObjectCount;
            quint32 cylinderCollisionObjectListOffset;
            quint32 falloutVolumeCount;
            quint32 falloutVolumeListOffset;
            QMap<quint32, const Scene::MeshSceneNode*> bgOffsetMap;
            QMap<quint32, QString> bgNameOffsetMap;
            //TODO: Mystery 8
            //TODO: Reflective level models
            //TODO: Level model instances
            QMap<quint32, const Scene::GroupSceneNode*> levelModelPointerAOffsetMap;
            QMap<quint32, const Scene::GroupSceneNode*> levelModelPointerBOffsetMap;
            QMap<quint32, const Scene::GroupSceneNode*> levelModelOffsetMap;
            QMap<const Scene::GroupSceneNode*, quint32> levelModelCountMap;
            QMap<quint32, QString> levelModelNameOffsetMap;
            quint32 switchCount;
            quint32 switchListOffset;
            //TODO: Fog anim header
            quint32 wormholeCount;
            quint32 wormholeListOffset;
            //TODO: Fog
            //TODO: Mystery 3

            //All 3D models for mesh collision
            QHash<QString, Resource::ResourceMesh*> models; //name, mesh - Using a hashmap as it will have a quicker lookup

        public:
            virtual ~SMB2LzExporter();

            void setModels(QHash<QString, Resource::ResourceMesh*> &models);

            /**
             * @brief Generates an uncompressed LZ for SMB 2, and writes it to dev
             *
             * @param dev The QDataStream to write bytes to
             * @param stage The stage to generate an uncompressed LZ for
             */
            void generate(QDataStream &dev, const Stage &stage);

        protected:
            /**
             * @brief Recursive function - Searches through the node's children, and their children, and their children, etc
             *        for MeshCollisionSceneNodes, and adds their vertices/indices to the vectors specified
             *
             * @param node The node to recursively search
             * @param vertices All vertices - This will be added to
             * @param indices Every 3 integers here corresponds to vertices for a triangle in the vertices vector - This will be added to
             */
            void addCollisionTriangles(
                    const Scene::SceneNode *node,
                    QVector<Model::Vertex> &allVertices,
                    QVector<unsigned int> &allIndices
                    );

            void optimizeCollision(const Stage &stage);

            /**
             * @brief Calculates offsets and item counts and writes it to class scoped variables
             *
             * @param stage The stage to calculate offsets for
             */
            void calculateOffsets(const Stage &stage);

            /**
             * @brief Recursive function - Searches through the node's children, and their children, and their children, etc
             *        for MeshCollisionSceneNodes, and adds COLLISION_TRIANGLE_LENGTH * number of triangles for each one
             *
             * @param node The node to recursively search
             * @param nextOffset The value to add to
             */
            void addCollisionTriangleOffsets(const Scene::SceneNode *node, quint32 &nextOffset);

            void writeFileHeader(QDataStream &dev);
            void writeStart(QDataStream &dev, const Stage &stage);
            void writeFallout(QDataStream &dev, const Stage &stage);
            void writeCollisionHeader(QDataStream &dev, const Scene::GroupSceneNode *node);
            void writeCollisionTriangleIndexList(QDataStream &dev, const TriangleIntersectionGrid *intGrid);
            void writeCollisionTriangleIndexListPointers(QDataStream &dev, const Scene::GroupSceneNode *node);
            void writeGoal(QDataStream &dev, const Scene::GoalSceneNode *node);
            void writeBumper(QDataStream &dev, const Scene::BumperSceneNode *node);
            void writeJamabar(QDataStream &dev, const Scene::JamabarSceneNode *node);
            void writeBanana(QDataStream &dev, const Scene::BananaSceneNode *node);

            /**
             * @brief Recursive function - Searches through the node's children, and their children, and their children, etc
             *        for MeshCollisionSceneNodes, and writes them
             *
             * @param dev The QDataStream to write to
             * @param node The node to recursively search
             */
            void writeCollisionTriangles(QDataStream &dev, const Scene::SceneNode *node);

            void writeLevelModelPointerAList(QDataStream &dev, const Scene::GroupSceneNode *node);
            void writeLevelModelPointerBList(QDataStream &dev, const Scene::GroupSceneNode *node);
            void writeLevelModelList(QDataStream &dev, const Scene::GroupSceneNode *node);
            void writeLevelModelNameList(QDataStream &dev, const Scene::GroupSceneNode *node);
            void writeBackgroundModel(QDataStream &dev, const Scene::MeshSceneNode *node);
            void writeBackgroundName(QDataStream &dev, const Scene::MeshSceneNode *node);

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
            quint32 addAllCounts(QMap<const Scene::GroupSceneNode*, quint32> &m);

            /**
             * @brief Rounds up a value to the nearest multiple of 4
             *
             * @param n The number to round
             *
             * @return The rounded up value
             */
            quint32 roundUpNearest4(quint32 n);

            //The rest of this file is madness required for the collision triangle writing port guff
            float toDegrees(float theta);
            glm::vec3 dotm(glm::vec3 a,glm::vec3 r0,glm::vec3 r1,glm::vec3 r2);
            float dot(glm::vec3 a,glm::vec3 b);
            glm::vec3 cross(glm::vec3 a,glm::vec3 b);
            glm::vec3 normalize(glm::vec3 v);
            glm::vec3 hat(glm::vec3 v);
            float reverseAngle(float c,float s);
    };
}

