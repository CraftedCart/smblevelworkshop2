#include "ws2lz/SMB2LzExporter.hpp"
#include "ws2lz/WS2Lz.hpp"
#include "ws2common/WS2Common.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include <QDebug>
#include <math.h>

namespace WS2Lz {
    void SMB2LzExporter::generate(QDataStream &dev, const WS2Common::Stage &stage) {
        dev.setByteOrder(QDataStream::BigEndian);
        dev.setFloatingPointPrecision(QDataStream::SinglePrecision);

        calculateOffsets(stage);
        writeFileHeader(dev, stage);
        writeStart(dev, stage);
        writeFallout(dev, stage);
    }

    void SMB2LzExporter::calculateOffsets(const WS2Common::Stage &stage) {
        //TODO: Calculate offsets
        qWarning() << "calculateOffsets NYI!";

        quint32 nextOffset = FILE_HEADER_LENGTH;

        startOffset = nextOffset;
        nextOffset += START_LENGTH;

        falloutOffset = nextOffset;
        nextOffset += FALLOUT_LENGTH;

        //Just set all this guff to null for now in case it isn't
        collisionHeaderListOffset = 0;
        collisionHeaderCount = 0; //TODO: Set to null for now as NYI
        //collisionHeaderCount = stage.getRootNode()->getChildCount(); //Number of collision headers
        //startOffset = 0;
        //falloutOffset = 0;
        goalCount = 0;
        goalListOffset = 0;
        bumperCount = 0;
        bumperListOffset = 0;
        jamabarCount = 0;
        jamabarListOffset = 0;
        bananaCount = 0;
        bananaListOffset = 0;
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

    void SMB2LzExporter::writeFileHeader(QDataStream &dev, const WS2Common::Stage &stage) {
        writeNull(dev, 4); dev << 0x447A0000; //Magic number (Probably)
        dev << collisionHeaderCount;
        dev << collisionHeaderListOffset;
        dev << startOffset;
        dev << falloutOffset;
        dev << goalCount;
        dev << goalListOffset;
        dev << bumperCount;
        dev << bumperListOffset;
        dev << jamabarCount;
        dev << jamabarListOffset;
        dev << bananaCount;
        dev << bananaListOffset;
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
}

