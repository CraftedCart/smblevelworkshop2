/**
 * @file
 * @brief Header for the CommandWidget class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_COMMANDWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_COMMANDWIDGET_HPP

#include <QLineEdit>
#include <QCompleter>

namespace WS2Editor {
    namespace UI {
        class CommandWidget : public QWidget {
            protected:
                QLineEdit *lineEdit;
                QCompleter *completer;

            protected:
                bool eventFilter(QObject *obj, QEvent *event) override;
                virtual void keyPressEvent(QKeyEvent *event) override;

            public:
                CommandWidget(QPoint pos, QWidget *parent);

            public slots:
                /**
                 * @brief Tries to run the command typed into the lineEdit
                 */
                void executeCommand();
        };
    }
}

#endif

