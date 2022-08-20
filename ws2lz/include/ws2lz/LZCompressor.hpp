#ifndef LZCOMPRESSOR_HPP
#define LZCOMPRESSOR_HPP

#include <QByteArray>
#include "ws2lz_export.h"

namespace WS2Lz {
    class WS2LZ_EXPORT LZCompressor {
        public:
        QByteArray compress(QByteArray data);
    };
}

#endif // LZCOMPRESSOR_HPP
