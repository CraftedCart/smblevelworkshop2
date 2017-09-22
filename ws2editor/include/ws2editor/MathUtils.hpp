#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MATHUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MATHUTILS_HPP

#include <glm/glm.hpp>
#include <QVector3D>
#include <QVector2D>
#include <assimp/matrix4x4.h>
#include <btBulletDynamicsCommon.h>

namespace WS2Editor {

    /**
     * @brief Various functions to convert between types and other misc math related functions
     */
    namespace MathUtils {
        glm::vec3 toGlmVec3(const QVector3D &vec);
        glm::vec3 toGlmVec3(const QVector2D &vec);
        glm::vec3 toGlmVec3(const btVector3 &vec);
        glm::vec2 toGlmVec2(const QVector2D &vec);
        glm::mat4 toGlmMat4(const aiMatrix4x4 &mat);
        btVector3 toBtVector3(const glm::vec3 &vec);
        QPoint toQPoint(const glm::vec2 &vec);
        int randInt(const int a, const int b);
    }
}

#endif

