/**
 * @file
 * @brief Header for the ResourceEditorTexture class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCEEDITORTEXTURE_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCEEDITORTEXTURE_HPP

#include "ws2editor/glplatform.hpp"
#include "ws2common/resource/ResourceTexture.hpp"
#include <QOpenGLTexture>
#include <QFile>

namespace WS2Editor {
    namespace Resource {
        class ResourceEditorTexture : public WS2Common::Resource::ResourceTexture {
            Q_OBJECT

            protected:
                /**
                 * @brief The GL texture object
                 */
                QOpenGLTexture *glTexture;

            public:
                /**
                 * @brief Constructs and empty texture
                 */
                ResourceEditorTexture();

                /**
                 * @brief Copy constructor, adding editor data to a ResourceTexture
                 *
                 * @param origin The ResourceTexture to copy
                 */
                ResourceEditorTexture(const WS2Common::Resource::ResourceTexture &origin);

                /**
                 * @brief Attempts to load the GL texture
                 */
                void load() override;

                /**
                 * @brief Unloads the OpenGL texture
                 */
                void unload() override;

                /**
                 * @brief Getter for glTexture
                 *
                 * @return A pointer to the texture object if the texture is loaded, nullptr otherwise
                 */
                QOpenGLTexture* getGlTexture();

                /**
                 * @brief Setter for glTexture
                 * @note This object should be the owner of the pointer. Don't delete the pointer!
                 * @note This will mark the resource as loaded if not nullptr, or unloaded otherwise
                 *
                 * @param tex The texture pointer to set
                 */
                void setGlTexture(QOpenGLTexture *tex);
        };
    }
}

#endif

