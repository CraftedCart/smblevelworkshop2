#ifndef LZSSDICTIONARY_HPP
#define LZSSDICTIONARY_HPP

#include "ws2lz_export.h"
#include <QByteArray>
#include <QPair>
#include <QVector>
#include <QHash>

typedef QPair<unsigned int, unsigned int> offsetLength;
typedef QHash<QByteArray, unsigned int> dictionary;
typedef QHash<unsigned int, QByteArray> reverseDictionary;

namespace WS2Lz {
	class WS2LZ_EXPORT LZSSDictionary {
		private:
			QVector<dictionary> dict;
			QVector<reverseDictionary> rdict;

		public:
			static const unsigned int WINDOW_SIZE = 0x1000;
			static const unsigned int WINDOW_MASK = 0x0FFF;
			static const unsigned int MAX_REF_LEN = 18;
			static const unsigned int MIN_REF_LEN = 3;

			unsigned int ptr = 0;

			LZSSDictionary();
			void add(QByteArray bytes);
			offsetLength* find(QByteArray bytes);

	};
}

#endif // LZSSDICTIONARY_HPP
