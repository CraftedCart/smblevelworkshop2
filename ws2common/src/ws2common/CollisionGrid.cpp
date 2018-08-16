#include "ws2common/CollisionGrid.hpp"
#include "ws2common/SerializeUtils.hpp"
#include <QDebug>

namespace WS2Common {
    void CollisionGrid::setGridStart(const glm::vec2 gridStart) {
        this->gridStart = gridStart;
    }

    const glm::vec2 CollisionGrid::getGridStart() const {
        return gridStart;
    }

    void CollisionGrid::setGridStep(const glm::vec2 gridStep) {
        this->gridStep = gridStep;
    }

    const glm::vec2 CollisionGrid::getGridStep() const {
        return gridStep;
    }

    void CollisionGrid::setGridStepCount(const glm::uvec2 gridStepCount) {
        this->gridStepCount = gridStepCount;
    }

    const glm::uvec2 CollisionGrid::getGridStepCount() const {
        return gridStepCount;
    }

    void CollisionGrid::serializeDataXml(QXmlStreamWriter &s) const {
        SerializeUtils::writeVec2(s, "start", gridStart, "x", "z");
        SerializeUtils::writeVec2(s, "step", gridStart, "x", "z");
        SerializeUtils::writeUVec2(s, "count", gridStepCount, "x", "z");
    }

    CollisionGrid CollisionGrid::deserializeDataXml(QXmlStreamReader &xml){
        CollisionGrid grid;

        while (!(xml.isEndElement() && xml.name() == "collisionGrid")) {
            xml.readNext();
            if (!xml.isStartElement()) continue; //Ignore all end elements

            if (xml.name() == "start") {
                grid.setGridStart(SerializeUtils::getVec2Attributes(xml.attributes(), "x", "z"));
            } else if (xml.name() == "step") {
                grid.setGridStep(SerializeUtils::getVec2Attributes(xml.attributes(), "x", "z"));
            } else if (xml.name() == "count") {
                grid.setGridStepCount(SerializeUtils::getUVec2Attributes(xml.attributes(), "x", "z"));
            } else {
                qWarning().noquote() << "Unrecognised tag: collisionGrid >" << xml.name();
            }
        }

        return grid;
    }
}

