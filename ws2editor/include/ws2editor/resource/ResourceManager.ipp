namespace WS2Editor {
    namespace Resource {
        namespace ResourceManager {
            template <class T>
            T getResourceFromFilePath(QString filePath) {
                auto res = std::find_if(
                        getResources().begin(),
                        getResources().end(),
                        [&filePath](const WS2Common::Resource::AbstractResource *object) { return object->getFilePaths().contains(filePath); }
                        );

                if (res != getResources().end()) {
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

