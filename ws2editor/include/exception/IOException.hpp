/**
 * @file
 * @brief Header for the WS2::Exception::IOException class
 */

#ifndef SMBLEVELWORKSHOP2_EXCEPTION_IOEXCEPTION_HPP
#define SMBLEVELWORKSHOP2_EXCEPTION_IOEXCEPTION_HPP

#include "exception/RuntimeException.hpp"

namespace WS2 {
    namespace Exception {

        /**
         * @brief Exception class for when issues arise when inputting or outputting
         *
         * Example use: When failing to read a file
         */
        class IOException : public RuntimeException {
            public:
                IOException();
                IOException(QString message);

                IOException* clone() const override;
                void raise() const override;
        };
    }
}

#endif

