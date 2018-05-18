#include "ws2editor/ui/PluginsWindow.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#include <QGridLayout>
#include <QListWidget>
#include <QPainter>

namespace WS2Editor {
    namespace UI {
        PluginsWindow::PluginsWindow(QWidget *parent) : QDialog(parent) {
            setWindowTitle(tr("Plugins"));
            resize(720, 480);

            QGridLayout *layout = new QGridLayout(this);
            setLayout(layout);

            QListWidget *listWidget = new QListWidget(this);
            layout->addWidget(listWidget);

            //Create loaded, initialized, and failed icons
            QImage pluginImage = QImage(":/Workshop2/Icons/Plugin.png").scaled(24, 24);
            QImage pluginFailImage = QImage(":/Workshop2/Icons/PluginFail.png").scaled(24, 24);

            QImage loadedImage(24, 24, QImage::Format_ARGB32);
            tintImage(loadedImage, pluginFailImage, QColor(255, 235, 59));
            QPixmap loadedPixmap;
            loadedPixmap.convertFromImage(loadedImage);
            QIcon loadedIcon(loadedPixmap);

            QImage initializedImage(24, 24, QImage::Format_ARGB32);
            tintImage(initializedImage, pluginImage, QColor(76, 175, 80));
            QPixmap initializedPixmap;
            initializedPixmap.convertFromImage(initializedImage);
            QIcon initializedIcon(initializedPixmap);

            QImage failedImage(24, 24, QImage::Format_ARGB32);
            tintImage(failedImage, pluginFailImage, QColor(244, 67, 54));
            QPixmap failedPixmap;
            failedPixmap.convertFromImage(failedImage);
            QIcon failedIcon(failedPixmap);

            //Fetch plugins and add themto the list
            WS2EditorInstance *ws2Instance = WS2EditorInstance::getInstance();
            for (QPluginLoader *loader : ws2Instance->getLoadedPlugins()) {
                listWidget->addItem(loader->fileName());

                if (ws2Instance->getInitializedPlugins().contains(loader)) {
                    //It's been initialized
                    listWidget->item(listWidget->count() - 1)->setIcon(initializedIcon);
                } else {
                    //It failed to initialize
                    listWidget->item(listWidget->count() - 1)->setIcon(loadedIcon);
                }
            }

            for (QPluginLoader *loader : ws2Instance->getFailedPlugins()) {
                listWidget->addItem(loader->fileName());
                listWidget->item(listWidget->count() - 1)->setIcon(failedIcon);
            }
        }

        void PluginsWindow::tintImage(QImage &output, QImage &input, QColor tint) {
            for (int y = 0; y < output.height(); y++) {
                for (int x = 0; x < output.width(); x++) {
                    tint.setAlpha(input.pixelColor(x, y).alpha());
                    output.setPixelColor(x, y, tint);
                }
            }
        }
    }
}

