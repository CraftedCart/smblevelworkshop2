#include "ws2lz/SMB2LzExporter.hpp"
#include "ws2lz/WS2Lz.hpp"
#include "ws2common/WS2Common.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include <QDebug>
#include <math.h>

namespace WS2Lz {
    void SMB2LzExporter::generate(QDataStream &dev, const WS2Common::Stage &stage) {
        dev.setByteOrder(QDataStream::BigEndian);
        dev.setFloatingPointPrecision(QDataStream::SinglePrecision);

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

        //TODO: Write collision triangles
        //TODO: Write collision triangle index list

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
        writeNull(dev, 4); //TODO: Offset to collision triangle list
        writeNull(dev, 4); //TODO: Offset to collision grid triangle list pointer
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
        for(unsigned int i = 0; i < count; i++) {
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
                rot.x / 360.0f * 65535.0f,
                rot.y / 360.0f * 65535.0f,
                rot.z / 360.0f * 65535.0f
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
}

