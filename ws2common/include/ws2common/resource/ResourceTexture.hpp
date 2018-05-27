/**
 * @file
 * @brief Header for the ResourceTexture class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCETEXTURE_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCETEXTURE_HPP

#include "ws2common_export.h"
#include "ws2common/resource/AbstractResource.hpp"
#include <QImage>

namespace WS2Common {
    namespace Resource {
        class WS2COMMON_EXPORT ResourceTexture : public WS2Common::Resource::AbstractResource {
            Q_OBJECT

            protected:
                /**
                 * @brief The texture object - Will be nullptr if not loaded
                 */
                QImage *texture;

            public:
                /**
                 * @brief Loads the texture into the QImage *texture
                 */
                void load() override;

                /**
                 * @brief Deletes texture
                 */
                void unload() override;

                /**
                 * @brief Getter for texture
                 *
                 * @return A pointer to texture, or nullptr if not loaded
                 */
                QImage* getTexture();

                /**
                 * @brief Const getter for texture
                 *
                 * @return A pointer to texture, or nullptr if not loaded
                 */
                const QImage* getTexture() const;
        };
    }
}

#endif

