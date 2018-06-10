/**
 * @file
 * @brief Header for the collabsable widget class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_PPLUGIN_HPPLUGIN_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_PPLUGIN_HPPLUGIN_HPP

#include "ws2editor_export.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

namespace WS2Editor {
    namespace Widget {
        class WS2EDITOR_EXPORT CollapsableWidget : public QWidget {
            Q_OBJECT

            protected:
                QVBoxLayout mainLayout;
                QToolButton toggleButton;
                QParallelAnimationGroup toggleAnimation;
                QWidget contentArea;
                int animationDuration;

            public:
                explicit CollapsableWidget(const QString &title = "", const int animationDuration = 100, QWidget *parent = nullptr);
                void setContentLayout(QLayout &contentLayout);

                void toggleContentShown(bool visible);

            public slots:
                void animationFinished();
        };
    }
}

#endif

