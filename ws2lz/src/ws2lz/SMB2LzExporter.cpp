#include "ws2lz/SMB2LzExporter.hpp"
#include "ws2lz/WS2Lz.hpp"
#include "ws2common/WS2Common.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/MeshCollisionSceneNode.hpp"
#include "ws2common/scene/RaceTrackPathSceneNode.hpp"
#include "ws2common/scene/BoosterSceneNode.hpp"
#include "ws2common/scene/GolfHoleSceneNode.hpp"
#include <QElapsedTimer>
#include <QThreadPool>
#include <QDebug>
#include <QtMath>
#include <glm/gtc/constants.hpp>
#include <math.h>

//Macros
/**
 * @brief Iterates over all collision headers - The value of group will be the GroupSceneNode at the current iteration
 */
#define forEachGroup(group) foreach(const Scene::GroupSceneNode* group, collisionHeaderOffsetMap)

/**
 * @brief Iterates over all background - The value of group will be the GroupSceneNode at the current iteration
 */
#define forEachBg(mesh) foreach(const Scene::MeshSceneNode* mesh, bgOffsetMap)

/**
 * @brief Iterates over all foreground - The value of group will be the GroupSceneNode at the current iteration
 */
#define forEachFg(mesh) foreach(const Scene::MeshSceneNode* mesh, fgOffsetMap)

/**
 * @brief Iterates over all of a SceneNode's children, and runs the following code if the child can be dynamically
 *        casted to type
 */
#define forEachChildType(parent, type, child)\
    foreach(Scene::SceneNode *childNode, parent->getChildren())\
        if (type child = dynamic_cast<type>(childNode))

/**
 * @brief Combines forEachGroup and forEachChildType - Iterates over all collision headers' children, and runs the
 *        following code if the child can be dyanmically casted to type
 */
#define forEachGroupChildType(type, child) forEachGroup(group) forEachChildType(group, type, child)

namespace WS2Lz {
    using namespace WS2Common;

    SMB2LzExporter::~SMB2LzExporter() {
        qDeleteAll(triangleIntGridMap.values());
    }

    void SMB2LzExporter::setModels(QHash<QString, Resource::ResourceMesh*> &models) {
        this->models = models;
    }

    void SMB2LzExporter::generate(QDataStream &dev, const Stage &stage) {
        //TODO: Add a configureDataStream function or something - to make it easy to override for a Dx exporter
        dev.setByteOrder(QDataStream::BigEndian);
        dev.setFloatingPointPrecision(QDataStream::SinglePrecision);

        optimizeCollision(stage);
        calculateOffsets(stage);

        stageType = StageType::toStageTypeFlag(stage.getStageType());

        //Write the data
        writeFileHeader(dev);
        writeStart(dev, stage);
        writeFallout(dev, stage);
        writeFog(dev, stage.getFog());
        writeFogAnimationHeader(dev, stage.getFogAnimation());
        writeFogAnimation(dev, stage.getFogAnimation());
        if (stage.getStageType() == EnumStageType::MONKEY_RACE_2) {
            writeRaceHeader(dev, stage);
            writeCPUTrackPathHeaders(dev, stage);
            forEachChildType(stage.getRootNode(), Scene::RaceTrackPathSceneNode*, node) writeRaceTrackPath(dev, node);
            forEachChildType(stage.getRootNode(), Scene::BoosterSceneNode*, node) writeBooster(dev, node);
        }
        if (stage.getStageType() == EnumStageType::MONKEY_GOLF_2) {
            forEachChildType(stage.getRootNode(), Scene::GolfHoleSceneNode*, node) writeGolfHole(dev, node);
        }
        forEachGroup(group) writeCollisionHeader(dev, group); //Collision Headers
        forEachGroup(group) writeCollisionTriangles(dev, group); //Collision triangles
        forEachGroup(group) writeCollisionTriangleIndexListPointers(dev, group); //Collision triangle pointer
        forEachGroup(group) writeCollisionTriangleIndexList(dev, triangleIntGridMap.value(group)); //Collision triangle index list
        forEachGroupChildType(const Scene::GoalSceneNode*, node) writeGoal(dev, node); //Goals
        forEachGroupChildType(const Scene::BumperSceneNode*, node) writeBumper(dev, node); //Bumpers
        forEachGroupChildType(const Scene::JamabarSceneNode*, node) writeJamabar(dev, node); //Jamabars
        forEachGroupChildType(const Scene::BananaSceneNode*, node) writeBanana(dev, node); //Bananas
        forEachGroupChildType(const Scene::ConeCollisionObjectSceneNode*, node) writeConeCollisionObject(dev, node); //ConeCollisionObjects
        forEachGroupChildType(const Scene::SphereCollisionObjectSceneNode*, node) writeSphereCollisionObject(dev, node); //SphereCollisionObjects
        forEachGroupChildType(const Scene::CylinderCollisionObjectSceneNode*, node) writeCylinderCollisionObject(dev, node); //CylinderCollisionObjects
        forEachGroupChildType(const Scene::SwitchSceneNode*, node) writeSwitch(dev, node); //Switches
        forEachGroupChildType(const Scene::WormholeSceneNode*, node) writeWormhole(dev, node); //Wormholes
        forEachGroup(group) writeLevelModelPointerAList(dev, group); //Level model pointers type A
        forEachGroup(group) writeLevelModelPointerBList(dev, group); //Level model pointers type B
        forEachGroup(group) writeLevelModelList(dev, group); //Level models
        forEachGroup(group) writeLevelModelNameList(dev, group); //Level model names     
        forEachBg(mesh) writeBackgroundModel(dev, mesh); //Background models
        forEachBg(mesh) writeBackgroundName(dev, mesh); //Background model names
        forEachFg(mesh) writeForegroundModel(dev, mesh); // Foreground models
        forEachFg(mesh) writeForegroundName(dev, mesh); // Foreground model names
        forEachBg(mesh) writeBgFgAnimationHeader(dev, mesh->getTransformAnimation()); // Background animation headers
        forEachFg(mesh) writeBgFgAnimationHeader(dev, mesh->getTransformAnimation()); // Foreground animation headers
        forEachGroup(group) writeAnimationHeader(dev, group->getTransformAnimation()); // Item group animation headers
        forEachBg(mesh) writeEffectHeader(dev, mesh); // Background effect headers
        forEachFg(mesh) writeEffectHeader(dev, mesh); // Foreground effect headers
        forEachBg(mesh) writeTextureScroll(dev, mesh); // Background texture scroll
        forEachFg(mesh) writeTextureScroll(dev, mesh); // Foreground texture scroll
        forEachGroup(group) writeTextureScroll(dev, group); // Item group texture scroll
        forEachBg(mesh) writeEffectAnimation(dev, mesh->getEffectAnimation());
        forEachFg(mesh) writeEffectAnimation(dev, mesh->getEffectAnimation());
        forEachBg(mesh) writeTransformAnimation(dev, mesh->getTransformAnimation(), true); // Background object animations (scaling)
        forEachFg(mesh) writeTransformAnimation(dev, mesh->getTransformAnimation(), true); // Foreground object animations (scaling)
        forEachGroup(group) writeTransformAnimation(dev, group->getTransformAnimation(), false); // Item group animations (no scaling)
        forEachGroup(group) writeRuntimeReflectiveModelList(dev, group); //Runtime reflective models
        forEachGroupChildType(const Scene::FalloutVolumeSceneNode*, node) writeFalloutVolume(dev, node); //Fallout volumes
        writeNull(dev, 64); // Taking a page out of Deluxe's book - having extra null padding at the end of the file can prevent weird crashes
    }

    void SMB2LzExporter::addCollisionTriangles(
            const Scene::SceneNode *node,
            QVector<Model::Vertex> &allVertices,
            QVector<unsigned int> &allIndices
            ) {
        if (dynamic_cast<const Scene::MeshCollisionSceneNode*>(node)) {
            const Scene::MeshCollisionSceneNode *coli= static_cast<const Scene::MeshCollisionSceneNode*>(node);
            //First, find the MeshSceneNode in the models QHash
            if (models.contains(coli->getMeshName())) {
                const Resource::ResourceMesh *mesh = models.value(coli->getMeshName());
                //Now loop over all MeshSegements, and add to nextOffset
                foreach(const Model::MeshSegment *seg, mesh->getMeshSegments()) {
                    //Also need to add the previous size of allVertices to the indices to be added
                    int prevSize = allVertices.size();
                    allVertices.append(seg->getVertices());

                    foreach(unsigned int ind, seg->getIndices()) {
                        allIndices.append(ind + prevSize);
                    }
                }
            } else {
                //models QHash doesn't have the mesh in question - don't add it
                qWarning().noquote() << "Missing mesh for collision" << coli->getMeshName();
            }
        }

        //Loop over all children, looking for MeshCollisionSceneNodes
        foreach(const Scene::SceneNode *child, node->getChildren()) {
            addCollisionTriangles(child, allVertices, allIndices);
        }
    }

    void SMB2LzExporter::optimizeCollision(const Stage &stage) {
        //First check what triangles intersect which grid times, in order to optimize collision
        qInfo() << "Now optimizing collision... This may take a little while";
        QElapsedTimer timer; //Measure how long this operation takes - probably a little while
        timer.start();

        QAtomicInt finishedThreads = 0;
        int maxThreads = 0;

        //Loop over all collision headers
        foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (dynamic_cast<Scene::GroupSceneNode*>(node)) {
                ++maxThreads;

                Scene::GroupSceneNode *groupNode = static_cast<Scene::GroupSceneNode*>(node);
                //Find all MeshCollisionSceneNodes, and add the triangles to allVertices/allIndices
                QVector<Model::Vertex> allVertices;
                QVector<unsigned int> allIndices;
                addCollisionTriangles(node, allVertices, allIndices);

                //Now create the TriangleIntrsectionGrid, which will check each triangle for intersections with each grid tile
                //Also multithread this!
                TriangleIntersectionGrid *intGrid = new TriangleIntersectionGrid(
                        allVertices,
                        groupNode->getCollisionGrid()
                        );

                class SortTask : public QRunnable {
                    public:
                        TriangleIntersectionGrid *intGrid;
                        QVector<unsigned int> indices;
                        CollisionGrid *grid;
                        QAtomicInt *finishedThreadsCounter;

                        virtual void run() override {
                            intGrid->sortIntersections(indices, *grid);
                            ++(*finishedThreadsCounter);
                        }
                };

                SortTask *task = new SortTask;
                task->intGrid = intGrid;
                task->indices = allIndices;
                task->grid = &groupNode->getCollisionGrid();
                task->finishedThreadsCounter = &finishedThreads;
                QThreadPool::globalInstance()->start(task);

                //Store it
                triangleIntGridMap[groupNode] = intGrid;
            }
        }

        int sleepTime = 1;
        while(finishedThreads < maxThreads) {
            printf("\r[");
            float percent = (float) finishedThreads / maxThreads;
            for (int i = 0; i < 24; i++) {
                if ((float) i / 24 < percent) {
                    printf("#");
                } else {
                    printf(" ");
                }
            }
            printf("] %d / %d item groups optimized", finishedThreads.load(), maxThreads);
            fflush(stdout);

            QThread::msleep(sleepTime); //Don't max out a CPU core on this progress reporting loop
            if (sleepTime < 500) ++sleepTime; //Start off with rapid checking but slow down to a maximum delay of 0.5s
        }
        printf("\r[");
        for (int i = 0; i < 24; i++) printf("#");
        printf("] %d / %d item groups optimized\n", finishedThreads.load(), maxThreads);
        fflush(stdout);

        //Finished - log the amount of time it took
        qInfo().noquote().nospace() << "Finished optimizing collision in " << timer.nsecsElapsed() / 1000000000.0f << "s";
    }

    void SMB2LzExporter::calculateOffsets(const Stage &stage) {
        quint32 nextOffset = FILE_HEADER_LENGTH;

        // This is for start position(s) (party games can have multiple start positions)
        startOffset = nextOffset;
        foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (Scene::StartSceneNode *start = dynamic_cast<Scene::StartSceneNode*>(node)) {
                nextOffset += START_LENGTH;
            }
        }

        falloutOffset = nextOffset;
        nextOffset += FALLOUT_LENGTH;

        // This is for the fog header (if the stage has custom fog)
        Fog *fog = stage.getFog();
        if (fog != nullptr) {
            fogOffset = nextOffset;
            nextOffset += FOG_LENGTH;
        }
        else fogOffset = 0;

        // This is for the fog animation header and keyframe list (if it exists)
        Animation::FogAnimation *fogAnim = stage.getFogAnimation();
        if (fogAnim != nullptr) {
            fogAnimationHeaderOffset = nextOffset;
            nextOffset += FOG_ANIMATION_HEADER_LENGTH;

            fogAnimStartKeyframesOffsetMap.insert(nextOffset, fogAnim);
            nextOffset += ANIMATION_KEYFRAME_LENGTH * fogAnim->getStartDistanceKeyframes().size();
            fogAnimEndKeyframesOffsetMap.insert(nextOffset, fogAnim);
            nextOffset += ANIMATION_KEYFRAME_LENGTH * fogAnim->getEndDistanceKeyframes().size();
            fogAnimRedKeyframesOffsetMap.insert(nextOffset, fogAnim);
            nextOffset += ANIMATION_KEYFRAME_LENGTH * fogAnim->getRedKeyframes().size();
            fogAnimGreenKeyframesOffsetMap.insert(nextOffset, fogAnim);
            nextOffset += ANIMATION_KEYFRAME_LENGTH * fogAnim->getGreenKeyframes().size();
            fogAnimBlueKeyframesOffsetMap.insert(nextOffset, fogAnim);
            nextOffset += ANIMATION_KEYFRAME_LENGTH * fogAnim->getBlueKeyframes().size();
            fogAnimUnknownKeyframesOffsetMap.insert(nextOffset, fogAnim);
            nextOffset += ANIMATION_KEYFRAME_LENGTH * fogAnim->getUnknownKeyframes().size();
        }
        else fogAnimationHeaderOffset = 0;

        // This is for Monkey Race-specific offsets
        if (stage.getStageType() == EnumStageType::MONKEY_RACE_2) {
            monkeyRaceHeaderOffset = nextOffset;
            nextOffset += MONKEY_RACE_HEADER_LENGTH;
            cpuTrackPathHeaderOffset = nextOffset;
            nextOffset += CPU_TRACK_PATH_HEADER_LENGTH;

            // Track path offsets
            foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
                if (Scene::RaceTrackPathSceneNode *path = dynamic_cast<Scene::RaceTrackPathSceneNode*>(node)) {
                    //PosX
                    raceTrackPathPosXKeyframesOffsetMap.insert(nextOffset, path->getTrackPath());
                    nextOffset += ANIMATION_KEYFRAME_LENGTH * path->getTrackPath()->getPosXKeyframes().size();
                    //PosY
                    raceTrackPathPosYKeyframesOffsetMap.insert(nextOffset, path->getTrackPath());
                    nextOffset += ANIMATION_KEYFRAME_LENGTH * path->getTrackPath()->getPosYKeyframes().size();
                    //PosZ
                    raceTrackPathPosZKeyframesOffsetMap.insert(nextOffset, path->getTrackPath());
                    nextOffset += ANIMATION_KEYFRAME_LENGTH * path->getTrackPath()->getPosZKeyframes().size();
                }
            }

            // Booster offsets
            foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
                if (Scene::BoosterSceneNode *group = dynamic_cast<Scene::BoosterSceneNode*>(node)) {
                    boosterOffsetMap.insert(nextOffset, group);
                    nextOffset += BOOSTER_LENGTH;
                }
            }
        }
        else monkeyRaceHeaderOffset = 0;

        // This is for Monkey Golf-specific offsets
        if (stage.getStageType() == EnumStageType::MONKEY_GOLF_2) {
            golfHoleOffset = nextOffset;
            nextOffset += GOLF_HOLE_LENGTH;
        }
        else golfHoleOffset = 0;

        //Find all GroupSceneNodes/Collision headers
        foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (Scene::GroupSceneNode *group = dynamic_cast<Scene::GroupSceneNode*>(node)) {
                //Found one
                collisionHeaderOffsetMap.insert(nextOffset, group);
                nextOffset += COLLISION_HEADER_LENGTH;
            }
        }

        //Iterate over all GroupSceneNodes/collision headers, and call addCollisionTriangleOffsets with them
        //This is for Collision Triangle data
        forEachGroup(group) {
            gridTriangleListOffsetMap.insert(nextOffset, group);
            addCollisionTriangleOffsets(group, nextOffset);
        }

        //Iterate over all GroupSceneNodes/collision headers, and fill the gridTriangleListPointersOffsetMap
        //This is for Collision Triangle Pointers
        forEachGroup(group) {
            gridTriangleListPointersOffsetMap.insert(nextOffset, group);
            const CollisionGrid &grid = group->getCollisionGrid();
            nextOffset += COLLISION_TRIANGLE_LIST_POINTER_LENGTH * grid.getGridStepCount().x * grid.getGridStepCount().y;
        }

        //Iterate over all GroupSceneNodes/collision headers, and fill the gridTriangleListOffsetMap
        //This is for the Collision Triangle Index List
        forEachGroup(group) {
            gridTriangleListOffsetMap.insert(nextOffset, group);

            //2D vector (X, Y) containing vectors of triangle indices
            QVector<QVector<QVector<quint16>>> indicesGrid = triangleIntGridMap.value(group)->getIndicesGrid();

            //Loop over all vectors
            for (int x = 0; x < indicesGrid.size(); x++) {
                for (int y = 0; y < indicesGrid[x].size(); y++) {
                    if (indicesGrid[x][y].size() == 0) {
                        //If this grid tile has zero triangles to collide with, just add a null offset to save file size
                        gridTriangleIndexListOffsetMap[group].append(0x00000000);
                    } else {
                        gridTriangleIndexListOffsetMap[group].append(nextOffset);

                        nextOffset += COLLISION_TRIANGLE_INDEX_LENGTH * (indicesGrid[x][y].size());
                        //Add an extra index length for the 0xFFFF list terminator
                        nextOffset += COLLISION_TRIANGLE_INDEX_LENGTH;
                    }
                }
            }

            //Keep it 4 byte aligned
            nextOffset = roundUpNearest4(nextOffset);
        }

        //Iterate over all GroupSceneNodes/collision headers, and count goals to add to nextOffset
        //Additionally, this should avoid the no extra points glitch, as the 1st collision header should point to a
        //goal list regardless of whether it has any goals or not
        forEachGroup(group) {
            goalOffsetMap.insert(nextOffset, group);
            quint32 goalCount = 0; //Number of goals in this collision header

            forEachChildType(group, Scene::GoalSceneNode*, node) {
                nextOffset += GOAL_LENGTH;
                goalCount++;
            }

            //Store goal count in the map
            goalCountMap[group] = goalCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count bumpers to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            bumperOffsetMap.insert(nextOffset, group);
            quint32 bumperCount = 0; //Number of bumpers in this collision header

            forEachChildType(group, Scene::BumperSceneNode*, node) {
                nextOffset += BUMPER_LENGTH;
                bumperCount++;
            }

            //Store bumper count in the map
            bumperCountMap[group] = bumperCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count jamabars to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            jamabarOffsetMap.insert(nextOffset, group);
            quint32 jamabarCount = 0; //Number of jamabars in this collision header

            forEachChildType(group, Scene::JamabarSceneNode*, node) {
                nextOffset += JAMABAR_LENGTH;
                jamabarCount++;
            }

            //Store jamabar count in the map
            jamabarCountMap[group] = jamabarCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count bananas to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            bananaOffsetMap.insert(nextOffset, group);
            quint32 bananaCount = 0; //Number of bananas in this collision header

            forEachChildType(group, Scene::BananaSceneNode*, node) {
                nextOffset += BANANA_LENGTH;
                bananaCount++;
            }

            //Store banana count in the map
            bananaCountMap[group] = bananaCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count coneCollisionObjects to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            coneCollisionObjectOffsetMap.insert(nextOffset, group);
            quint32 coneCollisionObjectCount = 0; //Number of coneCollisionObjects in this collision header

            forEachChildType(group, Scene::ConeCollisionObjectSceneNode*, node) {
                nextOffset += CONE_COLLISION_LENGTH;
                coneCollisionObjectCount++;
            }

            //Store coneCollisionObject count in the map
            coneCollisionObjectCountMap[group] = coneCollisionObjectCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count sphereCollisionObjects to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            sphereCollisionObjectOffsetMap.insert(nextOffset, group);
            quint32 sphereCollisionObjectCount = 0; //Number of sphereCollisionObjects in this collision header

            forEachChildType(group, Scene::SphereCollisionObjectSceneNode*, node) {
                nextOffset += SPHERE_COLLISION_LENGTH;
                sphereCollisionObjectCount++;
            }

            //Store sphereCollisionObject count in the map
            sphereCollisionObjectCountMap[group] = sphereCollisionObjectCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count cylinderCollisionObjects to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            cylinderCollisionObjectOffsetMap.insert(nextOffset, group);
            quint32 cylinderCollisionObjectCount = 0; //Number of cylinderCollisionObjects in this collision header

            forEachChildType(group, Scene::CylinderCollisionObjectSceneNode*, node) {
                nextOffset += CYLINDER_COLLISION_LENGTH;
                cylinderCollisionObjectCount++;
            }

            //Store cylinderCollisionObject count in the map
            cylinderCollisionObjectCountMap[group] = cylinderCollisionObjectCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count switches to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            switchOffsetMap.insert(nextOffset, group);
            quint32 switchCount = 0; //Number of switches in this collision header

            forEachChildType(group, Scene::SwitchSceneNode*, node) {
                nextOffset += SWITCH_LENGTH;
                switchCount++;
            }

            //Store switch count in the map
            switchCountMap[group] = switchCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count wormholes to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            wormholeOffsetMap.insert(nextOffset, group);
            quint32 wormholeCount = 0; //Number of wormholes in this collision header

            forEachChildType(group, Scene::WormholeSceneNode*, node) {
                //Need to store the offset of every wormhole, so that they can be linked together (by offset) later
                wormholeIndividualOffsetMap.insert(nextOffset, node->getUuid());

                nextOffset += WORMHOLE_LENGTH;
                wormholeCount++;
            }

            //Store wormhole count in the map
            wormholeCountMap[group] = wormholeCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count level models to add to nextOffset
        //For level model pointer type A
        //Basically the exact same as before with goals
        forEachGroup(group) {
            levelModelPointerAOffsetMap.insert(nextOffset, group);

            forEachChildType(group, Scene::MeshSceneNode*, node) {
                if (!node->isRuntimeReflective()) {
                    nextOffset += LEVEL_MODEL_POINTER_TYPE_A_LENGTH;
                }
            }
        }

        //Iterate over all GroupSceneNodes/collision headers, and count level models to add to nextOffset
        //For level model pointer type B
        //Basically the exact same as before with goals
        forEachGroup(group) {
            levelModelPointerBOffsetMap.insert(nextOffset, group);

            forEachChildType(group, Scene::MeshSceneNode*, node) {
                if (!node->isRuntimeReflective()) {
                    nextOffset += LEVEL_MODEL_POINTER_TYPE_B_LENGTH;
                }
            }
        }

        //Iterate over all GroupSceneNodes/collision headers, and count level models to add to nextOffset
        //Basically the exact same as before with goals
        forEachGroup(group) {
            levelModelOffsetMap.insert(nextOffset, group);
            quint32 levelModelCount = 0; //Number of levelModels in this collision header

            forEachChildType(group, Scene::MeshSceneNode*, node) {
                if (!node->isRuntimeReflective()) {
                    nextOffset += LEVEL_MODEL_LENGTH;
                    levelModelCount++;
                }
            }

            //Store levelModel count in the map
            levelModelCountMap[group] = levelModelCount;
        }

        //Iterate over all level models, and add the model name + null terminator padded to 4 bytes to nextOffset
        forEachGroup(group) {
            forEachChildType(group, Scene::MeshSceneNode*, node) {
                levelModelNameOffsetMap.insert(nextOffset, node->getMeshName());

                //+ 1 because size() does not include a null terminator
                nextOffset += roundUpNearest4(node->getMeshName().size() + 1);
            }
        }

        //Find all background models
        foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (dynamic_cast<Scene::BackgroundGroupSceneNode*>(node)) {
                Scene::BackgroundGroupSceneNode *group = static_cast<Scene::BackgroundGroupSceneNode*>(node);

                //Found one, now iterate over all children
                foreach(Scene::SceneNode *bg, group->getChildren()) {
                    if (Scene::MeshSceneNode *mesh = dynamic_cast<Scene::MeshSceneNode*>(bg)) {
                        //Found a background mesh
                        //Add it to a map
                        bgOffsetMap.insert(nextOffset, mesh);
                        nextOffset += BACKGROUND_MODEL_LENGTH;
                    } else {
                        qWarning() << "There's a non-MeshSceneNode within a background group. This should never happen. Ignoring for now.";
                    }
                }
            }
        }

        //Iterate over all background models, and add the model name + null terminator padded to 4 bytes to nextOffset
        forEachBg(node) {
            bgNameOffsetMap.insert(nextOffset, node->getMeshName());

            //+ 1 because size() does not include a null terminator
            nextOffset += roundUpNearest4(node->getMeshName().size() + 1);
        }

        //Find all foreground models
        foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (dynamic_cast<Scene::ForegroundGroupSceneNode*>(node)) {
                Scene::ForegroundGroupSceneNode *group = static_cast<Scene::ForegroundGroupSceneNode*>(node);

                //Found one, now iterate over all children
                foreach(Scene::SceneNode *fg, group->getChildren()) {
                    if (Scene::MeshSceneNode *mesh = dynamic_cast<Scene::MeshSceneNode*>(fg)) {
                        //Found a foreground mesh
                        //Add it to a map
                        fgOffsetMap.insert(nextOffset, mesh);
                        nextOffset += BACKGROUND_MODEL_LENGTH;
                    } else {
                        qWarning() << "There's a non-MeshSceneNode within a foreground group. This should never happen. Ignoring for now.";
                    }
                }
            }
        }

        //Iterate over all foreground models, and add the model name + null terminator padded to 4 bytes to nextOffset
        forEachFg(node) {
            fgNameOffsetMap.insert(nextOffset, node->getMeshName());

            //+ 1 because size() does not include a null terminator
            nextOffset += roundUpNearest4(node->getMeshName().size() + 1);
        }


        //BG animation headers
        forEachBg(group) {
            if (group->getTransformAnimation() != nullptr) {
                //This node has animation
                bgAnimHeaderOffsetMap.insert(nextOffset, group);
                nextOffset += BACKGROUND_ANIMATION_HEADER_LENGTH;
            }
        }

        //FG animation headers
        forEachFg(group) {
            if (group->getTransformAnimation() != nullptr) {
                //This node has animation
                fgAnimHeaderOffsetMap.insert(nextOffset, group);
                nextOffset += BACKGROUND_ANIMATION_HEADER_LENGTH;
            }
        }

        //Animation headers
        forEachGroup(group) {
            if (group->getTransformAnimation() != nullptr) {
                //This node has animation
                groupAnimHeaderOffsetMap.insert(nextOffset, group);
                nextOffset += ANIMATION_HEADER_LENGTH;
            }
        }

        //BG effect headers
        forEachBg(node) {
            effectHeaderOffsetMap.insert(nextOffset, node);
            nextOffset += EFFECT_HEADER_LENGTH;
        }

        //FG effect headers
        forEachFg(node) {
            effectHeaderOffsetMap.insert(nextOffset, node);
            nextOffset += EFFECT_HEADER_LENGTH;
        }

        //BG texture Scroll
        forEachBg(node) {
            textureScrollOffsetMap.insert(nextOffset, node);
            nextOffset += TEXTURE_SCROLL_LENGTH;
        }

        //FG texture Scroll
        forEachFg(node) {
            textureScrollOffsetMap.insert(nextOffset, node);
            nextOffset += TEXTURE_SCROLL_LENGTH;
        }

        //IG texture scroll
        forEachGroup(group) {
            textureScrollOffsetMap.insert(nextOffset, group);
            nextOffset += TEXTURE_SCROLL_LENGTH;
        }

        //BG effect keyframes
        forEachBg(node) {
            Animation::EffectAnimation *anim = node->getEffectAnimation();
            if (anim != nullptr) {
                effectAnimType1KeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += EFFECT_TYPE_1_KEYFRAME_LENGTH * anim->getEffect1Keyframes().size();

                effectAnimType2KeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += EFFECT_TYPE_2_KEYFRAME_LENGTH * anim->getEffect2Keyframes().size();
            }
        }
        //FG effect keyframes
        forEachFg(node) {
            Animation::EffectAnimation *anim = node->getEffectAnimation();
            if (anim != nullptr) {
                effectAnimType1KeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += EFFECT_TYPE_1_KEYFRAME_LENGTH * anim->getEffect1Keyframes().size();

                effectAnimType2KeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += EFFECT_TYPE_2_KEYFRAME_LENGTH * anim->getEffect2Keyframes().size();
            }
        }

        //BG animation keyframes
        forEachBg(group) {
            const Animation::TransformAnimation *anim = group->getTransformAnimation();

            if (anim != nullptr) {
                //This node has animation

                //ScaleX
                animScaleXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getScaleXKeyframes().size();
                //ScaleY
                animScaleYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getScaleYKeyframes().size();
                //ScaleZ
                animScaleZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getScaleZKeyframes().size();

                //RotX
                animRotXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotXKeyframes().size();
                //RotY
                animRotYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotYKeyframes().size();
                //RotZ
                animRotZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotZKeyframes().size();

                //PosX
                animPosXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosXKeyframes().size();
                //PosY
                animPosYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosYKeyframes().size();
                //PosZ
                animPosZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosZKeyframes().size();


            }
        }

        //FG animation keyframes
        forEachFg(group) {
            const Animation::TransformAnimation *anim = group->getTransformAnimation();

            if (anim != nullptr) {
                //This node has animation

                //ScaleX
                animScaleXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getScaleXKeyframes().size();
                //ScaleY
                animScaleYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getScaleYKeyframes().size();
                //ScaleZ
                animScaleZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getScaleZKeyframes().size();

                //RotX
                animRotXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotXKeyframes().size();
                //RotY
                animRotYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotYKeyframes().size();
                //RotZ
                animRotZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotZKeyframes().size();

                //PosX
                animPosXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosXKeyframes().size();
                //PosY
                animPosYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosYKeyframes().size();
                //PosZ
                animPosZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosZKeyframes().size();

            }
        }

        //Animation keyframes
        forEachGroup(group) {
            const Animation::TransformAnimation *anim = group->getTransformAnimation();

            if (anim != nullptr) {
                //This node has animation

                //RotX
                animRotXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotXKeyframes().size();
                //RotY
                animRotYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotYKeyframes().size();
                //RotZ
                animRotZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getRotZKeyframes().size();

                //PosX
                animPosXKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosXKeyframes().size();
                //PosY
                animPosYKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosYKeyframes().size();
                //PosZ
                animPosZKeyframesOffsetMap.insert(nextOffset, anim);
                nextOffset += ANIMATION_KEYFRAME_LENGTH * anim->getPosZKeyframes().size();

            }
        }

        //Reflective model names
        // forEachGroup(group) {
        //     forEachChildType(group, Scene::MeshSceneNode*, node) {
        //         if (node->isRuntimeReflective()) {
        //             runtimeReflectiveModelNameOffsetMap.insert(nextOffset, node->getMeshName());

        //             //+ 1 because size() does not include a null terminator
        //             nextOffset += roundUpNearest4(node->getMeshName().size() + 1);
        //         }
        //     }
        // }

        //Reflective models
        forEachGroup(group) {
            runtimeReflectiveModelOffsetMap.insert(nextOffset, group);
            quint32 runtimeReflectiveModelCount = 0; //Number of reflective models in this collision header

            forEachChildType(group, Scene::MeshSceneNode*, node) {
                if (node->isRuntimeReflective()) {
                    nextOffset += RUNTIME_REFLECTIVE_MODEL_LENGTH;
                    runtimeReflectiveModelCount++;
                }
            }

            runtimeReflectiveModelCountMap[group] = runtimeReflectiveModelCount;
        }

        //Fallout volumes
        forEachGroup(group) {
            falloutVolumeOffsetMap.insert(nextOffset, group);
            quint32 falloutVolumeCount = 0; //Number of fallout volumes in this collision header

            forEachChildType(group, Scene::FalloutVolumeSceneNode*, node) {
                nextOffset += FALLOUT_VOLUME_LENGTH;
                falloutVolumeCount++;
            }

            falloutVolumeCountMap[group] = falloutVolumeCount;
        }

        //TODO: Reflective level model
        //TODO: Level model instances
    }

    void SMB2LzExporter::addCollisionTriangleOffsets(const Scene::SceneNode *node, quint32 &nextOffset) {
        //TODO: Store this in a map or hash or something, per collision header, add a function argument for the collision header, store in a nested maps - Collision header, object name, offset
        if (dynamic_cast<const Scene::MeshCollisionSceneNode*>(node)) {
            const Scene::MeshCollisionSceneNode *coli= static_cast<const Scene::MeshCollisionSceneNode*>(node);
            //First, find the MeshSceneNode in the models QHash
            if (models.contains(coli->getMeshName())) {
                const Resource::ResourceMesh *mesh = models.value(coli->getMeshName());
                //Now loop over all MeshSegements, and add to nextOffset
                foreach (const Model::MeshSegment *seg, mesh->getMeshSegments()) {
                    int triangleCount = seg->getIndices().size() / 3;
                    nextOffset += COLLISION_TRIANGLE_LENGTH * triangleCount;
                }
            } /* else {
                //models QHash doesn't have the mesh in question - don't add it
                //qWarning().noquote() << "Missing mesh for collision" << coli->getMeshName();
                //Don't warn - This is done in addCollisionTriangles
            } */
        }

        //Loop over all children, looking for MeshCollisionSceneNodes
        foreach(const Scene::SceneNode *child, node->getChildren()) {
            addCollisionTriangleOffsets(child, nextOffset);
        }
    }

    void SMB2LzExporter::writeFileHeader(QDataStream &dev) {
        quint32 goalCount = addAllCounts(goalCountMap);
        quint32 bumperCount = addAllCounts(bumperCountMap);
        quint32 jamabarCount = addAllCounts(jamabarCountMap);
        quint32 bananaCount = addAllCounts(bananaCountMap);
        quint32 coneCollisionObjectCount = addAllCounts(coneCollisionObjectCountMap);
        quint32 sphereCollisionObjectCount = addAllCounts(sphereCollisionObjectCountMap);
        quint32 cylinderCollisionObjectCount = addAllCounts(cylinderCollisionObjectCountMap);
        quint32 wormholeCount = addAllCounts(runtimeReflectiveModelCountMap);
        quint32 falloutVolumeCount = addAllCounts(falloutVolumeCountMap);

        writeNull(dev, 4); dev << 0x447A0000; //Magic number (Probably)
        dev << (quint32) collisionHeaderOffsetMap.size();
        dev << (quint32) (collisionHeaderOffsetMap.size() > 0 ? collisionHeaderOffsetMap.firstKey() : 0);
        dev << startOffset;
        dev << falloutOffset;
        dev << goalCount;
        //firstKey will crash if the list is empty - so just use 0 if size() is not > 0
        dev << (quint32) (goalCount > 0 ? goalOffsetMap.firstKey() : 0); //Goal list offset
        dev << bumperCount;
        dev << (quint32) (bumperCount > 0 ? bumperOffsetMap.firstKey() : 0); //Bumper list offset
        dev << jamabarCount;
        dev << (quint32) (jamabarCount > 0 ? jamabarOffsetMap.firstKey() : 0); //Jamabar list offset
        dev << bananaCount;
        dev << (quint32) (bananaCount > 0 ? bananaOffsetMap.firstKey() : 0); //Banana list offset
        dev << coneCollisionObjectCount;
        dev << (quint32) (coneCollisionObjectCount > 0 ? coneCollisionObjectOffsetMap.firstKey() : 0); //Banana list offset
        dev << sphereCollisionObjectCount;
        dev << (quint32) (sphereCollisionObjectCount > 0 ? sphereCollisionObjectOffsetMap.firstKey() : 0); //Banana list offset
        dev << cylinderCollisionObjectCount;
        dev << (quint32) (cylinderCollisionObjectCount > 0 ? cylinderCollisionObjectOffsetMap.firstKey() : 0); //Banana list offset
        dev << falloutVolumeCount;
        dev << (quint32) (falloutVolumeCount > 0 ? falloutVolumeOffsetMap.firstKey() : 0); //Fallout volume list offset
        dev << (quint32) bgOffsetMap.size();
        dev << (quint32) (bgOffsetMap.size() > 0 ? bgOffsetMap.firstKey() : 0); //Background list offset
        dev << (quint32) fgOffsetMap.size();
        dev << (quint32) (fgOffsetMap.size() > 0 ? fgOffsetMap.firstKey() : 0); //Foreground list offset
        dev << (quint32) monkeyRaceHeaderOffset;
        dev << (quint32) stageType;
        dev << wormholeCount;
        //We have to write 0 here if we want wormhole surfaces to work
        //This also means wormholes + reflective surfaces won't co-operate
        dev << (quint32) (wormholeCount > 0 ? runtimeReflectiveModelOffsetMap.firstKey() : 0); //Reflective model list offset
        dev << (quint32) golfHoleOffset;
        writeNull(dev, 8);
        writeNull(dev, 8); //TODO: Level model instances
        dev << addAllCounts(levelModelCountMap);
        dev << levelModelPointerAOffsetMap.firstKey();
        dev << addAllCounts(levelModelCountMap);
        dev << levelModelPointerBOffsetMap.firstKey();
        writeNull(dev, 12);
        dev << addAllCounts(switchCountMap);
        dev << (quint32) (switchOffsetMap.size() > 0 ? switchOffsetMap.firstKey() : 0); //Switch list offset
        dev << (quint32) fogAnimationHeaderOffset;
        dev << addAllCounts(wormholeCountMap);
        dev << (quint32) (wormholeOffsetMap.size() > 0 ? wormholeOffsetMap.firstKey() : 0); //Wormhole list offset
        dev << (quint32) fogOffset;
        writeNull(dev, 20);
        writeNull(dev, 4); //TODO: Mystery 3
        writeNull(dev, 1988);
    }

    void SMB2LzExporter::writeStart(QDataStream &dev, const Stage &stage) {
        QMultiMap<quint32, Scene::StartSceneNode*> startPositions;

        //Find the start position(s)
        forEachChildType(stage.getRootNode(), Scene::StartSceneNode*, node) {
            if (dynamic_cast<Scene::StartSceneNode*>(node)) {
                Scene::StartSceneNode* startPos = static_cast<Scene::StartSceneNode*>(node);
                startPositions.insert(startPos->getPlayerID(), startPos);
            }
        }

        //Write the bytes
        for (int playerId = startPositions.size()-1; playerId >= 0; playerId--) {
            Scene::StartSceneNode *start = startPositions.value(playerId);
            dev << start->getPosition();
            dev << convertRotation(start->getRotation());
            writeNull(dev, 2);
        }

    }

    void SMB2LzExporter::writeFallout(QDataStream &dev, const Stage &stage) {
        dev << stage.getFalloutY();
    }

    void SMB2LzExporter::writeCollisionHeader(QDataStream &dev, const Scene::GroupSceneNode *node) {
        //Will be nullptr if this node has no animation
        const Animation::TransformAnimation *anim = node->getTransformAnimation();

        dev << node->getOriginPosition(); //Center of rotation
        dev << convertRotation(node->getOriginRotation()); //Center of rotation

        //Animation loop type/seesaw
        if (node->getAnimationSeesawType() == SEESAW) {
            dev << (quint16) 0x0002;
        } else {
            if (anim != nullptr && anim->getLoopType() == Animation::PLAY_ONCE) {
                dev << (quint16) 0x0001;
            } else {
                dev << (quint16) 0x0000;
            }
        }

        dev << (anim != nullptr ? groupAnimHeaderOffsetMap.key(node) : (quint32) 0); //Offset to animation header

        dev << node->getConveyorSpeed();
        dev << gridTriangleListOffsetMap.key(node);
        dev << gridTriangleListPointersOffsetMap.key(node);
        dev << node->getCollisionGrid().getGridStart();
        dev << node->getCollisionGrid().getGridStep();
        dev << node->getCollisionGrid().getGridStepCount();
        dev << goalCountMap.value(node);
        dev << goalOffsetMap.key(node);
        dev << bumperCountMap.value(node);
        dev << bumperOffsetMap.key(node);
        dev << jamabarCountMap.value(node);
        dev << jamabarOffsetMap.key(node);
        dev << bananaCountMap.value(node);
        dev << bananaOffsetMap.key(node);
        dev << coneCollisionObjectCountMap.value(node);
        dev << coneCollisionObjectOffsetMap.key(node);
        dev << sphereCollisionObjectCountMap.value(node);
        dev << sphereCollisionObjectOffsetMap.key(node);
        dev << cylinderCollisionObjectCountMap.value(node);
        dev << cylinderCollisionObjectOffsetMap.key(node);
        dev << falloutVolumeCountMap.value(node);
        dev << falloutVolumeOffsetMap.key(node);
        dev << runtimeReflectiveModelCountMap.value(node);
        dev << runtimeReflectiveModelOffsetMap.key(node);
        writeNull(dev, 8); //TODO: Everything else
        dev << levelModelCountMap.value(node);
        dev << levelModelPointerBOffsetMap.key(node);
        writeNull(dev, 8); //Unknown/Null
        dev << (quint16) node->getAnimationGroupId();
        writeNull(dev, 2); //Null padding
        dev << switchCountMap.value(node);
        dev << switchOffsetMap.key(node);
        writeNull(dev, 4); //Unknown/Null (Mystery 5 count?)
        writeNull(dev, 4); //TODO: Offset to mystery 5
        dev << node->getSeesawSensitivity();
        dev << node->getSeesawFriction();
        dev << node->getSeesawSpring();
        dev << wormholeCountMap.value(node);
        dev << wormholeOffsetMap.key(node);

        //Initial animation state
        if (node->getTransformAnimation() == nullptr) {
            writeNull(dev, 4);
        } else {
            dev << node->getTransformAnimation()->getInitialState();
        }

        writeNull(dev, 4); //TODO: Unknown/Null
        dev << (anim != nullptr ? anim->getLoopTime() : (quint32) 0); //Anim loop time
        dev << textureScrollOffsetMap.key(node);
        writeNull(dev, 960); //TODO: Everything else
    }

    void SMB2LzExporter::writeCollisionTriangleIndexList(QDataStream &dev, const TriangleIntersectionGrid *intGrid) {
        unsigned int bytesWritten = 0;

        //This will write a list of collision triangle indices per grid tile
        for (int x = 0; x < intGrid->getIndicesGrid().size(); x++) {
            for (int y = 0; y < intGrid->getIndicesGrid()[x].size(); y++) {
                //Don't bother writing anything if this grid tile has zero triangles
                if (intGrid->getIndicesGrid()[x][y].size() == 0) continue;

                foreach(quint16 index, intGrid->getIndicesGrid()[x][y]) {
                    dev << index;
                    bytesWritten += 2;
                }
                //Add 0xFFFF terminator
                dev << (quint16) 0xFFFF;
                bytesWritten += 2;
            }
        }

        //Keep 4 byte padded if not
        if (bytesWritten % 4 != 0) {
            writeNull(dev, 2);
            bytesWritten += 2;
        }
    }

    void SMB2LzExporter::writeCollisionTriangleIndexListPointers(QDataStream &dev, const Scene::GroupSceneNode *node) {
        unsigned int totalTiles = node->getCollisionGrid().getGridStepCount().x * node->getCollisionGrid().getGridStepCount().y;
        for (unsigned int i = 0; i < totalTiles; i++) {
            dev << gridTriangleIndexListOffsetMap[node][i];
        }
    }

    void SMB2LzExporter::writeGoal(QDataStream &dev, const Scene::GoalSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        dev << (quint8) node->getType();
        dev << (quint8) node->getCastShadow();
    }

    void SMB2LzExporter::writeBumper(QDataStream &dev, const Scene::BumperSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << node->getScale();
    }

    void SMB2LzExporter::writeJamabar(QDataStream &dev, const Scene::JamabarSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << node->getScale();
    }

    void SMB2LzExporter::writeBanana(QDataStream &dev, const Scene::BananaSceneNode *node) {
        dev << node->getPosition();
        dev << (quint32) node->getType();
    }

    void SMB2LzExporter::writeSwitch(QDataStream &dev, const Scene::SwitchSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        dev << (quint16) node->getType();
        dev << (quint16) node->getLinkedAnimGroupId();
        writeNull(dev, 2);
    }

    void SMB2LzExporter::writeWormhole(QDataStream &dev, const Scene::WormholeSceneNode *node) {
        dev << (quint32) 0x00000001;
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << wormholeIndividualOffsetMap.key(node->getDestinationUuid()); //Destination wormhole offset
    }

    void SMB2LzExporter::writeFalloutVolume(QDataStream &dev, const Scene::FalloutVolumeSceneNode *node) {
        dev << node->getPosition();
        dev << node->getScale();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
    }

    void SMB2LzExporter::writeFog(QDataStream &dev, const Fog *fog)
    {
        if (fog == nullptr) return;

        dev << (quint8) fog->getFogType();
        writeNull(dev, 3);
        dev << fog->getStartDistance();
        dev << fog->getEndDistance();
        dev << fog->getRedValue();
        dev << fog->getGreenValue();
        dev << fog->getBlueValue();
        writeNull(dev, 12);

    }

    void SMB2LzExporter::writeFogAnimationHeader(QDataStream &dev, const Animation::FogAnimation *anim)
    {
        if (anim == nullptr) return;

        dev << (quint32) anim->getStartDistanceKeyframes().size();
        dev << (quint32) fogAnimStartKeyframesOffsetMap.key(anim);
        dev << (quint32) anim->getEndDistanceKeyframes().size();
        dev << (quint32) fogAnimEndKeyframesOffsetMap.key(anim);
        dev << (quint32) anim->getRedKeyframes().size();
        dev << (quint32) fogAnimRedKeyframesOffsetMap.key(anim);
        dev << (quint32) anim->getGreenKeyframes().size();
        dev << (quint32) fogAnimGreenKeyframesOffsetMap.key(anim);
        dev << (quint32) anim->getBlueKeyframes().size();
        dev << (quint32) fogAnimBlueKeyframesOffsetMap.key(anim);
        dev << (quint32) anim->getUnknownKeyframes().size();
        dev << (quint32) fogAnimUnknownKeyframesOffsetMap.key(anim);
    }

    void SMB2LzExporter::writeRaceHeader(QDataStream &dev, const Stage &stage)
    { 
        foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (Scene::RaceTrackPathSceneNode *pathNode = dynamic_cast<Scene::RaceTrackPathSceneNode*>(node)) {
                if (pathNode->getTrackPath()->getPlayerID() == 0) {
                    dev << (quint32) pathNode->getTrackPath()->getPosXKeyframes().size();
                    dev << (quint32) raceTrackPathPosXKeyframesOffsetMap.key(pathNode->getTrackPath());
                    dev << (quint32) pathNode->getTrackPath()->getPosYKeyframes().size();
                    dev << (quint32) raceTrackPathPosYKeyframesOffsetMap.key(pathNode->getTrackPath());
                    dev << (quint32) pathNode->getTrackPath()->getPosZKeyframes().size();
                    dev << (quint32) raceTrackPathPosZKeyframesOffsetMap.key(pathNode->getTrackPath());
                }
            }
        }
        // We are making the assumption that there will *always* be 7 CPU track paths - this seems to be a requirement of the game
        dev << (quint32) 0x7;
        dev << (quint32) cpuTrackPathHeaderOffset;
        dev << (quint32) boosterOffsetMap.size();
        dev << (quint32) (boosterOffsetMap.size() > 0 ? boosterOffsetMap.firstKey() : 0);
        // There's extra stuff after the header here, not sure what it's for
        writeNull(dev, 192);

    }

    void SMB2LzExporter::writeBooster(QDataStream &dev, const Scene::BoosterSceneNode *node)
    {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
    }

    void SMB2LzExporter::writeGolfHole(QDataStream &dev, const Scene::GolfHoleSceneNode *node)
    {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
    }

    void SMB2LzExporter::writeConeCollisionObject(QDataStream &dev, const Scene::ConeCollisionObjectSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << node->getRadius();
        dev << node->getHeight();
        dev << node->getRadius();
    }

    void SMB2LzExporter::writeSphereCollisionObject(QDataStream &dev, const Scene::SphereCollisionObjectSceneNode *node) {
        dev << node->getPosition();
        dev << node->getRadius();
        writeNull(dev, 4);
    }

    void SMB2LzExporter::writeCylinderCollisionObject(QDataStream &dev, const Scene::CylinderCollisionObjectSceneNode *node) {
        dev << node->getPosition();
        dev << node->getRadius();
        dev << node->getHeight();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
    }

    void SMB2LzExporter::writeCollisionTriangles(QDataStream &dev, const Scene::SceneNode *node) {
        if (dynamic_cast<const Scene::MeshCollisionSceneNode*>(node)) {
            const Scene::MeshCollisionSceneNode *coli= static_cast<const Scene::MeshCollisionSceneNode*>(node);
            //This node is a MeshCollisionSceneNode - Loop over all the triangles and write them
            //First, find the MeshSceneNode in the models QHash
            if (models.contains(coli->getMeshName())) {
                const Resource::ResourceMesh *mesh = models.value(coli->getMeshName());
                //Now loop over all MeshSegements, and write thier collision triangles

                foreach (const Model::MeshSegment *seg, mesh->getMeshSegments()) {
                    //Now loop over all triangles
                    for (int i = 0; i < seg->getIndices().size(); i += 3) {
                        glm::tvec3<Model::Vertex> verts(
                                seg->getVertices().at(seg->getIndices().at(i)),
                                seg->getVertices().at(seg->getIndices().at(i + 1)),
                                seg->getVertices().at(seg->getIndices().at(i + 2))
                                );

                        //////////////// BEGINNING OF MADNESS ////////////////
                        //This code is mostly just copied from Yoshimaster96's smb(2)cnv
                        //with some tweaks to make it less of a nightmare to read
                        //I'm still not 100% sure what everything does though
                        //
                        //For clarification as to how a collision triangle is stored, the first coordinate is (X1, Y1, Z1),
                        //the second coordinate is calculated as (X1 + DX2X1, Y1 + DY2Y1, Z1) then rotated about first
                        //point by Z, then Y, then X. The third coordinate is calculated similarly, from
                        //(X1 + DX3X1, Y1 + DY3Y1, Z1), rotated by the same angles
                        //
                        //Update:
                        //Where the madness began: http://kuribo64.net/board/thread.php?pid=55329
                        //Blank: cx, sx, cy, sy, cz and sz are floats and are the cosine and sine of the rotation angles.
                        //Yoshimaster96: Those aren't given however. I'm trying to find the rotation angles.
                        //Blank: You calculate the angles using cx, sx, etc. In my code snippet this is done with the reverse_angle function.
                        //
                        //For more info on how collision triangles work within Super Monkey Ball, see
                        //https://craftedcart.github.io/SMBLevelWorkshop/documentation/index.html?page=lzFormat2#spec-lzFormat2-section-collisionTriangle

                        glm::vec3 a = verts.x.position;
                        glm::vec3 b = verts.y.position;
                        glm::vec3 c = verts.z.position;

                        glm::vec3 normal = glm::normalize(cross(glm::normalize(b - a), glm::normalize(c - a)));

                        float l = qSqrt(normal.x * normal.x + normal.z * normal.z);

                        float cy;
                        float sy;
                        if (qFabs(l) < 0.001f) {
                            cy = 1.0f;
                            sy = 0.0f;
                        } else {
                            cy = normal.z / l;
                            sy = -normal.x / l;
                        }

                        float cx = l;
                        float sx = normal.y;

                        glm::mat3 rotXMat(
                                1.0f, 0.0f, 0.0f,
                                0.0f, cx, sx,
                                0.0f, -sx, cx
                                );

                        glm::mat3 rotYMat(
                                cy, 0.0f, -sy,
                                0.0f, 1.0f, 0.0f,
                                sy, 0.0f, cy
                                );

                        glm::vec3 dotry = dotm(b - a, rotYMat);
                        glm::vec3 dotrxry = dotm(dotry, rotXMat);
                        l = sqrtf(dotrxry.x * dotrxry.x + dotrxry.y * dotrxry.y);

                        float cz = dotrxry.x / l;
                        float sz = -dotrxry.y / l;

                        glm::mat3 rotZMat(
                                cz, sz, 0.0f,
                                -sz, cz, 0.0f,
                                0.0f, 0.0f, 1.0f
                                );

                        //Delta position for vertex B from vertex A (Before rotation is applied)
                        glm::vec3 deltaPosB = dotm(dotrxry, rotZMat);

                        dotry = dotm(c - a, rotYMat);
                        dotrxry = dotm(dotry, rotXMat);
                        //Delta position for vertex C from vertex A (Before rotation is applied)
                        glm::vec3 deltaPosC = dotm(dotrxry, rotZMat);

                        glm::vec3 n0v(deltaPosC.x - deltaPosB.x, deltaPosC.y - deltaPosB.y, deltaPosC.z - deltaPosB.z);
                        glm::vec3 n1v(-deltaPosC.x, -deltaPosC.y, -deltaPosC.z);
                        glm::vec3 tangent = glm::normalize(hat(n0v));
                        glm::vec3 bitangent = glm::normalize(hat(n1v));

                        float rotX = (2 * glm::pi<float>()) - reverseAngle(cx, sx);
                        float rotY = (2 * glm::pi<float>()) - reverseAngle(cy, sy);
                        float rotZ = (2 * glm::pi<float>()) - reverseAngle(cz, sz);

                        dev << a.x; //X1 pos
                        dev << a.y; //Y1 pos
                        dev << a.z; //Z1 pos
                        dev << normal.x; //X normal
                        dev << normal.y; //Y normal
                        dev << normal.z; //Z normal
                        dev << convertRotation(glm::vec3(rotX, rotY, rotZ)); //XYZ rotation from the XZ plane
                        dev << coli->getCollisionTriangleFlag();
                        dev << deltaPosB.x; //DX2X1
                        dev << deltaPosB.y; //DY2X1
                        dev << deltaPosC.x; //DX3X1
                        dev << deltaPosC.y; //DY3X1
                        dev << tangent.x; //X tangent
                        dev << tangent.y; //Y tangent
                        dev << bitangent.x; //X bitangent
                        dev << bitangent.y; //Y bitangent
                        //////////////// END OF MADNESS ////////////////
                    }
                }
            } /* else {
                //models QHash doesn't have the mesh in question - don't add it
                //Also don't warn because that happens when calculating offsets
                //qWarning().noquote() << "Missing mesh for collision" << coli->getMeshName();
            } */
        }

        //Loop over all children, looking for MeshCollisionSceneNodes
        foreach(const Scene::SceneNode *child, node->getChildren()) {
            writeCollisionTriangles(dev, child);
        }
    }

    void SMB2LzExporter::writeLevelModelPointerAList(QDataStream &dev, const Scene::GroupSceneNode *node) {
        quint32 nextOffset = levelModelOffsetMap.key(node);

        forEachChildType(node, Scene::MeshSceneNode*, child) {
            if (!child->isRuntimeReflective()) {
                dev << (quint32) (child->getBitflag());
                dev << (quint32) 0x00000001;
                dev << nextOffset;

                //Level models for the same collision header are just sequential stores, so it's fine to just add
                //on the length of a single level model
                nextOffset += LEVEL_MODEL_LENGTH;
            }
        }
    }

    void SMB2LzExporter::writeLevelModelPointerBList(QDataStream &dev, const Scene::GroupSceneNode *node) {
        quint32 nextOffset = levelModelPointerAOffsetMap.key(node);

        forEachChildType(node, Scene::MeshSceneNode*, child) {
            if (!child->isRuntimeReflective()) {
                dev << nextOffset;

                //Level model pointer type As for the same collision header are just sequential stores, so it's fine to
                //just add on the length of a single level model
                nextOffset += LEVEL_MODEL_POINTER_TYPE_A_LENGTH;
            }
        }
    }

    void SMB2LzExporter::writeLevelModelList(QDataStream &dev, const Scene::GroupSceneNode *node) {
        forEachChildType(node, Scene::MeshSceneNode*, child) {
            if (!child->isRuntimeReflective()) {
                writeNull(dev, 4);
                dev << levelModelNameOffsetMap.key(child->getMeshName());
                writeNull(dev, 8);
            }
        }
    }

    void SMB2LzExporter::writeLevelModelNameList(QDataStream &dev, const Scene::GroupSceneNode *node) {
        forEachChildType(node, Scene::MeshSceneNode*, child) {
                //Write the object name
                dev.writeRawData(child->getMeshName().toLatin1(), child->getMeshName().size());

                writeNull(dev, 1); //Add a null terminator
                //Pad to 4 bytes
                writeNull(dev, roundUpNearest4(child->getMeshName().size() + 1) - (child->getMeshName().size() + 1));
        }
    }

    void SMB2LzExporter::writeBackgroundModel(QDataStream &dev, const Scene::MeshSceneNode *node) {
        const Animation::TransformAnimation *anim = node->getTransformAnimation();
        dev << (quint32) node->getMeshType();
        dev << bgNameOffsetMap.key(node->getMeshName());
        writeNull(dev, 4);
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << node->getScale();
        writeNull(dev, 4); //TODO: Figure out use of header #1
        dev << (anim != nullptr ? bgAnimHeaderOffsetMap.key(node) : (quint32) 0); //Offset to animation header
        dev << effectHeaderOffsetMap.key(node);
    }

    void SMB2LzExporter::writeForegroundModel(QDataStream &dev, const Scene::MeshSceneNode *node) {
        const Animation::TransformAnimation *anim = node->getTransformAnimation();
        dev << (quint32) node->getMeshType();
        dev << fgNameOffsetMap.key(node->getMeshName());
        writeNull(dev, 4);
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << node->getScale();
        writeNull(dev, 4); //TODO: Figure out use of header #1
        dev << (anim != nullptr ? fgAnimHeaderOffsetMap.key(node) : (quint32) 0); //Offset to animation header
        dev << effectHeaderOffsetMap.key(node);
    }

    void SMB2LzExporter::writeBackgroundName(QDataStream &dev, const Scene::MeshSceneNode *node) {
        //Write the object name
        dev.writeRawData(node->getMeshName().toLatin1(), node->getMeshName().size());

        writeNull(dev, 1); //Add a null terminator
        //Pad to 4 bytes
        writeNull(dev, roundUpNearest4(node->getMeshName().size() + 1) - (node->getMeshName().size() + 1));
    }

    void SMB2LzExporter::writeForegroundName(QDataStream &dev, const Scene::MeshSceneNode *node) {
        //Write the object name
        dev.writeRawData(node->getMeshName().toLatin1(), node->getMeshName().size());

        writeNull(dev, 1); //Add a null terminator
        //Pad to 4 bytes
        writeNull(dev, roundUpNearest4(node->getMeshName().size() + 1) - (node->getMeshName().size() + 1));
    }

    void SMB2LzExporter::writeAnimationHeader(QDataStream &dev, const Animation::TransformAnimation *anim) {
        //Not all groups will have animation - get outta here if nullptr
        if (anim == nullptr) return;

        dev << (quint32) anim->getRotXKeyframes().size(); //Number of rot X keyframes
        dev << (quint32) animRotXKeyframesOffsetMap.key(anim); //Offset to pos X keyframes
        dev << (quint32) anim->getRotYKeyframes().size(); //Number of rot Y keyframes
        dev << (quint32) animRotYKeyframesOffsetMap.key(anim); //Offset to rot Y keyframes
        dev << (quint32) anim->getRotZKeyframes().size(); //Number of rot Z keyframes
        dev << (quint32) animRotZKeyframesOffsetMap.key(anim); //Offset to rot Z keyframes
        dev << (quint32) anim->getPosXKeyframes().size(); //Number of pos X keyframes
        dev << (quint32) animPosXKeyframesOffsetMap.key(anim); //Offset to pos X keyframes
        dev << (quint32) anim->getPosYKeyframes().size(); //Number of pos Y keyframes
        dev << (quint32) animPosYKeyframesOffsetMap.key(anim); //Offset to pos Y keyframes
        dev << (quint32) anim->getPosZKeyframes().size(); //Number of pos Z keyframes
        dev << (quint32) animPosZKeyframesOffsetMap.key(anim); //Offset to pos Z keyframes
        writeNull(dev, 16);
    }

    void SMB2LzExporter::writeBgFgAnimationHeader(QDataStream &dev, const Animation::TransformAnimation *anim) {
        //Not all groups will have animation - get outta here if nullptr
        if (anim == nullptr) return;
        writeNull(dev, 4);
        dev << anim->getLoopTime();
        dev << (quint32) anim->getScaleXKeyframes().size(); //Number of scale X keyframes
        dev << (quint32) (anim->getScaleXKeyframes().size() != 0 ? animScaleXKeyframesOffsetMap.key(anim) : 0); //Offset to scale X keyframes
        dev << (quint32) anim->getScaleYKeyframes().size(); //Number of scale Y keyframes
        dev << (quint32) (anim->getScaleYKeyframes().size() != 0 ? animScaleYKeyframesOffsetMap.key(anim) : 0); //Offset to scale Y keyframes
        dev << (quint32) anim->getScaleZKeyframes().size(); //Number of scale Z keyframes
        dev << (quint32) (anim->getScaleZKeyframes().size() != 0 ? animScaleZKeyframesOffsetMap.key(anim) : 0); //Offset to scale Z keyframes
        dev << (quint32) anim->getRotXKeyframes().size(); //Number of rot X keyframes
        dev << (quint32) (anim->getRotXKeyframes().size() != 0 ? animRotXKeyframesOffsetMap.key(anim) : 0); //Offset to rotation X keyframes
        dev << (quint32) anim->getRotYKeyframes().size(); //Number of rot Y keyframes
        dev << (quint32) (anim->getRotYKeyframes().size() != 0 ? animRotYKeyframesOffsetMap.key(anim) : 0); //Offset to rotation Y keyframes
        dev << (quint32) anim->getRotZKeyframes().size(); //Number of rot Z keyframes
        dev << (quint32) (anim->getRotZKeyframes().size() != 0 ? animRotZKeyframesOffsetMap.key(anim) : 0); //Offset to rotation Z keyframes
        dev << (quint32) anim->getPosXKeyframes().size(); //Number of pos X keyframes
        dev << (quint32) (anim->getPosXKeyframes().size() != 0 ? animPosXKeyframesOffsetMap.key(anim) : 0); //Offset to pos X keyframes
        dev << (quint32) anim->getPosYKeyframes().size(); //Number of pos Y keyframes
        dev << (quint32) (anim->getPosYKeyframes().size() != 0 ? animPosYKeyframesOffsetMap.key(anim) : 0); //Offset to pos Y keyframes
        dev << (quint32) anim->getPosZKeyframes().size(); //Number of pos Z keyframes
        dev << (quint32) (anim->getPosZKeyframes().size() != 0 ? animPosZKeyframesOffsetMap.key(anim) : 0); //Offset to pos Z keyframes
        writeNull(dev, 16);
    }

    void SMB2LzExporter::writeEffectHeader(QDataStream &dev, const Scene::MeshSceneNode *node) {
        dev << effectAnimType1KeyframesOffsetMap.size();
        dev << (quint32) (effectAnimType1KeyframesOffsetMap.size() > 0 ? effectAnimType1KeyframesOffsetMap.firstKey() : 0);
        dev << effectAnimType2KeyframesOffsetMap.size();
        dev << (quint32) (effectAnimType2KeyframesOffsetMap.size() > 0 ? effectAnimType2KeyframesOffsetMap.firstKey() : 0);
        dev << textureScrollOffsetMap.key(node);
        writeNull(dev, 28); //TODO: Whatever this stuff is
    }

    void SMB2LzExporter::writeTextureScroll(QDataStream &dev, const Scene::SceneNode *node) {
        dev << node->getTextureScroll();
    }

    void SMB2LzExporter::writeTransformAnimation(QDataStream &dev, const Animation::TransformAnimation *anim, bool scale=false) {
        //Not all groups will have animation - get outta here if nullptr
        if (anim == nullptr) return;
        if (scale) {
            foreach(Animation::KeyframeF *k, anim->getScaleXKeyframes()) writeKeyframeF(dev, k);
            foreach(Animation::KeyframeF *k, anim->getScaleYKeyframes()) writeKeyframeF(dev, k);
            foreach(Animation::KeyframeF *k, anim->getScaleZKeyframes()) writeKeyframeF(dev, k);
        }
        foreach(Animation::KeyframeF *k, anim->getRotXKeyframes()) writeKeyframeAngleF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getRotYKeyframes()) writeKeyframeAngleF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getRotZKeyframes()) writeKeyframeAngleF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getPosXKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getPosYKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getPosZKeyframes()) writeKeyframeF(dev, k);
    }

    void SMB2LzExporter::writeFogAnimation(QDataStream &dev, const Animation::FogAnimation *anim)
    {
        if (anim == nullptr) return;

        foreach(Animation::KeyframeF *k, anim->getStartDistanceKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getEndDistanceKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getRedKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getGreenKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getBlueKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, anim->getUnknownKeyframes()) writeKeyframeF(dev, k);
    }

    void SMB2LzExporter::writeRaceTrackPath(QDataStream &dev, const Scene::RaceTrackPathSceneNode *node)
    {
        foreach(Animation::KeyframeF *k, node->getTrackPath()->getPosXKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, node->getTrackPath()->getPosYKeyframes()) writeKeyframeF(dev, k);
        foreach(Animation::KeyframeF *k, node->getTrackPath()->getPosZKeyframes()) writeKeyframeF(dev, k);
    }

    void SMB2LzExporter::writeCPUTrackPathHeaders(QDataStream &dev, const Stage &stage)
    {
        // Again, assuming there are always 7 CPU players

        QMultiMap<quint32, Animation::RaceTrackPath*> trackPaths;
        foreach(Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (dynamic_cast<Scene::RaceTrackPathSceneNode*>(node)) {
                Scene::RaceTrackPathSceneNode *path = static_cast<Scene::RaceTrackPathSceneNode*>(node);
                trackPaths.insert(path->getTrackPath()->getPlayerID(), path->getTrackPath());
            }
        }

        if (trackPaths.size() < 8) {
            qCritical() << "Less than the required 8 track paths found";
        }

        for (quint32 cpuId = 1; cpuId < 8; cpuId++) {
            dev << (quint32) trackPaths.value(cpuId)->getPosXKeyframes().size();
            dev << (quint32) raceTrackPathPosXKeyframesOffsetMap.key(trackPaths.value(cpuId));
            dev << (quint32) trackPaths.value(cpuId)->getPosYKeyframes().size();
            dev << (quint32) raceTrackPathPosYKeyframesOffsetMap.key(trackPaths.value(cpuId));
            dev << (quint32) trackPaths.value(cpuId)->getPosZKeyframes().size();
            dev << (quint32) raceTrackPathPosZKeyframesOffsetMap.key(trackPaths.value(cpuId));
       }
    }

    void SMB2LzExporter::writeRuntimeReflectiveModelList(QDataStream &dev, const Scene::GroupSceneNode *node) {
        forEachChildType(node, Scene::MeshSceneNode*, child) {
            if (child->isRuntimeReflective()) {
                dev << levelModelNameOffsetMap.key(child->getMeshName());
                writeNull(dev, 8);
            }
        }
    }

    void SMB2LzExporter::writeKeyframeF(QDataStream &dev, const Animation::KeyframeF *k) {
        dev << (quint32) k->getEasing(); //Easing
        dev << k->getValue().first;
        dev << k->getValue().second;
        dev << k->getHandleAValue();
        dev << k->getHandleBValue();
    }

    void SMB2LzExporter::writeKeyframeEffect1(QDataStream &dev, const Animation::KeyframeEffect1 *k)
    {
        dev << k->getPosition();
        dev << k->getRotation();
        dev << (quint8) k->getUnknownByte1();
        dev << (quint8) k->getUnknownByte2();
    }

    void SMB2LzExporter::writeKeyframeEffect2(QDataStream &dev, const Animation::KeyframeEffect2 *k)
    {
       dev << k->getPosition();
       dev << (quint8) k->getUnknownByte1();
       dev << (quint8) k->getUnknownByte2();
       dev << (quint8) k->getUnknownByte3();
       dev << (quint8) k->getUnknownByte4();
    }

    void SMB2LzExporter::writeEffectAnimation(QDataStream &dev, const Animation::EffectAnimation *anim)
    {
        if (anim != nullptr) {
            foreach(Animation::KeyframeEffect1 *k, anim->getEffect1Keyframes()) writeKeyframeEffect1(dev, k);
            foreach(Animation::KeyframeEffect2 *k, anim->getEffect2Keyframes()) writeKeyframeEffect2(dev, k);
        }
    }

    void SMB2LzExporter::writeKeyframeAngleF(QDataStream &dev, const Animation::KeyframeF *k) {
        dev << (quint32) k->getEasing(); //Easing
        dev << k->getValue().first;
        dev << qRadiansToDegrees(k->getValue().second);
        dev << k->getHandleAValue();
        dev << k->getHandleBValue();
    }

    void SMB2LzExporter::writeNull(QDataStream &dev, const unsigned int count) {
        for (unsigned int i = 0; i < count; i++) {
            dev.writeRawData("\0", 1);
        }
    }

    glm::tvec3<quint16> SMB2LzExporter::convertRotation(glm::vec3 rot) {
        rot.x = fmod(rot.x, 2 * glm::pi<float>());
        rot.y = fmod(rot.y, 2 * glm::pi<float>());
        rot.z = fmod(rot.z, 2 * glm::pi<float>());

        if (rot.x < 0) rot.x += 2 * glm::pi<float>();
        if (rot.y < 0) rot.y += 2 * glm::pi<float>();
        if (rot.z < 0) rot.z += 2 * glm::pi<float>();

        return glm::tvec3<quint16>(
                rot.x / (2 * glm::pi<float>()) * 65536.0f,
                rot.y / (2 * glm::pi<float>()) * 65536.0f,
                rot.z / (2 * glm::pi<float>()) * 65536.0f
                );
    }

    quint32 SMB2LzExporter::addAllCounts(QMap<const Scene::GroupSceneNode*, quint32> &m) {
        quint32 total = 0;

        QMapIterator<const Scene::GroupSceneNode*, quint32> i(m);
        while (i.hasNext()) {
            i.next();
            total += i.value();
        }

        return total;
    }

    quint32 SMB2LzExporter::roundUpNearest4(quint32 n) {
        if (n % 4 == 0) return n;
        return (n + 3) / 4 * 4;
    }

    //The rest of this file is madness required for the collision triangle writing guff
    glm::vec3 SMB2LzExporter::dotm(glm::vec3 a, glm::mat3 m) {
        float d0 = (a.x * m[0][0]) + (a.y * m[1][0]) + (a.z * m[2][0]);
        float d1 = (a.x * m[0][1]) + (a.y * m[1][1]) + (a.z * m[2][1]);
        float d2 = (a.x * m[0][2]) + (a.y * m[1][2]) + (a.z * m[2][2]);
        return glm::vec3(d0, d1, d2);
    }

    glm::vec3 SMB2LzExporter::cross(glm::vec3 a, glm::vec3 b) {
        float d0 = (a.y * b.z) - (a.z * b.y);
        float d1 = (a.z * b.x) - (a.x * b.z);
        float d2 = (a.x * b.y) - (a.y * b.x);
        return glm::vec3(d0, d1, d2);
    }

    glm::vec3 SMB2LzExporter::hat(glm::vec3 v) {
        return glm::vec3(-v.y, v.x, 0.0f);
    }

    float SMB2LzExporter::reverseAngle(float c, float s) {
        float a = asin(s);
        if (c < 0.0f) a = glm::pi<float>() - a;
        if (qFabs(c) < qFabs(s)) {
            a = acos(c);
            if(s < 0.0f) a = -a;
        }
        if (a < 0.0f) {
            if(a > -0.001f) a = 0.0f;
            else a += 2 * glm::pi<float>();
        }
        return a;
    }
}

