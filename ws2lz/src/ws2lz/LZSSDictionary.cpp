#include "ws2lz/LZSSDictionary.hpp"

namespace WS2Lz {
    LZSSDictionary::LZSSDictionary() {
        // For each reference length there is one dictionary mapping substrings to dictionary offsets
        for (unsigned int i =0; i < MAX_REF_LEN + 1; i++) {
            dict.append(*(new dictionary));
        }
        // For each reference length there is also a reverse dictionary mapping dictionary offsets to substrings
        // This makes removing dictionary entries much more efficient
        for (unsigned int i =0; i < MAX_REF_LEN + 1; i++) {
            rdict.append(*(new reverseDictionary));
        }
    }

    // Add all initial parts of a byte array to the dictionary
    void LZSSDictionary::add(QByteArray bytes) {
        unsigned int maxLength = (MAX_REF_LEN > bytes.length()) ? bytes.length() : MAX_REF_LEN;
        unsigned int offset = ptr;

        // Generate all substrings
        for (unsigned int length = MIN_REF_LEN; length < maxLength + 1; length++) {
            QByteArray subStr = bytes.left(length);
            dictionary& current_dict = dict[length];
            reverseDictionary& current_rdict = rdict[length];

            if (current_dict.find(subStr) != current_dict.end()) {
                unsigned int prevOffset = current_dict[subStr];
                if (current_rdict.find(prevOffset) != current_rdict.end()) {
                    current_rdict.remove(prevOffset);
                }
            }

            if (current_rdict.find(offset) != current_rdict.end()) {
                current_dict.remove(current_rdict[offset]);
            }

            current_dict.insert(subStr, offset);
            current_rdict.insert(offset, subStr);
        }

        ptr = (ptr + 1) & WINDOW_MASK;
    }

    offsetLength* LZSSDictionary::find(QByteArray bytes) {
        unsigned int maxLength = (MAX_REF_LEN > bytes.length()) ? bytes.length() : MAX_REF_LEN;

        for (unsigned int length = maxLength; length > MIN_REF_LEN - 1; length--) {
            QByteArray subStr = bytes.left(length);
            dictionary& current_dict = dict[length];

            if (current_dict.find(subStr) != current_dict.end()) {
                unsigned int offset = current_dict[subStr];
                if (offset != ptr) {
                    return new offsetLength(offset, length);
                }
            }
        }
        return NULL;
    }
}
