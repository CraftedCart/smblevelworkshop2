/**
 * @file
 * @brief Header for the WS2Editor::Model::Texture struct
 */

#ifndef SMBLEVELWORKSHOP2_MODEL_TEXTURE_HPP
#define SMBLEVELWORKSHOP2_MODEL_TEXTURE_HPP

#include "glplatform.hpp"
#include "resource/AbstractResource.hpp"
#include <QOpenGLTexture>
#include <QFile>

namespace WS2Editor {
    namespace Resource {
        class ResourceTexture : public AbstractResource {
            Q_OBJECT

            protected:
                /**
                 * @brief The GL texture object
                 */
                QOpenGLTexture *texture;

            public:
                /**
                 * @brief Attempts to load the GL texture
                 */
                void load() override;

                /**
                 * @brief Unloads the OpenGL texture
                 */
                void unload() override;

                /**
                 * @brief Getter for texture
                 *
                 * @return A pointer to the texture object if the texture is loaded, nullptr otherwise
                 */
                QOpenGLTexture* getTexture();

                /**
                 * @brief Setter for texture
                 * @note This object should be the owner of the pointer. Don't delete the pointer!
                 * @note This will mark the resource as loaded if not nullptr, or unloaded otherwise
                 *
                 * @param tex The texture pointer to set
                 */
                void setTexture(QOpenGLTexture *tex);
        };
    }
}

#endif

