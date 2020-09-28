#include "ws2lz/LZCompressor.hpp"
#include "ws2lz/LZSSDictionary.hpp"

namespace WS2Lz {
    QByteArray LZCompressor::compress(QByteArray data) {
        LZSSDictionary dict;

        // Prime the dictionary
        dict.ptr = dict.WINDOW_SIZE-2*dict.MAX_REF_LEN;

        for (unsigned int i=0; i<dict.MAX_REF_LEN; i++) {
            QByteArray zeroByteList((dict.MAX_REF_LEN-i), '\0');
            QByteArray dataByteList = data.left(i);

            QByteArray byteList(zeroByteList);
            byteList.append(dataByteList);

            dict.add(byteList);
        }

        // Output data
        QByteArray output;
        unsigned int dataSize = data.length();

        unsigned int i = 0;
        while (i < dataSize) {
            QByteArray chunk;
            int flags = 0;

            for (int bit = 0; bit < 8; bit++) {
                if (i >= dataSize) break;

                QByteArray subStr = data.mid(i, qMin(i+dict.MAX_REF_LEN, dataSize));
                offsetLength* ol = dict.find(subStr);

                // Yes, append dictionary reference
                if (ol != nullptr) {
                    chunk.append((char) (ol->first) & 0xFF);
                    chunk.append((char) (((ol->first >> 4) & 0xF0) | (ol->second - dict.MIN_REF_LEN)));

                    for (unsigned int j = 0; j < ol->second; j++) {
                        dict.add(data.mid(i+j, qMin(i+j+dict.MAX_REF_LEN, dataSize)));
                    }

                    i += ol->second;
                }

                // Not in dictionray, append literal
                else {
                    // Append literal value
                    char v = data[i];
                    chunk.append(v);

                    flags |= (1 << bit);

                    // Update dictionary
                    dict.add(data.mid(i, qMin(i+dict.MAX_REF_LEN, dataSize)));
                    i += 1;


                }
            }

            // Chunk complete, add to output
            output.append((char)flags);
            output.append(chunk);
        }

        // Add uncompressed size to the beginning (int, little endian)
        unsigned int uncompressedSize = dataSize;
        output.insert(0, (char) ((uncompressedSize >> 0) & 0xFF));
        output.insert(1, (char) ((uncompressedSize >> 8) & 0xFF));
        output.insert(2, (char) ((uncompressedSize >> 16) & 0xFF));
        output.insert(3, (char) ((uncompressedSize >> 24) & 0xFF));

        // Add compressed size to the beginning (int, little endian - including these new 4 bytes)
        unsigned int compressedSize = output.size() + 4;
        output.insert(0, (char) ((compressedSize >> 0) & 0xFF));
        output.insert(1, (char) ((compressedSize >> 8) & 0xFF));
        output.insert(2, (char) ((compressedSize >> 16) & 0xFF));
        output.insert(3, (char) ((compressedSize >> 24) & 0xFF));

        return output;
    }
}
