/**
 * @file
 * @brief Header for the ProfileWidget class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_PROFILEWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_PROFILEWIDGET_HPP

#include "ws2common/profile/Profiler.hpp"
#include <QWidget>

namespace WS2Editor {
    namespace Widget {
        class ProfilerWidget : public QWidget {
            Q_OBJECT

            protected:
                WS2Common::Profile::Profiler *profiler;

            protected:
                virtual void paintEvent(QPaintEvent *event) override;

            public:
                ProfilerWidget(QWidget *parent = nullptr);

            public slots:
                void onProfilerNextFrame(WS2Common::Profile::Profiler &profiler);
        };
    }
}

#endif

