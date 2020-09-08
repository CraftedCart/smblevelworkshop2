/**
 * @file
 * @brief Header for the SMB2LzExporter class
 */

#ifndef SMBLEVELWORKSHOP2_WS2LZ_SMB2LZEXPORTER_HPP
#define SMBLEVELWORKSHOP2_WS2LZ_SMB2LZEXPORTER_HPP

#include "ws2lz_export.h"
#include "ws2lz/TriangleIntersectionGrid.hpp"
#include "ws2common/Stage.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/SwitchSceneNode.hpp"
#include "ws2common/scene/WormholeSceneNode.hpp"
#include "ws2common/scene/FalloutVolumeSceneNode.hpp"
#include "ws2common/scene/ConeCollisionObjectSceneNode.hpp"
#include "ws2common/scene/SphereCollisionObjectSceneNode.hpp"
#include "ws2common/scene/CylinderCollisionObjectSceneNode.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QDataStream>
#include <QMap>
#include <QHash>

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
     * - Collision headers
     * - Collision triangles
     * - Collision triangle index list pointers
     * - Collision triangle index lists
     * - Goals
     * - Bumpers
     * - Jamabars
     * - Bananas
     * - Cone Collision Objects
     * - Sphere Collision Objects
     * - Cylinder Collision Objects
     * - Switches
     * - Wormholes
     * - Level model pointer type A
     * - Level model pointer type B
     * - Level model
     * - Level model name
     * - Animaton headers
     * - Animation keyframes
     * - Reflective models
     * - Fallout volumes
     */
    class WS2LZ_EXPORT SMB2LzExporter {
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
            const unsigned int CONE_COLLISION_LENGTH = 32;
            const unsigned int SPHERE_COLLISION_LENGTH = 20;
            const unsigned int CYLINDER_COLLISION_LENGTH = 28;
            const unsigned int SWITCH_LENGTH = 24;
            const unsigned int WORMHOLE_LENGTH = 28;
            const unsigned int BACKGROUND_MODEL_LENGTH = 56;
            const unsigned int ANIMATION_HEADER_LENGTH = 64;
            const unsigned int ANIMATION_KEYFRAME_LENGTH = 20;
            const unsigned int BACKGROUND_ANIMATION_HEADER_LENGTH = 96;
            const unsigned int RUNTIME_REFLECTIVE_MODEL_LENGTH = 12;
            const unsigned int FALLOUT_VOLUME_LENGTH = 32;
            const unsigned int EFFECT_HEADER_LENGTH = 48;
            const unsigned int TEXTURE_SCROLL_LENGTH = 8;

            //Other guff
            /**
             * @brief The TriangleIntersectionGrid per collision header - The TriangleIntersectionGrid stores which
             *        triangles should be checked for collision in each grid tile
             */
            QHash<const WS2Common::Scene::GroupSceneNode*, TriangleIntersectionGrid*> triangleIntGridMap;

            //Offsets and counts
            //Key: Offset, Value: What the offset points to
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> collisionHeaderOffsetMap;
            quint32 startOffset;
            quint32 falloutOffset;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> gridTriangleListPointersOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, QVector<quint32>> gridTriangleIndexListOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> gridTriangleListOffsetMap;
            //Offsets and counts for goals, bumpers, etc per collision header
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> goalOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> goalCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> bumperOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> bumperCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> jamabarOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> jamabarCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> bananaOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> bananaCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> coneCollisionObjectOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> coneCollisionObjectCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> sphereCollisionObjectOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> sphereCollisionObjectCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> cylinderCollisionObjectOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> cylinderCollisionObjectCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> switchOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> switchCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> wormholeOffsetMap; //Per collision header
            QMultiMap<quint32, QUuid> wormholeIndividualOffsetMap; //Per wormhole (Needed to link wormholes together) - the value is each wormhole's UUID
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> wormholeCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> runtimeReflectiveModelOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> runtimeReflectiveModelCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> falloutVolumeOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> falloutVolumeCountMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> groupAnimHeaderOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::MeshSceneNode*> bgAnimHeaderOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::MeshSceneNode*> fgAnimHeaderOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::MeshSceneNode*> effectHeaderOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::SceneNode*> textureScrollOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animPosXKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animPosYKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animPosZKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animRotXKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animRotYKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animRotZKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animScaleXKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animScaleYKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Animation::TransformAnimation*> animScaleZKeyframesOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::MeshSceneNode*> bgOffsetMap;
            QMultiMap<quint32, QString> bgNameOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::MeshSceneNode*> fgOffsetMap;
            QMultiMap<quint32, QString> fgNameOffsetMap;
            //TODO: Reflective level models
            //TODO: Level model instances
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerAOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerBOffsetMap;
            QMultiMap<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelOffsetMap;
            QMap<const WS2Common::Scene::GroupSceneNode*, quint32> levelModelCountMap;
            QMultiMap<quint32, QString> levelModelNameOffsetMap;
            //TODO: Fog anim header
            //TODO: Fog
            //TODO: Mystery 3

            //All 3D models for mesh collision
            QHash<QString, WS2Common::Resource::ResourceMesh*> models; //name, mesh - Using a hashmap as it will have a quicker lookup

            //Some flag related to whether or not the stage is for the main game or some party game mode
            quint32 stageType;

        public:
            virtual ~SMB2LzExporter();

            void setModels(QHash<QString, WS2Common::Resource::ResourceMesh*> &models);

            /**
             * @brief Generates an uncompressed LZ for SMB 2, and writes it to dev
             *
             * @param dev The QDataStream to write bytes to
             * @param stage The stage to generate an uncompressed LZ for
             */
            void generate(QDataStream &dev, const WS2Common::Stage &stage);

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
                    const WS2Common::Scene::SceneNode *node,
                    QVector<WS2Common::Model::Vertex> &allVertices,
                    QVector<unsigned int> &allIndices
                    );

            void optimizeCollision(const WS2Common::Stage &stage);

            /**
             * @brief Calculates offsets and item counts and writes it to class scoped variables
             *
             * @param stage The stage to calculate offsets for
             */
            void calculateOffsets(const WS2Common::Stage &stage);

            /**
             * @brief Recursive function - Searches through the node's children, and their children, and their children, etc
             *        for MeshCollisionSceneNodes, and adds COLLISION_TRIANGLE_LENGTH * number of triangles for each one
             *
             * @param node The node to recursively search
             * @param nextOffset The value to add to
             */
            void addCollisionTriangleOffsets(const WS2Common::Scene::SceneNode *node, quint32 &nextOffset);

            void writeFileHeader(QDataStream &dev);
            void writeStart(QDataStream &dev, const WS2Common::Stage &stage);
            void writeFallout(QDataStream &dev, const WS2Common::Stage &stage);
            void writeCollisionHeader(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeCollisionTriangleIndexList(QDataStream &dev, const TriangleIntersectionGrid *intGrid);
            void writeCollisionTriangleIndexListPointers(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeGoal(QDataStream &dev, const WS2Common::Scene::GoalSceneNode *node);
            void writeBumper(QDataStream &dev, const WS2Common::Scene::BumperSceneNode *node);
            void writeJamabar(QDataStream &dev, const WS2Common::Scene::JamabarSceneNode *node);
            void writeBanana(QDataStream &dev, const WS2Common::Scene::BananaSceneNode *node);
            void writeConeCollisionObject(QDataStream &dev, const WS2Common::Scene::ConeCollisionObjectSceneNode *node);
            void writeSphereCollisionObject(QDataStream &dev, const WS2Common::Scene::SphereCollisionObjectSceneNode *node);
            void writeCylinderCollisionObject(QDataStream &dev, const WS2Common::Scene::CylinderCollisionObjectSceneNode *node);
            void writeSwitch(QDataStream &dev, const WS2Common::Scene::SwitchSceneNode *node);
            void writeWormhole(QDataStream &dev, const WS2Common::Scene::WormholeSceneNode *node);
            void writeFalloutVolume(QDataStream &dev, const WS2Common::Scene::FalloutVolumeSceneNode *node);

            /**
             * @brief Recursive function - Searches through the node's children, and their children, and their children, etc
             *        for MeshCollisionSceneNodes, and writes them
             *
             * @param dev The QDataStream to write to
             * @param node The node to recursively search
             */
            void writeCollisionTriangles(QDataStream &dev, const WS2Common::Scene::SceneNode *node);

            void writeLevelModelPointerAList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeLevelModelPointerBList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeLevelModelList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeLevelModelNameList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeBackgroundModel(QDataStream &dev, const WS2Common::Scene::MeshSceneNode *node);
            void writeBackgroundName(QDataStream &dev, const WS2Common::Scene::MeshSceneNode *node);
            void writeForegroundModel(QDataStream &dev, const WS2Common::Scene::MeshSceneNode *node);
            void writeForegroundName(QDataStream &dev, const WS2Common::Scene::MeshSceneNode *node);
            void writeBgFgAnimationHeader(QDataStream &dev, const WS2Common::Animation::TransformAnimation *node);
            void writeAnimationHeader(QDataStream &dev, const WS2Common::Animation::TransformAnimation *anim);
            void writeEffectHeader(QDataStream &dev, const WS2Common::Scene::MeshSceneNode *node);
            void writeTextureScroll(QDataStream &dev, const WS2Common::Scene::SceneNode *node);
            void writeTransformAnimation(QDataStream &dev, const WS2Common::Animation::TransformAnimation *anim, bool scale);
            void writeRuntimeReflectiveModelList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node);
            void writeKeyframeF(QDataStream &dev, const WS2Common::Animation::KeyframeF *k);

            /**
             * @brief Like `writeKeyframeF`, but converts the value from radians to degrees first
             *
             * @param dev The data stream to write to
             * @param k The keyframe to write
             */
            void writeKeyframeAngleF(QDataStream &dev, const WS2Common::Animation::KeyframeF *k);

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

            //The rest of this file is madness required for the collision triangle writing port guff
            glm::vec3 dotm(glm::vec3 a, glm::mat3 m);
            glm::vec3 cross(glm::vec3 a, glm::vec3 b);
            glm::vec3 hat(glm::vec3 v);
            float reverseAngle(float c, float s);
    };
}

#endif

