/**
 * @file
 * @brief Header for the WS2::Exception::Exception class
 */

#ifndef SMBLEVELWORKSHOP2_EXCEPTION_EXCEPTION_HPP
#define SMBLEVELWORKSHOP2_EXCEPTION_EXCEPTION_HPP

#include <QException>
#include <QString>

namespace WS2 {
    namespace Exception {

        /**
         * @brief Base exception class for WS2 exceptions
         */
        class Exception : public QException {
            protected:
                QString message;

            public:
                Exception();
                Exception(QString message);

                const QString& getMessage() const;

                Exception* clone() const override;
                void raise() const override;
        };
    }
}

#endif

