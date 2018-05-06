#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_VIEWPORTWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_VIEWPORTWIDGET_HPP

#include "ws2editor/glplatform.hpp"
#include "ws2editor/EnumCameraNav.hpp"
#include "ws2editor/resource/ResourceScene.hpp"
#include "ws2editor/RenderManager.hpp"
#include <QOpenGLWidget>
#include <QWidget>
#include <QElapsedTimer>
#include <QOpenGLTexture>
#include <QSet>
#include <QString>
#include <QColor>

namespace WS2Editor {
    namespace Widget {
        class ViewportWidget : public QOpenGLWidget {
            Q_OBJECT

            protected:
                RenderManager *renderManager = new RenderManager();

                //Used to calculate delta time
                QElapsedTimer elapsedTimer;
                qint64 prevNanosecondsElapsed;
                qint64 deltaNanoseconds;
                float deltaSeconds;

                QSet<int> keysDown;
                bool mouseLocked = false;

                glm::vec3 targetCameraPos = glm::vec3(10.0f, 10.0f, 10.0f);
                glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 10.0f);
                glm::vec2 cameraRot = glm::vec2(0.0f, 0.0f);
                float cameraPivotDistance = 15.0f;
                glm::vec3 forward;
                glm::vec3 right;
                glm::vec3 up;
                EnumCameraNav cameraNavMode = EnumCameraNav::NAV_FIXED;

                glm::mat4 proj;
                glm::mat4 view;

                /**
                 * @brief Only exists when within paintGL
                 */
                QPainter *painter;

                QPixmap *tooltipPixmap;
                QPixmap *importPixmap;

                /**
                 * @brief Should drag and drop info be shown around the cursor
                 */
                bool showDrop = false;

                /**
                 * @brief Tip of the day string
                 */
                QString tip;

                //Gizmo collision bounds & highlight
                bool isGizmoPhysicsInWorld = false;
                PhysicsContainer *gizmoYPhysics;
                bool highlightGizmoY;
                PhysicsContainer *gizmoXPhysics;
                bool highlightGizmoX;
                PhysicsContainer *gizmoZPhysics;
                bool highlightGizmoZ;

            public:
                explicit ViewportWidget(QWidget *parent = nullptr);
                ~ViewportWidget();

                qint64 getDeltaNanoseconds();
                float getDeltaSeconds();

                RenderManager* getRenderManager();

                void makeCurrentContext();

            protected:
                virtual void initializeGL() override;
                virtual void resizeGL(int w, int h) override;
                virtual void paintGL() override;
                virtual void keyPressEvent(QKeyEvent *event) override;
                virtual void keyReleaseEvent(QKeyEvent *event) override;
                virtual void mousePressEvent(QMouseEvent *event) override;
                virtual void mouseReleaseEvent(QMouseEvent *event) override;
                virtual void wheelEvent(QWheelEvent *event) override;
                virtual void dragEnterEvent(QDragEnterEvent *event) override;
                virtual void dragMoveEvent(QDragMoveEvent *event) override;
                virtual void dropEvent(QDropEvent *event) override;
                virtual void dragLeaveEvent(QDragLeaveEvent *event) override;

                /**
                 * @brief Gets the cursor position relative to the this widget
                 *
                 * @return The cursor position relative to this widget
                 */
                glm::vec2 getRelativeCursorPos();

                /**
                 * @brief Draws a tooltip of the object at a pixel position
                 *
                 * @param painter The painter to draw with
                 * @param pos The position in pixels to draw the tooltip at relative to this widget
                 */
                void drawObjectTooltipAtPos(QPainter &painter, glm::vec2 pos);

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
                 * @brief Updates the transformation gizmos and enqueues a draw command for it (If need be)
                 */
                void updateGizmos();

                /**
                 * @brief Draws 2D text at the 3D point
                 *
                 * This calls void drawText(const glm::vec4 pos, const QString &str, const QColor &col) - but converts the
                 * provided vec3 to a vec4
                 *
                 * @param painter The painter to draw with
                 * @param pos The 3D pos to draw the text at
                 * @param str The string to draw
                 * @param col The color of the text
                 */
                void drawText(QPainter &painter, const glm::vec3 &pos, const QString &str, const QColor &col);

                /**
                 * @brief Draws info on the viewport (such as a tip of the day)
                 *
                 * For use when the scene is empty, such as before a project is opened or before any models have been
                 * imported.
                 *
                 * @param painter The painter to draw with
                 */
                void drawEmptySceneInfo(QPainter &painter);

                /**
                 * @brief Calculates for forward vector
                 */
                glm::vec3 calcForwardVector(glm::vec2 &rot);

                /**
                 * @brief Calculates the forward, right and up vectors
                 */
                void calcVectors();

                /**
                 * @brief Performs a raycast taking normalized device coordinates into mind
                 * Useful for camera
                 *
                 * @param pos The position in camera space to start raycasting from
                 * @param startPos The position of the camera
                 * @param distance The distance that the raycast should test for
                 * @param proj The projection matrix
                 * @param view The view matrix
                 *
                 * @return A pointer to the raycast callback (Don't forget to delete this when you're done!)
                 */
                btCollisionWorld::AllHitsRayResultCallback* ndcRaycast(const glm::vec2 pos, const glm::vec3 startPos,
                        const float distance, const glm::mat4 proj, const glm::mat4 view);

                /**
                 * @brief Sorts the ray callback result values by each hit point's distance from sourcePoint and returns an indices vector
                 *
                 * Elements are not directly sorted in the rayCallback. Rather, an indices vector is created, sorted based on the fractions in the ray callback, and returned
                 *
                 * @param rayCallback The ray callback result to sort the returned indices vector
                 *
                 * @return An indices array sorted based on distance
                 */
                QVector<int> sortAllHitsRayResultCallback(const btCollisionWorld::AllHitsRayResultCallback *rayCallback);

                /**
                 * @brief Selects the node at the given gcreen position
                 *
                 * @param pos The screen position
                 * @param toggleSelect If true, the selection of the object will be toggled, else the selection will be
                 *                     overwritten with the object at the screen position
                 */
                void selectNodeAtScreenPos(const glm::vec2 pos, bool toggleSelect);

            signals:
                void frameRendered(qint64 deltaNanoseconds);
        };
    }
}

#endif

