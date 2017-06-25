/**
 * @file
 * @brief Header for the WS2::Resource::AbstractResource class
 */

#ifndef SMBLEVELWORKSHOP2_RESOURCE_ABSTRACTRESOURCE_HPP
#define SMBLEVELWORKSHOP2_RESOURCE_ABSTRACTRESOURCE_HPP

#include <QString>

namespace WS2 {
    namespace Resource {

        /**
         * @brief Base class for all resources
         */
        class AbstractResoruce {
            protected:
                /**
                 * @brief The file path of where the resource originated from
                 */
                QString filePath;

                /**
                 * @brief Stores whether the resource is loaded or not
                 */
                bool loaded = false;

            public:
                /**
                 * @brief Calls `unload()` if `isLoaded() == true`
                 */
                virtual ~AbstractResoruce();

                /**
                 * @brief Setter for the file path
                 *
                 * @param filePath The file path to set
                 */
                void setFilePath(QString &filePath);

                /**
                 * @brief Getter for the file path
                 *
                 * @return A reference to the file path
                 */
                const QString& getFilePath() const;

                /**
                 * @brief Loads the resource. The WS2::Resource::AbstractResource implementation sets loaded to true.
                 */
                virtual void load();

                /**
                 * @brief Unloads the resource. The WS2::Resource::AbstractResource implementation sets loaded to false.
                 */
                virtual void unload();

                /**
                 * @brief Getter for WS2::Resource::AbstractResource::loaded
                 *
                 * @return Whether the resource is loaded
                 */
                bool isLoaded();
        };
    }
}

#endif

