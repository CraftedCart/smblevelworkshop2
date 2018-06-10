/**
 * @file
 * @brief Wormhole widget for the properties panel
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_WORMHOLEWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_WORMHOLEWIDGET_HPP

#include "ws2editor/widget/CollapsableWidget.hpp"
#include "ws2editor/RenderManager.hpp"
#include "ws2common/scene/WormholeSceneNode.hpp"
#include <QLineEdit>
#include <QLabel>
#include <QVector>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        class WormholeWidget : public WS2Editor::Widget::CollapsableWidget {
            Q_OBJECT

            protected:
                //Used to fetch banana models when reconstructing the mesh data
                WS2Editor::RenderManager *renderManager;

                QVector<WS2Common::Scene::WormholeSceneNode*> selectedWormholeNodes;

                QPixmap alertPixmap;
                QPixmap wormholePixmap;

                QLineEdit *destinationUuidLineEdit;

                QWidget *destinationAlertWidget;
                QLabel *alertIconLabel;
                QLabel *alertTextLabel;

                QWidget *destinationInfoWidget;
                QLabel *infoIconLabel;
                QLabel *infoTextLabel;

            protected:
                void updateValues();

            public:
                WormholeWidget(
                        QVector<WS2Common::Scene::WormholeSceneNode*> &nodes,
                        QString label = "",
                        QWidget *parent = nullptr
                        );

            public slots:
                void onNodeModified(WS2Common::Scene::SceneNode *node);
                void onDestinationModifiedConfirmed(const QString &newDestinationUuid);
                void onDestinationEdited(const QString &newDestinationUuid);
        };
    }
}

#endif

