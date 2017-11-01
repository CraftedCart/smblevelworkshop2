/**
 * @file
 * @brief Header for the AbstractResource class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_RESOURCE_ABSTRACTRESOURCE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_RESOURCE_ABSTRACTRESOURCE_HPP

#include <QVector>
#include <QString>
#include <QObject>

namespace WS2Common {
    namespace Resource {

        /**
         * @brief Base class for all resources
         */
        class AbstractResource : public QObject {
            Q_OBJECT

            protected:
                /**
                 * @brief A unique identifier for this resource
                 */
                QString id;

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
                virtual ~AbstractResource();

                /**
                 * @brief Setter for the resource ID
                 *
                 * @param id A reference to the ID to set
                 */
                void setId(const QString &id);

                /**
                 * @brief Getter for the resource ID
                 *
                 * @return A reference to the resource ID
                 */
                const QString& getId() const;

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
                 * @brief Getter for the WS2Editor::Resource::AbstractResource::filePaths vector
                 *
                 * @return A reference to the filePaths vector
                 */
                const QVector<QString>& getFilePaths() const;

                /**
                 * @brief Loads the resource. The WS2Editor::Resource::AbstractResource implementation sets loaded to true.
                 */
                virtual void load();

                /**
                 * @brief Unloads the resource. The WS2Editor::Resource::AbstractResource implementation sets loaded to false.
                 */
                virtual void unload();

                /**
                 * @brief Getter for WS2Editor::Resource::AbstractResource::loaded
                 *
                 * @return Whether the resource is loaded
                 */
                bool isLoaded() const;
        };
    }
}

#endif

