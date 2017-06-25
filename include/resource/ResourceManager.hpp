/**
 * @file
 * @brief Header for the WS2::Resource::ResourceManager namespace
 */

#ifndef SMBLEVELWORKSHOP2_RESOURCEMANAGER_HPP
#define SMBLEVELWORKSHOP2_RESOURCEMANAGER_HPP

#include "resource/AbstractResource.hpp"
#include <QVector>

namespace WS2 {
    namespace Resource {

        /**
         * @brief Stores and manages various runtime resources
         */
        namespace ResourceManager {
            /**
             * @brief Vector of all resources<br>
             *        It is preferable to use the functions to access the resources rather than by directly accessing
             *        them from this vector.
             */
            extern QVector<AbstractResoruce*> resources;

            /**
             * @brief Adds a resource to the WS2::Resource::ResourceManager::resources vector
             * @note WS2::Resource::ResourceManager should be the owner of all resources, therefore **do not** delete
             *       the pointer after adding the resource.
             *
             * @param res The resource to add
             */
            void addResource(AbstractResoruce *res);

            /**
             * @brief Gets a resource originating from the given file path
             *
             * The resource is statically casted to the type T specified
             *
             * @tparam T The type to cast the resource to. This should be a pointer type.
             * @param filePath The file path of the resource
             *
             * @return A pointer to the resource if it already exists, or nullptr otherwise
             */
            template <class T> T getResourceFromFilePath(QString filePath) {
                auto res = std::find_if(
                        resources.begin(),
                        resources.end(),
                        [&filePath](const AbstractResoruce *object) { return object->getFilePath() == filePath; }
                        );

                if (res != resources.end()) {
                    //Resource found
                    return static_cast<T>(*res);
                } else {
                    //Resource not found
                    return nullptr;
                }
            }
        }
    }
}

#endif

