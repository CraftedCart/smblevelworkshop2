/**
 * @file
 * @brief Header for the WS2::Resource::AbstractResource class
 */

#ifndef SMBLEVELWORKSHOP2_RESOURCE_ABSTRACTRESOURCE_HPP
#define SMBLEVELWORKSHOP2_RESOURCE_ABSTRACTRESOURCE_HPP

#include <QVector>
#include <QString>

namespace WS2 {
    namespace Resource {

        /**
         * @brief Base class for all resources
         */
        class AbstractResoruce {
            protected:
                /**
                 * @brief The file paths of where the resource originated from
                 */
                QVector<QString> filePaths;

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
                 * @brief Setter for the file paths
                 *
                 * This will clear all existing file paths and add the file path specified
                 *
                 * @param filePath The file path to set
                 */
                void setFilePath(const QString &filePath);

                /**
                 * @brief Adds a file path to the filePaths vector
                 *
                 * @param filePath The file path to add
                 */
                void addFilePath(const QString &filePath);

                /**
                 * @brief Getter for the first file path
                 *
                 * @return A pointer to the file path if it exists, nullptr otherwise
                 */
                const QString* getFirstFilePath() const;

                /**
                 * @brief Getter for the WS2::Resource::AbstractResource::filePaths vector
                 *
                 * @return A reference to the filePaths vector
                 */
                const QVector<QString>& getFilePaths() const;

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

