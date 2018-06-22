/**
 * @file
 * @brief Header for the Result class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_RESULT_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_RESULT_HPP

#include "ws2common_export.h"
#include "ws2common/EnumStatus.hpp"
#include <QString>

namespace WS2Common {
    template<typename T = std::nullptr_t>
    class WS2COMMON_EXPORT Result {
        protected:
            EnumStatus status;
            QString message;
            T result;

        public:
            Result(EnumStatus status = EnumStatus::SUCCESS, QString message = "", T result = nullptr);

            EnumStatus getStatus() const;
            const QString& getMessage() const;
            T& getResult();
            const T& getResult() const;
    };
}

#include "ws2common/Result.ipp"

#endif

