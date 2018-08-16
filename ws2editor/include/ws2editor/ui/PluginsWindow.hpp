/**
 * @file
 * @brief Header for the PluginsWindow class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_PLUGINSWINDOW_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_PLUGINSWINDOW_HPP

#include "ws2editor_export.h"
#include <QDialog>

namespace WS2Editor {
    namespace UI {
        class WS2EDITOR_EXPORT PluginsWindow : public QDialog {
            Q_OBJECT

            public:
                explicit PluginsWindow(QWidget *parent = 0);

            protected:
                void tintImage(QImage &output, QImage &input, QColor tint);
        };
    }
}

#endif

