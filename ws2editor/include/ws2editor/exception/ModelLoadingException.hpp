/**
 * @file
 * @brief Header for the WS2Editor::Exception::ModelLoadingException class
 */

#ifndef SMBLEVELWORKSHOP2_EXCEPTION_MODELLOADINGEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_EXCEPTION_MODELLOADINGEXCEPTION_HPP

#include "ws2editor/exception/RuntimeException.hpp"

namespace WS2Editor {
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

