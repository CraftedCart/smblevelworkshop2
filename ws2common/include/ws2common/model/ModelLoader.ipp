namespace WS2Common {
    namespace Model {
        namespace ModelLoader {
            template <class T>
            T getResourceFromFilePath(
                    QString filePath,
                    QVector<Resource::AbstractResource*> &vec,
                    QMutex *resourcesMutex
                    ) {
                if (resourcesMutex != nullptr) resourcesMutex->lock();
                auto res = std::find_if(
                        vec.begin(),
                        vec.end(),
                        [&filePath](const Resource::AbstractResource *object) { return object->getFilePaths().contains(filePath); }
                        );

                if (res != vec.end()) {
                    //Resource found
                    if (resourcesMutex != nullptr) resourcesMutex->unlock();
                    return static_cast<T>(*res);
                } else {
                    //Resource not found
                    if (resourcesMutex != nullptr) resourcesMutex->unlock();
                    return nullptr;
                }
            }
        }
    }
}

