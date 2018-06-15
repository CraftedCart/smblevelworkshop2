#include "ws2editor/widget/PropertiesWidget.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

namespace WS2Editor {
    namespace Widget {
        PropertiesWidget::PropertiesWidget(QWidget *parent) : QWidget(parent) {}

        void PropertiesWidget::updatePropertiesWidget(QVector<WS2Common::Scene::SceneNode*>& nodes) {
            qDeleteAll(children());

            layout = new QVBoxLayout();
            layout->setAlignment(Qt::AlignTop);
            layout->setMargin(0);
            setLayout(layout);

            emit onUpdatePropertiesWidget(layout, nodes);
        }
    }
}

