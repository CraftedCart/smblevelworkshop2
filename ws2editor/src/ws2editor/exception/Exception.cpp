#include "ws2editor/exception/Exception.hpp"

namespace WS2Editor {
    namespace Exception {
        Exception::Exception() {
            message = "No Exception message given";
        }

        Exception::Exception(QString message) {
            this->message = message;
        }

        const QString& Exception::getMessage() const {
            return message;
        }

        Exception* Exception::clone() const {
            return new Exception(*this);
        }

        void Exception::raise() const {
            throw *this;
        }
    }
}

