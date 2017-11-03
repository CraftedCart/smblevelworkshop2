namespace WS2Common {
    namespace Model {
        namespace ModelLoader {
            template <class T>
            T getResourceFromFilePath(QString filePath, QVector<Resource::AbstractResource*> &vec) {
                auto res = std::find_if(
                        vec.begin(),
                        vec.end(),
                        [&filePath](const Resource::AbstractResource *object) { return object->getFilePaths().contains(filePath); }
                        );

                if (res != vec.end()) {
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

