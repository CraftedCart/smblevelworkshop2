/**
 * @file
 * @brief Header for the RuntimeException class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_RUNTIMEEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_RUNTIMEEXCEPTION_HPP

#include "ws2common_export.h"
#include "ws2common/exception/Exception.hpp"

namespace WS2Common {
    namespace Exception {

        /**
         * @brief Base exception class WS2COMMON_EXPORT for WS2 runtime exceptions
         */
        class WS2COMMON_EXPORT RuntimeException : public Exception {
            public:
                RuntimeException();
                RuntimeException(QString message);

                RuntimeException* clone() const override;
                void raise() const override;
        };
    }
}

#endif

