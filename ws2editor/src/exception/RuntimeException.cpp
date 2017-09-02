#include "exception/RuntimeException.hpp"

namespace WS2 {
    namespace Exception {
        RuntimeException::RuntimeException() {
            message = "No RuntimeException message given";
        }

        RuntimeException::RuntimeException(QString message) {
            this->message = message;
        }

        RuntimeException* RuntimeException::clone() const {
            return new RuntimeException(*this);
        }

        void RuntimeException::raise() const {
            throw *this;
        }
    }
}

