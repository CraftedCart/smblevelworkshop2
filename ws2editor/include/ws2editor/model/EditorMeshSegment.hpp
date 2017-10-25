/**
 * @file
 * @brief Header for the EditorMeshSegment class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_EDITORMESHSEGMENT_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_EDITORMESHSEGMENT_HPP

#include "ws2common/model/Vertex.hpp"
#include "ws2common/model/MeshSegment.hpp"
#include "ws2editor/resource/ResourceEditorTexture.hpp"
#include "ws2editor/glplatform.hpp"
#include <QVector>

namespace WS2Editor {
    namespace Model {
        /**
         * @brief Contains model and GL data for one material in a 3D model
         */
        class EditorMeshSegment : public WS2Common::Model::MeshSegment {
            protected:
                GLuint vao;
                GLuint vbo;
                GLuint ebo;

                /**
                 * @brief Generates GL buffers
                 */
                void generateGlBuffers();

            public:
                /**
                 * @brief Create a new Mesh object with the arguments given
                 *
                 * @param vertices
                 * @param indices
                 * @param textures
                 */
                EditorMeshSegment(
                        QVector<WS2Common::Model::Vertex> vertices,
                        QVector<unsigned int> indices,
                        QVector<Resource::ResourceEditorTexture*> textures);

                /**
                 * @brief Generates GL buffers for the mesh
                 */
                void load();

                /**
                 * @brief Deletes the GL buffers for the mesh
                 */
                void unload();

                /**
                 * @return The VAO ID of the mesh
                 */
                const GLuint& getVao() const;

            protected:
                /**
                 * @brief Converts a vector of ResourceEditorTexture*s to a vector of ResourceTexture*s
                 *
                 * @param vec The vector to convert
                 *
                 * @return A vector of ResourceTexture*s
                 */
                QVector<WS2Common::Resource::ResourceTexture*> vectorToBaseTexture(QVector<Resource::ResourceEditorTexture*> vec);
        };
    }
}

#endif

