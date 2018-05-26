/**
 * @file
 * @brief Header for the ModelLoadingException class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_MODELLOADINGEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_MODELLOADINGEXCEPTION_HPP

#include "ws2common_export.h"
#include "ws2common/exception/RuntimeException.hpp"

namespace WS2Common {
    namespace Exception {

        /**
         * @brief Exception class WS2COMMON_EXPORT for when issues arise when loading a 3D model
         */
        class WS2COMMON_EXPORT ModelLoadingException : public RuntimeException {
            public:
                ModelLoadingException();
                ModelLoadingException(QString message);

                ModelLoadingException* clone() const override;
                void raise() const override;
        };
    }
}

#endif

