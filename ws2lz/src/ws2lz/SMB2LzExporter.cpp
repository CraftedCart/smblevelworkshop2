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
        levelModelCount = 0;
        levelModelPointerAListOffset = 0;
        levelModelPointerBListOffset = 0;
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
        dev << levelModelCount;
        dev << levelModelPointerAListOffset;
        dev << levelModelCount;
        dev << levelModelPointerBListOffset;
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
        writeNull(dev, 1080); //TODO: Everything else
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
}

