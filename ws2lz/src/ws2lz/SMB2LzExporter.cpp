#include "ws2lz/SMB2LzExporter.hpp"
#include "ws2lz/WS2Lz.hpp"
#include "ws2common/WS2Common.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/MeshCollisionSceneNode.hpp"
#include <QElapsedTimer>
#include <QDebug>
#include <math.h>

namespace WS2Lz {
    SMB2LzExporter::~SMB2LzExporter() {
        qDeleteAll(triangleIntGridMap.values());
    }

    void SMB2LzExporter::setModels(QHash<QString, WS2Common::Resource::ResourceMesh*> &models) {
        this->models = models;
    }

    void SMB2LzExporter::generate(QDataStream &dev, const WS2Common::Stage &stage) {
        dev.setByteOrder(QDataStream::BigEndian);
        dev.setFloatingPointPrecision(QDataStream::SinglePrecision);

        optimizeCollision(stage);
        calculateOffsets(stage);
        writeFileHeader(dev);
        writeStart(dev, stage);
        writeFallout(dev, stage);

        //Collision headers
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> collisionHeaderIter(collisionHeaderOffsetMap);
        while (collisionHeaderIter.hasNext()) {
            collisionHeaderIter.next();
            writeCollisionHeader(dev, collisionHeaderIter.value());
        }

        //Collision triangles
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> collisionTriangleIter(collisionHeaderOffsetMap);
        while (collisionTriangleIter.hasNext()) {
            collisionTriangleIter.next();
            writeCollisionTriangles(dev, collisionTriangleIter.value());
        }

        //Collision triangle pointers
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> collisionTrianglePointerIter(collisionHeaderOffsetMap);
        while (collisionTrianglePointerIter.hasNext()) {
            collisionTrianglePointerIter.next();
            //Writes the collision triangle list pointers
            writeCollisionTriangleIndexListPointers(dev, collisionTrianglePointerIter.value());
        }

        //Collision triangle index list
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> collisionTriangleIndexIter(collisionHeaderOffsetMap);
        while (collisionTriangleIndexIter.hasNext()) {
            collisionTriangleIndexIter.next();
            writeCollisionTriangleIndexList(dev, triangleIntGridMap.value(collisionTriangleIndexIter.value()));
        }

        //Goals
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> goalIter(collisionHeaderOffsetMap);
        while (goalIter.hasNext()) {
            goalIter.next();
            foreach(WS2Common::Scene::SceneNode *node, goalIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::GoalSceneNode>(node)) {
                    writeGoal(dev, static_cast<WS2Common::Scene::GoalSceneNode*>(node));
                }
            }
        }

        //Bumpers
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> bumperIter(collisionHeaderOffsetMap);
        while (bumperIter.hasNext()) {
            bumperIter.next();
            foreach(WS2Common::Scene::SceneNode *node, bumperIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::BumperSceneNode>(node)) {
                    writeBumper(dev, static_cast<WS2Common::Scene::BumperSceneNode*>(node));
                }
            }
        }

        //Jamabars
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> jamabarIter(collisionHeaderOffsetMap);
        while (jamabarIter.hasNext()) {
            jamabarIter.next();
            foreach(WS2Common::Scene::SceneNode *node, jamabarIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::JamabarSceneNode>(node)) {
                    writeJamabar(dev, static_cast<WS2Common::Scene::JamabarSceneNode*>(node));
                }
            }
        }

        //Bananas
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> bananaIter(collisionHeaderOffsetMap);
        while (bananaIter.hasNext()) {
            bananaIter.next();
            foreach(WS2Common::Scene::SceneNode *node, bananaIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::BananaSceneNode>(node)) {
                    writeBanana(dev, static_cast<WS2Common::Scene::BananaSceneNode*>(node));
                }
            }
        }

        //Level model pointer type A
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerAIter(collisionHeaderOffsetMap);
        while (levelModelPointerAIter.hasNext()) {
            levelModelPointerAIter.next();
            writeLevelModelPointerAList(dev, levelModelPointerAIter.value());
        }

        //Level model pointer type B
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerBIter(collisionHeaderOffsetMap);
        while (levelModelPointerBIter.hasNext()) {
            levelModelPointerBIter.next();
            writeLevelModelPointerBList(dev, levelModelPointerBIter.value());
        }

        //Level models
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelIter(collisionHeaderOffsetMap);
        while (levelModelIter.hasNext()) {
            levelModelIter.next();
            writeLevelModelList(dev, levelModelIter.value());
        }

        //Level model names
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelNameIter(collisionHeaderOffsetMap);
        while (levelModelNameIter.hasNext()) {
            levelModelNameIter.next();
            writeLevelModelNameList(dev, levelModelNameIter.value());
        }
    }

    void SMB2LzExporter::addCollisionTriangles(
            const WS2Common::Scene::SceneNode *node,
            QVector<WS2Common::Model::Vertex> &allVertices,
            QVector<unsigned int> &allIndices
            ) {
        if (WS2Common::instanceOf<WS2Common::Scene::MeshCollisionSceneNode>(node)) {
            const WS2Common::Scene::MeshCollisionSceneNode *coli= static_cast<const WS2Common::Scene::MeshCollisionSceneNode*>(node);
            //First, find the MeshSceneNode in the models QHash
            if (models.contains(coli->getMeshName())) {
                const WS2Common::Resource::ResourceMesh *mesh = models.value(coli->getMeshName());
                //Now loop over all MeshSegements, and add to nextOffset
                foreach (const WS2Common::Model::MeshSegment *seg, mesh->getMeshSegments()) {
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
        foreach(const WS2Common::Scene::SceneNode *child, node->getChildren()) {
            addCollisionTriangles(child, allVertices, allIndices);
        }
    }

    void SMB2LzExporter::optimizeCollision(const WS2Common::Stage &stage) {
        //First check what triangles intersect which grid times, in order to optimize collision
        qDebug() << "Now optimizing collision... This may take a little while";
        QElapsedTimer timer; //Measure how long this operation takes - probably a little while
        timer.start();

        //Loop over all collision headers
        foreach(WS2Common::Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (WS2Common::instanceOf<WS2Common::Scene::GroupSceneNode>(node)) {
                WS2Common::Scene::GroupSceneNode *groupNode = static_cast<WS2Common::Scene::GroupSceneNode*>(node);
                //Find all MeshCollisionSceneNodes, and add the triangles to allVertices/allIndices
                QVector<WS2Common::Model::Vertex> allVertices;
                QVector<unsigned int> allIndices;
                addCollisionTriangles(node, allVertices, allIndices);

                //Now create the TriangleIntrsectionGrid, which will check each triangle for intersections with each grid tile
                TriangleIntersectionGrid *intGrid = new TriangleIntersectionGrid(
                        allVertices,
                        allIndices,
                        *groupNode->getCollisionGrid()
                        );

                //Store it
                triangleIntGridMap[groupNode] = intGrid;
            }
        }

        //Finished - log the amount of time it took
        qDebug().noquote().nospace() << "Finished optimizing collision in " << timer.nsecsElapsed() / 1000000000.0f << "s";
    }

    void SMB2LzExporter::calculateOffsets(const WS2Common::Stage &stage) {
        quint32 nextOffset = FILE_HEADER_LENGTH;

        startOffset = nextOffset;
        nextOffset += START_LENGTH;

        falloutOffset = nextOffset;
        nextOffset += FALLOUT_LENGTH;

        //Find all GroupSceneNodes/Collision headers
        foreach(WS2Common::Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (WS2Common::instanceOf<WS2Common::Scene::GroupSceneNode>(node)) {
                //Found one
                collisionHeaderOffsetMap[nextOffset] = static_cast<WS2Common::Scene::GroupSceneNode*>(node);
                nextOffset += COLLISION_HEADER_LENGTH;
            }
        }

        //Iterate over all GroupSceneNodes/collision headers, and call addCollisionTriangleOffsets with them
        //This is for Collision Triangle data
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> collisionTriangleIter(collisionHeaderOffsetMap);
        while (collisionTriangleIter.hasNext()) {
            collisionTriangleIter.next();
            gridTriangleListOffsetMap[nextOffset] = collisionTriangleIter.value();
            addCollisionTriangleOffsets(collisionTriangleIter.value(), nextOffset);
        }

        //Iterate over all GroupSceneNodes/collision headers, and fill the gridTriangleListPointersOffsetMap
        //This is for Collision Triangle Pointers
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> collisionTrianglePointerIter(collisionHeaderOffsetMap);
        while (collisionTrianglePointerIter.hasNext()) {
            collisionTrianglePointerIter.next();
            gridTriangleListPointersOffsetMap[nextOffset] = collisionTrianglePointerIter.value();
            const WS2Common::CollisionGrid *grid = collisionTriangleIter.value()->getCollisionGrid();
            nextOffset += COLLISION_TRIANGLE_LIST_POINTER_LENGTH * grid->getGridStepCount().x * grid->getGridStepCount().y;
        }

        //Iterate over all GroupSceneNodes/collision headers, and fill the gridTriangleListOffsetMap
        //This is for the Collision Triangle Index List
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> collisionTriangleIndexIter(collisionHeaderOffsetMap);
        while (collisionTriangleIndexIter.hasNext()) {
            collisionTriangleIndexIter.next();
            gridTriangleListOffsetMap[nextOffset] = collisionTriangleIndexIter.value();

            //2D vector (X, Y) containing vectors of triangle indices
            QVector<QVector<QVector<quint16>>> indicesGrid = triangleIntGridMap.value(collisionTriangleIndexIter.value())->getIndicesGrid();

            //Loop over all vectors
            for (int x = 0; x < indicesGrid.size(); x++) {
                for (int y = 0; y < indicesGrid[x].size(); y++) {
                    if (indicesGrid[x][y].size() == 0) {
                        //If this grid tile has zero triangles to collide with, just add a null offset to save file size
                        gridTriangleIndexListOffsetMap[collisionTriangleIndexIter.value()].append(0x00000000);
                    } else {
                        gridTriangleIndexListOffsetMap[collisionTriangleIndexIter.value()].append(nextOffset);

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
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> goalIter(collisionHeaderOffsetMap);
        while (goalIter.hasNext()) {
            goalIter.next();
            goalOffsetMap[nextOffset] = goalIter.value();
            quint32 goalCount = 0; //Number of goals in this collision header

            foreach(WS2Common::Scene::SceneNode *node, goalIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::GoalSceneNode>(node)) {
                    nextOffset += GOAL_LENGTH;
                    goalCount++;
                }
            }

            //Store goal count in the map
            goalCountMap[goalIter.value()] = goalCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count bumpers to add to nextOffset
        //Basically the exact same as before with goals
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> bumperIter(collisionHeaderOffsetMap);
        while (bumperIter.hasNext()) {
            bumperIter.next();
            bumperOffsetMap[nextOffset] = bumperIter.value();
            quint32 bumperCount = 0; //Number of bumpers in this collision header

            foreach(WS2Common::Scene::SceneNode *node, bumperIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::BumperSceneNode>(node)) {
                    nextOffset += BUMPER_LENGTH;
                    bumperCount++;
                }
            }

            //Store bumper count in the map
            bumperCountMap[bumperIter.value()] = bumperCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count jamabars to add to nextOffset
        //Basically the exact same as before with goals
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> jamabarIter(collisionHeaderOffsetMap);
        while (jamabarIter.hasNext()) {
            jamabarIter.next();
            jamabarOffsetMap[nextOffset] = jamabarIter.value();
            quint32 jamabarCount = 0; //Number of jamabars in this collision header

            foreach(WS2Common::Scene::SceneNode *node, jamabarIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::JamabarSceneNode>(node)) {
                    nextOffset += JAMABAR_LENGTH;
                    jamabarCount++;
                }
            }

            //Store jamabar count in the map
            jamabarCountMap[jamabarIter.value()] = jamabarCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count bananas to add to nextOffset
        //Basically the exact same as before with goals
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> bananaIter(collisionHeaderOffsetMap);
        while (bananaIter.hasNext()) {
            bananaIter.next();
            bananaOffsetMap[nextOffset] = bananaIter.value();
            quint32 bananaCount = 0; //Number of bananas in this collision header

            foreach(WS2Common::Scene::SceneNode *node, bananaIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::BananaSceneNode>(node)) {
                    nextOffset += BANANA_LENGTH;
                    bananaCount++;
                }
            }

            //Store banana count in the map
            bananaCountMap[bananaIter.value()] = bananaCount;
        }

        //Iterate over all GroupSceneNodes/collision headers, and count level models to add to nextOffset
        //For level model pointer type A
        //Basically the exact same as before with goals
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerAIter(collisionHeaderOffsetMap);
        while (levelModelPointerAIter.hasNext()) {
            levelModelPointerAIter.next();
            levelModelPointerAOffsetMap[nextOffset] = levelModelPointerAIter.value();

            foreach(WS2Common::Scene::SceneNode *node, levelModelPointerAIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                    nextOffset += LEVEL_MODEL_POINTER_TYPE_A_LENGTH;
                }
            }
        }

        //Iterate over all GroupSceneNodes/collision headers, and count level models to add to nextOffset
        //For level model pointer type B
        //Basically the exact same as before with goals
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelPointerBIter(collisionHeaderOffsetMap);
        while (levelModelPointerBIter.hasNext()) {
            levelModelPointerBIter.next();
            levelModelPointerBOffsetMap[nextOffset] = levelModelPointerBIter.value();

            foreach(WS2Common::Scene::SceneNode *node, levelModelPointerBIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                    nextOffset += LEVEL_MODEL_POINTER_TYPE_B_LENGTH;
                }
            }
        }

        //Iterate over all GroupSceneNodes/collision headers, and count level models to add to nextOffset
        //Basically the exact same as before with goals
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelIter(collisionHeaderOffsetMap);
        while (levelModelIter.hasNext()) {
            levelModelIter.next();
            levelModelOffsetMap[nextOffset] = levelModelIter.value();
            quint32 levelModelCount = 0; //Number of levelModels in this collision header

            foreach(WS2Common::Scene::SceneNode *node, levelModelIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                    nextOffset += LEVEL_MODEL_LENGTH;
                    levelModelCount++;
                }
            }

            //Store levelModel count in the map
            levelModelCountMap[levelModelIter.value()] = levelModelCount;
        }

        //Iterate over all level models, and add the model name + null terminator padded to 4 bytes to nextOffset
        QMapIterator<quint32, const WS2Common::Scene::GroupSceneNode*> levelModelNameIter(collisionHeaderOffsetMap);
        while (levelModelNameIter.hasNext()) {
            levelModelNameIter.next();
            foreach(WS2Common::Scene::SceneNode *node, levelModelNameIter.value()->getChildren()) {
                if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                    //Found a level model
                    WS2Common::Scene::MeshSceneNode *mesh = static_cast<WS2Common::Scene::MeshSceneNode*>(node);
                    levelModelNameOffsetMap[nextOffset] = mesh->getMeshName();

                    //+ 1 because size() does not include a null terminator
                    nextOffset += roundUpNearest4(mesh->getMeshName().size() + 1);
                }
            }
        }

        //Just set all this guff to null for now in case it isn't
        coneCollisionObjectCount = 0;
        coneCollisionObjectListOffset = 0;
        sphereCollisionObjectCount = 0;
        sphereCollisionObjectListOffset = 0;
        cylinderCollisionObjectCount = 0;
        cylinderCollisionObjectListOffset = 0;
        falloutVolumeCount = 0;
        falloutVolumeListOffset = 0;
        backgroundModelCount = 0;
        backgroundModelListOffset = 0;
        //TODO: Mystery 8
        //TODO: Reflective level model
        //TODO: Level model instances
        switchCount = 0;
        switchListOffset = 0;
        //TODO: Fog anim header
        wormholeCount = 0;
        wormholeListOffset = 0;
    }

    void SMB2LzExporter::addCollisionTriangleOffsets(const WS2Common::Scene::SceneNode *node, quint32 &nextOffset) {
        //TODO: Store this in a map or hash or something, per collision header, add a function argument for the collision header, store in a nested maps - Collision header, object name, offset
        if (WS2Common::instanceOf<WS2Common::Scene::MeshCollisionSceneNode>(node)) {
            const WS2Common::Scene::MeshCollisionSceneNode *coli= static_cast<const WS2Common::Scene::MeshCollisionSceneNode*>(node);
            //First, find the MeshSceneNode in the models QHash
            if (models.contains(coli->getMeshName())) {
                const WS2Common::Resource::ResourceMesh *mesh = models.value(coli->getMeshName());
                //Now loop over all MeshSegements, and add to nextOffset
                foreach (const WS2Common::Model::MeshSegment *seg, mesh->getMeshSegments()) {
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
        foreach(const WS2Common::Scene::SceneNode *child, node->getChildren()) {
            addCollisionTriangleOffsets(child, nextOffset);
        }
    }

    void SMB2LzExporter::writeFileHeader(QDataStream &dev) {
        writeNull(dev, 4); dev << 0x447A0000; //Magic number (Probably)
        dev << (quint32) collisionHeaderOffsetMap.size();
        dev << (quint32) (collisionHeaderOffsetMap.size() > 0 ? collisionHeaderOffsetMap.firstKey() : 0);
        dev << startOffset;
        dev << falloutOffset;
        dev << addAllCounts(goalCountMap);
        //firstKey will crash if the list is empty - so just use 0 if size() is not > 0
        dev << (quint32) (goalOffsetMap.size() > 0 ? goalOffsetMap.firstKey() : 0); //Goal list offset
        dev << addAllCounts(bumperCountMap);
        dev << (quint32) (bumperOffsetMap.size() > 0 ? bumperOffsetMap.firstKey() : 0); //Bumper list offset
        dev << addAllCounts(bumperCountMap);
        dev << (quint32) (jamabarOffsetMap.size() > 0 ? jamabarOffsetMap.firstKey() : 0); //Jamabar list offset
        dev << addAllCounts(bananaCountMap);
        dev << (quint32) (bananaOffsetMap.size() > 0 ? bananaOffsetMap.firstKey() : 0); //Banana list offset
        dev << coneCollisionObjectCount;
        dev << coneCollisionObjectListOffset;
        dev << sphereCollisionObjectCount;
        dev << sphereCollisionObjectListOffset;
        dev << cylinderCollisionObjectCount;
        dev << cylinderCollisionObjectListOffset;
        dev << falloutVolumeCount;
        dev << falloutVolumeListOffset;
        dev << backgroundModelCount;
        dev << backgroundModelListOffset;
        writeNull(dev, 8); //TODO: Mystery 8
        writeNull(dev, 4);
        dev << (quint32) 0x00000001;
        writeNull(dev, 8); //TODO: Reflective level models
        writeNull(dev, 12);
        writeNull(dev, 8); //TODO: Level model instances
        dev << addAllCounts(levelModelCountMap);
        dev << levelModelPointerAOffsetMap.firstKey();
        dev << addAllCounts(levelModelCountMap);
        dev << levelModelPointerBOffsetMap.firstKey();
        writeNull(dev, 12);
        dev << switchCount;
        dev << switchListOffset;
        writeNull(dev, 4); //TODO: Fog animation header
        dev << wormholeCount;
        dev << wormholeListOffset;
        writeNull(dev, 4); //TODO: Fog
        writeNull(dev, 20);
        writeNull(dev, 4); //TODO: Mystery 3
        writeNull(dev, 1988);
    }

    void SMB2LzExporter::writeStart(QDataStream &dev, const WS2Common::Stage &stage) {
        WS2Common::Scene::StartSceneNode *start;

        //Find the start
        foreach(WS2Common::Scene::SceneNode *node, stage.getRootNode()->getChildren()) {
            if (WS2Common::instanceOf<WS2Common::Scene::StartSceneNode>(node)) {
                //Found it!
                start = static_cast<WS2Common::Scene::StartSceneNode*>(node);
                break;
            }
        }

        //Write the bytes
        dev << start->getPosition();
        dev << convertRotation(start->getRotation());
        writeNull(dev, 2);
    }

    void SMB2LzExporter::writeFallout(QDataStream &dev, const WS2Common::Stage &stage) {
        dev << stage.getFalloutY();
    }

    void SMB2LzExporter::writeCollisionHeader(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node) {
        writeNull(dev, 12); //TODO: Center of rotation vec3
        writeNull(dev, 6); //TODO: Initial rotation vec3
        writeNull(dev, 2); //TODO: Animation loop type/seesaw
        writeNull(dev, 4); //TODO: Offset to animation header
        writeNull(dev, 12); //TODO: Conveyor speed vec3
        dev << gridTriangleListOffsetMap.key(node);
        dev << gridTriangleListPointersOffsetMap.key(node);
        dev << node->getCollisionGrid()->getGridStart();
        dev << node->getCollisionGrid()->getGridStep();
        dev << node->getCollisionGrid()->getGridStepCount();
        dev << goalCountMap.value(node);
        dev << goalOffsetMap.key(node);
        dev << bumperCountMap.value(node);
        dev << bumperOffsetMap.key(node);
        dev << jamabarCountMap.value(node);
        dev << jamabarOffsetMap.key(node);
        dev << bananaCountMap.value(node);
        dev << bananaOffsetMap.key(node);
        writeNull(dev, 48); //TODO: Everything else
        dev << levelModelCountMap.value(node);
        dev << levelModelPointerBOffsetMap.key(node);
        writeNull(dev, 1024); //TODO: Everything else
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

    void SMB2LzExporter::writeCollisionTriangleIndexListPointers(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node) {
        unsigned int totalTiles = node->getCollisionGrid()->getGridStepCount().x * node->getCollisionGrid()->getGridStepCount().y;
        for (unsigned int i = 0; i < totalTiles; i++) {
            dev << gridTriangleIndexListOffsetMap[node][i];
        }
    }

    void SMB2LzExporter::writeGoal(QDataStream &dev, const WS2Common::Scene::GoalSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        dev << (quint16) node->getType();
    }

    void SMB2LzExporter::writeBumper(QDataStream &dev, const WS2Common::Scene::BumperSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << node->getScale();
    }

    void SMB2LzExporter::writeJamabar(QDataStream &dev, const WS2Common::Scene::JamabarSceneNode *node) {
        dev << node->getPosition();
        dev << convertRotation(node->getRotation());
        writeNull(dev, 2);
        dev << node->getScale();
    }

    void SMB2LzExporter::writeBanana(QDataStream &dev, const WS2Common::Scene::BananaSceneNode *node) {
        dev << node->getPosition();
        dev << (quint32) node->getType();
    }

    void SMB2LzExporter::writeCollisionTriangles(QDataStream &dev, const WS2Common::Scene::SceneNode *node) {
        if (WS2Common::instanceOf<WS2Common::Scene::MeshCollisionSceneNode>(node)) {
            const WS2Common::Scene::MeshCollisionSceneNode *coli= static_cast<const WS2Common::Scene::MeshCollisionSceneNode*>(node);
            //This node is a MeshCollisionSceneNode - Loop over all the triangles and write them
            //First, find the MeshSceneNode in the models QHash
            if (models.contains(coli->getMeshName())) {
                const WS2Common::Resource::ResourceMesh *mesh = models.value(coli->getMeshName());
                //Now loop over all MeshSegements, and write thier collision triangles

                foreach (const WS2Common::Model::MeshSegment *seg, mesh->getMeshSegments()) {
                    //Now loop over all triangles
                    for (int i = 0; i < seg->getIndices().size(); i += 3) {
                        //verts.x/y/z corresponds to each triangle's vertex
                        glm::tvec3<WS2Common::Model::Vertex> verts(
                                seg->getVertices().at(seg->getIndices().at(i)),
                                seg->getVertices().at(seg->getIndices().at(i + 1)),
                                seg->getVertices().at(seg->getIndices().at(i + 2))
                                );

                        //////////////// BEGINNING OF MADNESS ////////////////
                        //This code is mostly just copied from Yoshimaster96's smb(2)cnv
                        //I hope I don't have to maintain this
                        //This way lies madness
                        //Send help
                        //And no, I don't know what the heck these variable names are supposed to mean
                        //Just know that this works, and don't touch it
                        //Better yet, don't look at it - spare yourself the insanity
                        //I think there's supposed to be some matrices in here (rxr/ryr/rzr maybe?) but just shown as multiple vec3s
                        //I recall someone talking about rotational matrices being in here
                        //Although I dare not touch this madness, maybe
                        //If any brave soul dares to try to make sense of this and clean it up, thank you
                        //
                        //Update:
                        //Where the madness began: http://kuribo64.net/board/thread.php?pid=55329
                        //Blank: cx, sx, cy, sy, cz and sz are floats and are the cosine and sine of the rotation angles.
                        //Yoshimaster96: Those aren't given however. I'm trying to find the rotation angles.
                        //Blank: You calculate the angles using cx, sx, etc. In my code snippet this is done with the reverse_angle function.

                        //glm::vec3 na = {verts.x.normal.x, verts.x.normal.y, verts.x.normal.z}; //This line isn't even used anyway
                        glm::vec3 a = {verts.x.position.x, verts.x.position.y, verts.x.position.z};
                        glm::vec3 b = {verts.y.position.x, verts.y.position.y, verts.y.position.z};
                        glm::vec3 c = {verts.z.position.x, verts.z.position.y, verts.z.position.z};
                        glm::vec3 ba = {b.x - a.x, b.y - a.y, b.z - a.z};
                        glm::vec3 ca = {c.x - a.x, c.y - a.y, c.z - a.z};
                        glm::vec3 normal = normalize(cross(normalize(ba),normalize(ca)));
                        float l = sqrtf(normal.x * normal.x + normal.z * normal.z);
                        float cy = normal.z / l;
                        float sy = -normal.x / l;
                        if(fabs(l) < 0.001f) {
                            cy = 1.0f;
                            sy = 0.0f;
                        }
                        float cx = l;
                        float sx = normal.y;
                        glm::vec3 rxr0(1.0f, 0.0f, 0.0f);
                        glm::vec3 rxr1(0.0f, cx, sx);
                        glm::vec3 rxr2(0.0f, -sx, cx);
                        glm::vec3 ryr0(cy, 0.0f, -sy);
                        glm::vec3 ryr1(0.0f, 1.0f, 0.0f);
                        glm::vec3 ryr2(sy, 0.0f, cy);
                        glm::vec3 dotry = dotm(ba, ryr0, ryr1, ryr2);
                        glm::vec3 dotrxry = dotm(dotry, rxr0, rxr1, rxr2);
                        l = sqrtf(dotrxry.x * dotrxry.x + dotrxry.y * dotrxry.y);
                        float cz = dotrxry.x / l;
                        float sz = -dotrxry.y / l;
                        glm::vec3 rzr0(cz, sz, 0.0f);
                        glm::vec3 rzr1(-sz, cz, 0.0f);
                        glm::vec3 rzr2(0.0f, 0.0f, 1.0f);
                        glm::vec3 dotrz = dotm(dotrxry, rzr0, rzr1, rzr2);
                        dotry = dotm(ca, ryr0, ryr1, ryr2);
                        dotrxry = dotm(dotry, rxr0, rxr1, rxr2);
                        glm::vec3 dotrzrxry = dotm(dotrxry, rzr0, rzr1, rzr2);
                        glm::vec3 n0v(dotrzrxry.x - dotrz.x, dotrzrxry.y - dotrz.y, dotrzrxry.z - dotrz.z);
                        glm::vec3 n1v(-dotrzrxry.x, -dotrzrxry.y, -dotrzrxry.z);
                        glm::vec3 n0 = normalize(hat(n0v));
                        glm::vec3 n1 = normalize(hat(n1v));
                        float rotX = 360.0f - reverseAngle(cx, sx);
                        float rotY = 360.0f - reverseAngle(cy, sy);
                        float rotZ = 360.0f - reverseAngle(cz, sz);

                        dev << a.x; //X1 pos
                        dev << a.y; //Y1 pos
                        dev << a.z; //Z1 pos
                        dev << normal.x; //X normal
                        dev << normal.y; //Y normal
                        dev << normal.z; //Z normal
                        dev << convertRotation(glm::vec3(rotX, rotY, rotZ)); //XYZ rotation from the XZ plane
                        writeNull(dev, 2);
                        dev << dotrz.x; //DX2X1
                        dev << dotrz.y; //DY2X1
                        dev << dotrzrxry.x; //DX3X1
                        dev << dotrzrxry.y; //DY3X1
                        dev << n0.x; //X tangent
                        dev << n0.y; //Y tangent
                        dev << n1.x; //X bitangent
                        dev << n1.y; //Y bitangent
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
        foreach(const WS2Common::Scene::SceneNode *child, node->getChildren()) {
            writeCollisionTriangles(dev, child);
        }
    }

    void SMB2LzExporter::writeLevelModelPointerAList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node) {
        quint32 nextOffset = levelModelOffsetMap.key(node);

        foreach(WS2Common::Scene::SceneNode *node, node->getChildren()) {
            if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                writeNull(dev, 4);
                dev << (quint32) 0x00000001;
                dev << nextOffset;

                //Level models for the same collision header are just sequential stores, so it's fine to just add
                //on the length of a single level model
                nextOffset += LEVEL_MODEL_LENGTH;
            }
        }
    }

    void SMB2LzExporter::writeLevelModelPointerBList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node) {
        quint32 nextOffset = levelModelPointerAOffsetMap.key(node);

        foreach(WS2Common::Scene::SceneNode *node, node->getChildren()) {
            if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                dev << nextOffset;

                //Level model pointer type As for the same collision header are just sequential stores, so it's fine to
                //just add on the length of a single level model
                nextOffset += LEVEL_MODEL_POINTER_TYPE_A_LENGTH;
            }
        }
    }

    void SMB2LzExporter::writeLevelModelList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node) {
        foreach(WS2Common::Scene::SceneNode *node, node->getChildren()) {
            if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                writeNull(dev, 4);
                dev << levelModelNameOffsetMap.key(static_cast<WS2Common::Scene::MeshSceneNode*>(node)->getMeshName());
                writeNull(dev, 8);
            }
        }
    }

    void SMB2LzExporter::writeLevelModelNameList(QDataStream &dev, const WS2Common::Scene::GroupSceneNode *node) {
        foreach(WS2Common::Scene::SceneNode *node, node->getChildren()) {
            if (WS2Common::instanceOf<WS2Common::Scene::MeshSceneNode>(node)) {
                WS2Common::Scene::MeshSceneNode *mesh = static_cast<WS2Common::Scene::MeshSceneNode*>(node);

                //Write the object name
                dev.writeRawData(mesh->getMeshName().toLatin1(), mesh->getMeshName().size());

                writeNull(dev, 1); //Add a null terminator
                //Pad to 4 bytes
                writeNull(dev, roundUpNearest4(mesh->getMeshName().size() + 1) - (mesh->getMeshName().size() + 1));
            }
        }
    }

    void SMB2LzExporter::writeNull(QDataStream &dev, const unsigned int count) {
        for (unsigned int i = 0; i < count; i++) {
            dev.writeRawData("\0", 1);
        }
    }

    glm::tvec3<quint16> SMB2LzExporter::convertRotation(glm::vec3 rot) {
        rot.x = fmod(rot.x, 360.0f);
        rot.y = fmod(rot.y, 360.0f);
        rot.z = fmod(rot.z, 360.0f);

        if (rot.x < 0) rot.x += 360.0f;
        if (rot.y < 0) rot.y += 360.0f;
        if (rot.z < 0) rot.z += 360.0f;

        return glm::tvec3<quint16>(
                rot.x / 360.0f * 65536.0f,
                rot.y / 360.0f * 65536.0f,
                rot.z / 360.0f * 65536.0f
                );
    }

    quint32 SMB2LzExporter::addAllCounts(QMap<const WS2Common::Scene::GroupSceneNode*, quint32> &m) {
        quint32 total = 0;

        QMapIterator<const WS2Common::Scene::GroupSceneNode*, quint32> i(m);
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
    float SMB2LzExporter::toDegrees(float theta) {
        //WHAT THE HECK IS THIS NUMBER EVEN SUPPOSED TO BE!? -CraftedCart
        return 57.2957795130824 * theta;
    }

    glm::vec3 SMB2LzExporter::dotm(glm::vec3 a, glm::vec3 r0, glm::vec3 r1, glm::vec3 r2) {
        float d0 = (a.x * r0.x) + (a.y * r1.x) + (a.z * r2.x);
        float d1 = (a.x * r0.y) + (a.y * r1.y) + (a.z * r2.y);
        float d2 = (a.x * r0.z) + (a.y * r1.z) + (a.z * r2.z);
        return glm::vec3(d0, d1, d2);
    }

    float SMB2LzExporter::dot(glm::vec3 a, glm::vec3 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    glm::vec3 SMB2LzExporter::cross(glm::vec3 a, glm::vec3 b) {
        float d0 = (a.y * b.z) - (a.z * b.y);
        float d1 = (a.z * b.x) - (a.x * b.z);
        float d2 = (a.x * b.y) - (a.y * b.x);
        return glm::vec3(d0,d1,d2);
    }

    glm::vec3 SMB2LzExporter::normalize(glm::vec3 v) {
        glm::vec3 retVal = v;
        float len = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
        retVal.x /= len;
        retVal.y /= len;
        retVal.z /= len;
        return retVal;
    }

    glm::vec3 SMB2LzExporter::hat(glm::vec3 v) {
        return glm::vec3(-v.y, v.x, 0.0f);
    }

    float SMB2LzExporter::reverseAngle(float c, float s) {
        float a = toDegrees(asin(s));
        if (c < 0.0f) a = 180.0f - a;
        if (fabs(c) < fabs(s)) {
            a = toDegrees(acos(c));
            if(s < 0.0f) a = -a;
        }
        if (a < 0.0f) {
            if(a > -0.001f) a = 0.0f;
            else a += 360.0;
        }
        return a;
    }
}

