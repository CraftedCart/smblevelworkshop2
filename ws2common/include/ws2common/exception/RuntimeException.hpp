/**
 * @file
 * @brief Header for the RuntimeException class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_RUNTIMEEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_RUNTIMEEXCEPTION_HPP

#include "ws2common/exception/Exception.hpp"

namespace WS2Common {
    namespace Exception {

        /**
         * @brief Base exception class for WS2 runtime exceptions
         */
        class RuntimeException : public Exception {
            public:
                RuntimeException();
                RuntimeException(QString message);

                RuntimeException* clone() const override;
                void raise() const override;
        };
    }
}

#endif

