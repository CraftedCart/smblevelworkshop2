namespace WS2Common {
    template<typename Base, typename T>
    bool instanceOf(const T *ptr) {
        return dynamic_cast<const Base*>(ptr) != nullptr;
    }
}

