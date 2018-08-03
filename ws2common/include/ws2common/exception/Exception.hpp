/**
 * @file
 * @brief Header for the Exception class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_EXCEPTION_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_EXCEPTION_EXCEPTION_HPP

#include "ws2common_export.h"
#include <QException>
#include <QString>

namespace WS2Common {
    namespace Exception {

        /**
         * @brief Base exception class for WS2 exceptions
         */
        class WS2COMMON_EXPORT Exception : public QException {
            protected:
                QString message;

            public:
                Exception();
                Exception(QString message);

                virtual const QString& getMessage() const;

                Exception* clone() const override;
                void raise() const override;
        };
    }
}

#endif

