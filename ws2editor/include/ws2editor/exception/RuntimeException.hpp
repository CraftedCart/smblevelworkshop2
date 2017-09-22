/**
 * @file
 * @brief Header for the WS2Editor::Exception::RuntimeException class
 */

#ifndef SMBLEVELWORKSHOP2_EXCEPTION_RUNTIMEEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_EXCEPTION_RUNTIMEEXCEPTION_HPP

#include "ws2editor/exception/Exception.hpp"

namespace WS2Editor {
    namespace Exception {

        /**
         * @brief Base exception class for WS2 runtime exceptions
         */
        class RuntimeException : public Exception {
            public:
                RuntimeException();
                RuntimeException(QString message);

                const QString& getMessage() const;

                RuntimeException* clone() const override;
                void raise() const override;
        };
    }
}

#endif

