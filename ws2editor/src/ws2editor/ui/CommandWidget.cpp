#include "ws2editor/ui/CommandWidget.hpp"
#include "ws2editor/command/CommandInterpreter.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QDebug>

namespace WS2Editor {
    namespace UI {
        CommandWidget::CommandWidget(QPoint pos, QWidget *parent) : QWidget(parent) {
            using namespace WS2Editor::Command;

            setAttribute(Qt::WA_DeleteOnClose);
            setWindowFlags(windowFlags() | Qt::Popup);

            QHBoxLayout *layout = new QHBoxLayout;

            QLabel *label = new QLabel(this);
            label->setPixmap(QPixmap(":/Workshop2/Icons/Console.png"));
            label->setScaledContents(true);
            label->setFixedSize(24, 24);
            layout->addWidget(label);

            lineEdit = new QLineEdit(this);
            lineEdit->setFixedWidth(512);
            layout->addWidget(lineEdit);

            setLayout(layout);
            lineEdit->setFocus();

            //Make sure the widget is on screen
            adjustSize();

            QRect desktop = QApplication::desktop()->screenGeometry();
            if (pos.x() + width() > desktop.width()) pos.setX(desktop.width() - width());
            if (pos.y() + height() > desktop.height()) pos.setY(desktop.height() - height());

            //Completion
            QStringList wordList;

            for (ICommandHandler *handler : CommandInterpreter::getInstance()->getRegisteredCommands()) {
                wordList << handler->getCommand();
            }

            completer = new QCompleter(wordList, this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            lineEdit->setCompleter(completer);

            move(pos);

            completer->popup()->installEventFilter(this);
            installEventFilter(this);

            connect(lineEdit, &QLineEdit::returnPressed, this, &CommandWidget::executeCommand);
        }

        bool CommandWidget::eventFilter(QObject *obj, QEvent *event) {
            if (obj == completer->popup()) {
                if (event->type() == QEvent::KeyPress) {
                    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                    if (keyEvent->key() == Qt::Key_Tab) {
                        //Select the next item in the completion list when tab is pressed - Supertab style
                        QModelIndex index = completer->popup()->currentIndex();
                        QModelIndex newIndex;

                        if (index.row() == -1) {
                            //Nothing's selected - select the first entry
                            newIndex = completer->popup()->model()->index(0, 0);
                        } else {
                            //Something's already selected - select the next one
                            //Going off the end of the list will automatically deselect everything
                            newIndex = completer->popup()->model()->index(index.row() + 1, index.column());
                        }

                        completer->popup()->setCurrentIndex(newIndex);

                        return true;
                    }
                }
            } else {
                if (event->type() == QEvent::KeyPress) {
                    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                    if (keyEvent->key() == Qt::Key_Return) {
                        close();

                        return true;
                    }
                }
            }

            //Pass the event on to the parent class WS2EDITOR_EXPORT
            return QWidget::eventFilter(obj, event);
        }

        void CommandWidget::executeCommand() {
            using namespace WS2Editor::Command;

            CommandReply reply = CommandInterpreter::getInstance()->runCommand(lineEdit->text());

            if (reply.getTranslatedMessage().length() == 0) {
                close();
            } else {
                delete lineEdit;

                QLabel *messageLabel = new QLabel(this);
                messageLabel->setText(reply.getTranslatedMessage());
                layout()->addWidget(messageLabel);
            }
        }
    }
}

