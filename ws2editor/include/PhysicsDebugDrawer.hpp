/**
 * @file
 * @brief Header for the PhysicsDebugDrawer class
 */

#ifndef SMBLEVELWORKSHOP2_PHYSICSDEBUGDRAWER_HPP
#define SMBLEVELWORKSHOP2_PHYSICSDEBUGDRAWER_HPP

#include "glplatform.hpp"
#include "MathUtils.hpp"
#include <btBulletDynamicsCommon.h>
#include <QVector>

namespace WS2Editor {
    class PhysicsDebugDrawer : public btIDebugDraw {
        protected:
            int debugMode;

            GLuint vao;
            GLuint vbo;
            QVector<glm::vec3> lineData; //From Pos, From Color, To Pos, To Color, From Pos, Fron Color, To Pos...

        public:
            PhysicsDebugDrawer();
            ~PhysicsDebugDrawer();

            void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
            void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
            void drawSphere(const btVector3& p, btScalar radius, const btVector3& color);
            void drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);
            void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
            void reportErrorWarning(const char* warningString);
            void draw3dText(const btVector3& location, const char* textString);
            void setDebugMode(int debugMode);
            int getDebugMode() const;

            /**
             * @brief Draws all buffered data and clears the buffers
             *
             * @note This assumes an appropriate shader is already bound and its uniforms specified
             */
            void drawAll();
    };
}

#endif

