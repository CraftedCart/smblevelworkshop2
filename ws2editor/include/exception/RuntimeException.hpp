/**
 * @file
 * @brief Header for the WS2::Exception::RuntimeException class
 */

#ifndef SMBLEVELWORKSHOP2_EXCEPTION_RUNTIMEEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_EXCEPTION_RUNTIMEEXCEPTION_HPP

#include "exception/Exception.hpp"

namespace WS2 {
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

