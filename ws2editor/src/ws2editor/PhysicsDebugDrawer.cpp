#include "ws2editor/PhysicsDebugDrawer.hpp"
#include "ws2editor/GLManager.hpp"
#include <QDebug>

namespace WS2Editor {
    PhysicsDebugDrawer::PhysicsDebugDrawer() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }

    PhysicsDebugDrawer::~PhysicsDebugDrawer() {
        glDeleteBuffers(1, &vbo);
    }

    void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) {
        lineData.append(WS2Common::MathUtils::toGlmVec3(from));
        lineData.append(WS2Common::MathUtils::toGlmVec3(fromColor));
        lineData.append(WS2Common::MathUtils::toGlmVec3(to));
        lineData.append(WS2Common::MathUtils::toGlmVec3(toColor));
    }

    void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
        lineData.append(WS2Common::MathUtils::toGlmVec3(from));
        lineData.append(WS2Common::MathUtils::toGlmVec3(color));
        lineData.append(WS2Common::MathUtils::toGlmVec3(to));
        lineData.append(WS2Common::MathUtils::toGlmVec3(color));
    }

    void PhysicsDebugDrawer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color) {
        //Suppress compiler warnings
        Q_UNUSED(p);
        Q_UNUSED(radius);
        Q_UNUSED(color);

        qDebug() << "PhysicsDebugDrawer: drawSphere not yet implemented!";
    }

    void PhysicsDebugDrawer::drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha) {
        //Suppress compiler warnings
        Q_UNUSED(a);
        Q_UNUSED(b);
        Q_UNUSED(c);
        Q_UNUSED(color);
        Q_UNUSED(alpha);

        qDebug() << "PhysicsDebugDrawer: drawTriangle not yet implemented!";
    }

    void PhysicsDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
        //Suppress compiler warnings
        Q_UNUSED(pointOnB);
        Q_UNUSED(normalOnB);
        Q_UNUSED(distance);
        Q_UNUSED(lifeTime);
        Q_UNUSED(color);

        qDebug() << "PhysicsDebugDrawer: drawContactPoint not yet implemented!";
    }

    void PhysicsDebugDrawer::reportErrorWarning(const char* warningString) {
        qDebug() << warningString;
        //TODO: Show onscreen maybe?
    }

    void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString) {
        //Suppress compiler warnings
        Q_UNUSED(location);
        Q_UNUSED(textString);

        qDebug() << "PhysicsDebugDrawer: draw3dText not yet implemented!";
    }

    void PhysicsDebugDrawer::setDebugMode(int debugMode) {
        this->debugMode = debugMode;
    }

    int PhysicsDebugDrawer::getDebugMode() const {
        return debugMode;
    }

    void PhysicsDebugDrawer::drawAll() {
        if (lineData.size() == 0) return; //Get outta here if there's no data to draw

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lineData.size(), &lineData.at(0), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                24,
                (void*) 0
                );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                24,
                (void*) (sizeof(glm::vec3))
                );

        //Draw the triangle !
        glDrawArrays(GL_LINES, 0, lineData.size() / 2);

        glDisableVertexAttribArray(0);

        glBindVertexArray(0);

        lineData.clear();
    }
}

