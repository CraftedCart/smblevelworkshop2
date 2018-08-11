#include "ws2common/config/StagedefConfigParser.hpp"
#include "ws2common/MessageHandler.hpp"
#include <QCoreApplication>
#include <QDebug>

//They're probably not so magical if there's so many of them
static const quint32 SMB1_MAGIC_NUMBERS[] = { //https://pastebin.com/iqQ5kvRs
    0x00000064,
    0x00000078,
    0x0000000A,
    0x0000000F,
    0x00000005,
    0x00000008,
    0x0000000E,
    0x00000019,
    0x00000014,
    0x0000001E,
    0x0000003C,
    0x0000001B,
    0x00000002,
    0x00000006,
    0x00000004,
    0x0000001F,
    0x00000012,
    0x0000001A,
    0x00000028,
    0x000001E0,
    0x000000F0,
    0x000000C8
};

static const quint32 SMB2_MAGIC_NUMBERS[] = { //https://pastebin.com/g9rvgUJz
    0x42C80000,
    0x447A0000,
    0x41F00000,
    0x42700000,
    0x41200000,
    0x45BB8000,
    0x453B8000,
    0x438C0000,
    0x43F00000,
    0x42A00000,
    0x42200000,
    0x44FA0000,
    0x41A00000,
    0x44E10000,
    0x40000000,
    0x40400000,
    0x43200000,
    0x43520000,
    0x42480000,
    0x43700000,
    0x44760000,
    0x43DC0000,
    0x442F0000,
    0x43480000
};

static const quint32 SMBDX_MAGIC_NUMBERS[] = { //https://pastebin.com/T2kaKTdQ
    0x0000C842,
    0x00007A44,
    0x0000F041,
    0x00007042,
    0x00002041,
    0x0080BB45,
    0x00803B45,
    0x00008C43,
    0x0000F043,
    0x0000A042,
    0x00002042,
    0x00004843,
    0x0000FA44,
    0x0000A041,
    0x0000E144,
    0x00000040,
    0x00004040,
    0x00002043,
    0x00005243,
    0x00004842,
    0x00007043,
    0x00007644,
    0x0000DC43,
    0x00002F44,
    0x0000C040,
    0x0000F042,
    0x0000A040,
    0x00000041,
    0x0000C841,
    0x0000D841,
    0x00008040,
    0x0000F841,
    0x00009041,
    0x00000042,
    0x00007041
};

static const quint32 SMB1_COLLISION_GROUP_LENGTH = 196;
static const quint32 GOAL_LENGTH = 0x14;
static const quint32 BUMPER_LENGTH = 0x20;
static const quint32 JAMABAR_LENGTH = 0x20;
static const quint32 BANANA_LENGTH = 0x10;
static const quint32 STAGE_OBJECT_LENGTH = 0xC;

namespace WS2Common {
    namespace Config {
        using namespace WS2Common::Scene;

        Stage* StagedefConfigParser::parseStage(QByteArray &bytes) {
            Stage *stage = new Stage();

            QDataStream stream(bytes);

            quint32 null;
            stream >> null;
            if (null != 0x00000000) {
                qCritical() << "Invalid stagedef (Does not start with 0x00000000 - Perhaps you forgot to decompress it?)";
                return stage;
            }

            //Check game version
            quint32 magicNumber;
            stream >> magicNumber;

            if (std::find(std::begin(SMB1_MAGIC_NUMBERS), std::end(SMB1_MAGIC_NUMBERS), magicNumber) !=
                    std::end(SMB1_MAGIC_NUMBERS)) {
                //SMB 1
                qDebug() << "Parsing SMB 1 stagedef";
                parseSmb1Stagedef(stage, bytes);

            } else if (std::find(std::begin(SMB2_MAGIC_NUMBERS), std::end(SMB2_MAGIC_NUMBERS), magicNumber) !=
                    std::end(SMB2_MAGIC_NUMBERS)) {
                //SMB 2
                qCritical() << "Parsing SMB 2 stagedefs has not been implemented yet";

            } else if (std::find(std::begin(SMBDX_MAGIC_NUMBERS), std::end(SMBDX_MAGIC_NUMBERS), magicNumber) !=
                    std::end(SMBDX_MAGIC_NUMBERS)) {
                //SMB 2
                qCritical() << "Parsing SMB Deluxe stagedefs has not been implemented yet";

            } else {
                qCritical() << "Invalid stagedef (No \"magic\" numbers matched - Perhaps you forgot to decompress it?)";
            }

            return stage;
        }

        void StagedefConfigParser::parseSmb1Stagedef(Stage *stage, QByteArray &bytes) {
            quint32 collisionGroupCount;
            quint32 collisionGroupOffset;
            quint32 startOffset;
            quint32 backgroundObjectCount;
            quint32 backgroundObjectOffset;

            QDataStream stream(bytes);
            configureDataStream(stream);

            stream.skipRawData(0x8); //Magic number
            stream >> collisionGroupCount;
            stream >> collisionGroupOffset;
            stream >> startOffset;
            stream.skipRawData(0x04 * 11); //Ignore stuff that's duplicated in collision groups anyway
            stream.skipRawData(0x10); //Marked as Zero
            stream.skipRawData(0x8); //Number/offset to something
            stream.skipRawData(0x8); //Number/offset to level models
            stream.skipRawData(0x8); //Marked as Zero
            stream >> backgroundObjectCount;
            stream >> backgroundObjectOffset;
            stream.skipRawData(0x8); //Number/offset to something
            stream.skipRawData(0x8); //Unknown: 0x00000000 0x00000001
            stream.skipRawData(0x8); //Number/offset to reflective objects
            stream.skipRawData(0x18); //Unknown

            stage->getRootNode()->addChild(parseStart(bytes, startOffset));

            for (quint32 i = 0; i < collisionGroupCount; ++i) {
                quint32 offset = collisionGroupOffset + SMB1_COLLISION_GROUP_LENGTH * i;
                stage->getRootNode()->addChild(parseSmb1CollisionGroup(bytes, offset));
            }
        }

        GroupSceneNode* StagedefConfigParser::parseSmb1CollisionGroup(QByteArray &bytes, quint32 offset) {
            QDataStream stream(bytes);
            configureDataStream(stream);
            stream.skipRawData(offset);

            glm::vec3 originPos;
            glm::tvec3<quint16> originRot;
            quint32 animationDataOffset;
            quint32 modelNamePointerPointer;
            quint32 triangleDataOffset;
            quint32 gridTriangleListOffset;
            glm::vec2 gridStart;
            glm::vec2 gridStep;
            glm::tvec2<quint32> gridStepCount;

            quint32 goalCount;
            quint32 goalOffset;
            quint32 bumperCount;
            quint32 bumperOffset;
            quint32 jamabarCount;
            quint32 jamabarOffset;
            quint32 bananaCount;
            quint32 bananaOffset;
            quint32 stageObjectCount;
            quint32 stageObjectOffset;

            GroupSceneNode *node = new GroupSceneNode(
                    QCoreApplication::translate("StagedefConfigParser", "Group at 0x%1")
                    .arg(QString::number(offset, 16).toUpper())
                    );

            stream >> originPos.x;
            stream >> originPos.y;
            stream >> originPos.z;
            stream >> originRot.x;
            stream >> originRot.y;
            stream >> originRot.z;
            stream.skipRawData(2);
            stream >> animationDataOffset;
            stream >> modelNamePointerPointer;
            stream >> triangleDataOffset;
            stream >> gridTriangleListOffset;
            stream >> gridStart.x;
            stream >> gridStart.y;
            stream >> gridStep.x;
            stream >> gridStep.y;
            stream >> gridStepCount.x;
            stream >> gridStepCount.y;
            stream >> goalCount;
            stream >> goalOffset;
            stream.skipRawData(0x8); //Skip goal count again, and zeroes
            stream >> bumperCount;
            stream >> bumperOffset;
            stream >> jamabarCount;
            stream >> jamabarOffset;
            stream >> bananaCount;
            stream >> bananaOffset;
            stream.skipRawData(0x18); //Unknowns/nulls
            stream >> stageObjectCount;
            stream >> stageObjectOffset;

            node->setOriginPosition(originPos);
            node->setOriginRotation(rotationShortToRadians(originRot));

            node->getCollisionGrid().setGridStart(gridStart);
            node->getCollisionGrid().setGridStep(gridStep);
            node->getCollisionGrid().setGridStepCount(gridStepCount);

            //Goals
            for (quint32 i = 0; i < goalCount; ++i) {
                quint32 offset = goalOffset + GOAL_LENGTH * i;
                node->addChild(parseSmb1Goal(bytes, offset));
            }

            //Bumpers
            for (quint32 i = 0; i < bumperCount; ++i) {
                quint32 offset = bumperOffset + BUMPER_LENGTH * i;
                node->addChild(parseBumper(bytes, offset));
            }

            //Jamabars
            for (quint32 i = 0; i < jamabarCount; ++i) {
                quint32 offset = jamabarOffset + JAMABAR_LENGTH * i;
                node->addChild(parseJamabar(bytes, offset));
            }

            //Bananas
            for (quint32 i = 0; i < bananaCount; ++i) {
                quint32 offset = bananaOffset + BANANA_LENGTH * i;
                node->addChild(parseBanana(bytes, offset));
            }

            //Stage objects
            for (quint32 i = 0; i < stageObjectCount; ++i) {
                quint32 offset = stageObjectOffset + STAGE_OBJECT_LENGTH * i;
                node->addChild(parseLevelModel(bytes, offset));
            }

            return node;
        }

        GoalSceneNode* StagedefConfigParser::parseSmb1Goal(QByteArray &bytes, quint32 offset) {
            QDataStream stream(bytes);
            configureDataStream(stream);
            stream.skipRawData(offset);

            glm::vec3 pos;
            glm::tvec3<quint16> rotShort;
            quint16 type;

            GoalSceneNode *node = new GoalSceneNode(
                    QCoreApplication::translate("StagedefConfigParser", "Goal at 0x%1")
                    .arg(QString::number(offset, 16).toUpper())
                    );

            stream >> pos.x;
            stream >> pos.y;
            stream >> pos.z;
            stream >> rotShort.x;
            stream >> rotShort.y;
            stream >> rotShort.z;
            stream >> type;

            node->setPosition(pos);
            node->setRotation(rotationShortToRadians(rotShort));

            switch (type) {
                case 0x4700: node->setType(EnumGoalType::GREEN); break;
                case 0x5200: node->setType(EnumGoalType::RED); break;
                default: node->setType(EnumGoalType::BLUE); break;
            }

            return node;
        }

        BumperSceneNode* StagedefConfigParser::parseBumper(QByteArray &bytes, quint32 offset) {
            QDataStream stream(bytes);
            configureDataStream(stream);
            stream.skipRawData(offset);

            glm::vec3 pos;
            glm::tvec3<quint16> rotShort;
            glm::vec3 scl;

            BumperSceneNode *node = new BumperSceneNode(
                    QCoreApplication::translate("StagedefConfigParser", "Bumper at 0x%1")
                    .arg(QString::number(offset, 16).toUpper())
                    );

            stream >> pos.x;
            stream >> pos.y;
            stream >> pos.z;
            stream >> rotShort.x;
            stream >> rotShort.y;
            stream >> rotShort.z;
            stream.skipRawData(2);
            stream >> scl.x;
            stream >> scl.y;
            stream >> scl.z;

            node->setPosition(pos);
            node->setRotation(rotationShortToRadians(rotShort));
            node->setScale(scl);

            return node;
        }

        JamabarSceneNode* StagedefConfigParser::parseJamabar(QByteArray &bytes, quint32 offset) {
            QDataStream stream(bytes);
            configureDataStream(stream);
            stream.skipRawData(offset);

            glm::vec3 pos;
            glm::tvec3<quint16> rotShort;
            glm::vec3 scl;

            JamabarSceneNode *node = new JamabarSceneNode(
                    QCoreApplication::translate("StagedefConfigParser", "Jamabar at 0x%1")
                    .arg(QString::number(offset, 16).toUpper())
                    );

            stream >> pos.x;
            stream >> pos.y;
            stream >> pos.z;
            stream >> rotShort.x;
            stream >> rotShort.y;
            stream >> rotShort.z;
            stream.skipRawData(2);
            stream >> scl.x;
            stream >> scl.y;
            stream >> scl.z;

            node->setPosition(pos);
            node->setRotation(rotationShortToRadians(rotShort));
            node->setScale(scl);

            return node;
        }

        BananaSceneNode* StagedefConfigParser::parseBanana(QByteArray &bytes, quint32 offset) {
            QDataStream stream(bytes);
            configureDataStream(stream);
            stream.skipRawData(offset);

            glm::vec3 pos;
            quint32 type;

            BananaSceneNode *node = new BananaSceneNode(
                    QCoreApplication::translate("StagedefConfigParser", "Banana at 0x%1")
                    .arg(QString::number(offset, 16).toUpper())
                    );

            stream >> pos.x;
            stream >> pos.y;
            stream >> pos.z;
            stream >> type;

            node->setPosition(pos);
            if (type == 0x00000001) {
                node->setType(EnumBananaType::BUNCH);
            } else {
                node->setType(EnumBananaType::SINGLE);
            }

            return node;
        }

        MeshSceneNode* StagedefConfigParser::parseLevelModel(QByteArray &bytes, quint32 offset) {
            QDataStream stream(bytes);
            configureDataStream(stream);
            stream.skipRawData(offset);

            quint32 nameOffset;
            QString name;

            stream.skipRawData(4);
            stream >> nameOffset;
            name = QString::fromLatin1(&bytes.constData()[nameOffset]);

            MeshSceneNode *node = new MeshSceneNode(
                    QCoreApplication::translate("StagedefConfigParser", "Stage model \"%1\" at 0x%2")
                    .arg(name)
                    .arg(QString::number(offset, 16).toUpper())
                    );

            node->setMeshName(name);

            return node;
        }

        StartSceneNode* StagedefConfigParser::parseStart(QByteArray &bytes, quint32 offset) {
            QDataStream stream(bytes);
            configureDataStream(stream);
            stream.skipRawData(offset);

            glm::vec3 pos;
            glm::tvec3<quint16> rotShort;

            StartSceneNode *node = new StartSceneNode(
                    QCoreApplication::translate("StagedefConfigParser", "Start at 0x%1")
                    .arg(QString::number(offset, 16).toUpper())
                    );

            stream >> pos.x;
            stream >> pos.y;
            stream >> pos.z;
            stream >> rotShort.x;
            stream >> rotShort.y;
            stream >> rotShort.z;

            node->setPosition(pos);
            node->setRotation(rotationShortToRadians(rotShort));

            return node;
        }

        glm::vec3 StagedefConfigParser::rotationShortToRadians(glm::tvec3<quint16> vec) {
            return glm::vec3(
                    (float) vec.x / 65535.0f * M_PI * 1.99996948242f, //* M_PI * 1.99... as that equals 359.9945 degrees
                    (float) vec.y / 65535.0f * M_PI * 1.99996948242f,
                    (float) vec.z / 65535.0f * M_PI * 1.99996948242f
                    );
        }

        void StagedefConfigParser::configureDataStream(QDataStream &stream) {
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        }
    }
}

