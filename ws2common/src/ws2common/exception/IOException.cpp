#include "ws2common/exception/IOException.hpp"

namespace WS2Common {
    namespace Exception {
        IOException::IOException() {
            message = "No IOException message given";
        }

        IOException::IOException(QString message) {
            this->message = message;
        }

        IOException* IOException::clone() const {
            return new IOException(*this);
        }

        void IOException::raise() const {
            throw *this;
        }
    }
}

