/**
 * Implementation file for the Result template class/
 */

namespace WS2Common {
    template<typename T>
    Result<T>::Result(EnumStatus status, QString message, T result) :
        status(status),
        message(message),
        result(result) {}

    template<typename T>
    EnumStatus Result<T>::getStatus() const {
        return status;
    }

    template<typename T>
    const QString& Result<T>::getMessage() const {
        return message;
    }
    template<typename T>
    T& Result<T>::getResult() {
        return result;
    }

    template<typename T>
    const T& Result<T>::getResult() const {
        return result;
    }
}

