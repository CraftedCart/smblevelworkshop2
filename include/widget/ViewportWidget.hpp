#ifndef SMBLEVELWORKSHOP2_WIDGET_VIEWPORTWIDGET_HPP
#define SMBLEVELWORKSHOP2_WIDGET_VIEWPORTWIDGET_HPP

#include "glplatform.hpp"
#include "EnumCameraNav.hpp"
#include "resource/ResourceScene.hpp"
#include <QOpenGLWidget>
#include <QWidget>
#include <QElapsedTimer>
#include <QOpenGLTexture>
#include <QSet>
#include <QString>
#include <QColor>

namespace WS2 {
    namespace Widget {
        class ViewportWidget : public QOpenGLWidget {
            Q_OBJECT

            protected:
                //Used to calculate delta time
                QElapsedTimer elapsedTimer;
                qint64 prevNanosecondsElapsed;
                qint64 deltaNanoseconds;
                float deltaSeconds;

                QSet<int> *keysDown = new QSet<int>();
                bool mouseLocked = false;

                float fov = 90.0f;
                glm::vec3 *cameraPos = new glm::vec3(10.0f, 10.0f, 10.0f);
                glm::vec2 *cameraRot = new glm::vec2(0.0f, 0.0f);
                float cameraPivotDistance = 15.0f;
                glm::vec3 forward;
                glm::vec3 right;
                glm::vec3 up;
                float posSpeed = 24.0f;
                float rotSpeed = 0.01f;
                EnumCameraNav cameraNavMode = EnumCameraNav::NAV_FIXED;

                glm::mat4 proj;
                glm::mat4 view;

            public:
                explicit ViewportWidget(QWidget *parent = nullptr);
                ~ViewportWidget();

                qint64 getDeltaNanoseconds();
                float getDeltaSeconds();

                void makeCurrentContext();

            protected:
                void initializeGL() override;
                //void resizeGL(int w, int h) override;
                void paintGL() override;
                void keyPressEvent(QKeyEvent *event) override;
                void keyReleaseEvent(QKeyEvent *event) override;
                void mousePressEvent(QMouseEvent *event) override;
                void mouseReleaseEvent(QMouseEvent *event) override;
                void wheelEvent(QWheelEvent *event) override;

                /**
                 * @brief Checks if a key is down
                 *
                 * @param key The key - Use the keys defined within the Qt namespace
                 *
                 * @return True is the key is down, false otherwise
                 */
                bool isKeyDown(int key);

                /**
                 * @brief Called before drawing with OpenGL
                 */
                void preDraw();

                /**
                 * @brief Draws 2D text at the 3D point
                 *
                 * This calls void drawText(const glm::vec4 pos, const QString &str, const QColor &col) - but converts the
                 * provided vec3 to a vec4
                 *
                 * @param pos The 3D pos to draw the text at
                 * @param str The string to draw
                 * @param col The color of the text
                 */
                void drawText(const glm::vec3 &pos, const QString &str, const QColor &col);

                /**
                 * @brief Recursively draws the node and all the node's children
                 *
                 * @param node The node to draw and/or recursively iterate over its children to draw
                 * @param transform The world transform of the parent node
                 */
                void recursiveDrawSceneNode(Scene::SceneNode *node, const glm::mat4 parentTransform) const;

                /**
                 * @brief Calculates for forward vector
                 */
                glm::vec3 calcForwardVector(glm::vec2 &rot);

                /**
                 * @brief Calculates the forward, right and up vectors
                 */
                void calcVectors();

                /**
                 * @brief Checks for OpenGL errors and logs them if any are found
                 *
                 * @param location This text is tacked on to the end of the log message.
                 *                 It's recommended you put where in the code the function is called, to aid with tracking
                 *                 down issues.
                 */
                void checkGLErrors(QString location);

                void selectNodeAtScreenPos(const glm::vec2 pos);

            signals:
                void frameRendered(qint64 deltaNanoseconds);
        };
    }
}

#endif

