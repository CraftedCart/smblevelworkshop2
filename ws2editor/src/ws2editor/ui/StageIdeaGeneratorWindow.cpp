#include "ws2editor/ui/StageIdeaGeneratorWindow.hpp"
#include "ws2editor/ui_StageIdeaGeneratorWindow.h"
#include "ws2common/MathUtils.hpp"
#include <QFile>
#include <QStringList>
#include <QTextStream>

namespace WS2Editor {
    namespace UI {
        StageIdeaGeneratorWindow::StageIdeaGeneratorWindow(QWidget *parent) : QDialog(parent), ui(new Ui::StageIdeaGeneratorWindow) {
            ui->setupUi(this);

            //Read all themes
            QFile themeFile(tr(":/ws2editor/lang/stageThemes-en_US.txt"));
            if (themeFile.open(QIODevice::ReadOnly)) {
                QTextStream in(&themeFile);

                while (!in.atEnd()) {
                    QString line = in.readLine();
                    themes.append(line);
                }

                themeFile.close();
            }

            //Read all mechanics
            QFile mechanicsFile(tr(":/ws2editor/lang/stageMechanics-en_US.txt"));
            if (mechanicsFile.open(QIODevice::ReadOnly)) {
                QTextStream in(&mechanicsFile);

                while (!in.atEnd()) {
                    QString line = in.readLine();
                    mechanics.append(line);
                }

                mechanicsFile.close();
            }

            //Set up the connections
            connect(ui->genCombinationPushButton, &QPushButton::clicked, this, &StageIdeaGeneratorWindow::selectCombination);
            connect(ui->genThemePushButton, &QPushButton::clicked, this, &StageIdeaGeneratorWindow::selectTheme);
            connect(ui->genMechanicPushButton, &QPushButton::clicked, this, &StageIdeaGeneratorWindow::selectMechanic);
        }

        StageIdeaGeneratorWindow::~StageIdeaGeneratorWindow() {
            delete ui;
        }

        void StageIdeaGeneratorWindow::selectCombination() {
            selectTheme();
            selectMechanic();
        }

        void StageIdeaGeneratorWindow::selectTheme() {
            QString sel = themes.at(WS2Common::MathUtils::randInt(0, themes.size() - 1));
            ui->themesListWidget->insertItem(0, sel);
        }

        void StageIdeaGeneratorWindow::selectMechanic() {
            QString sel = mechanics.at(WS2Common::MathUtils::randInt(0, mechanics.size() - 1));
            ui->mechanicsListWidget->insertItem(0, sel);
        }
    }
}

