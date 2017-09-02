/**
 * @file
 * @brief Header for the WS2::Exception::ModelLoadingException class
 */

#ifndef SMBLEVELWORKSHOP2_EXCEPTION_MODELLOADINGEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_EXCEPTION_MODELLOADINGEXCEPTION_HPP

#include "exception/RuntimeException.hpp"

namespace WS2 {
    namespace Exception {

        /**
         * @brief Exception class for when issues arise when loading a 3D model
         */
        class ModelLoadingException : public RuntimeException {
            public:
                ModelLoadingException();
                ModelLoadingException(QString message);

                ModelLoadingException* clone() const override;
                void raise() const override;
        };
    }
}

#endif

