/**
 * @file
 * @brief Header for the ProfileSegment class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_PROFILE_PROFILESEGMENT_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_PROFILE_PROFILESEGMENT_HPP

#include <QString>
#include <QElapsedTimer>

namespace WS2Common {
    namespace Profile {
        class ProfileSegment {
            protected:
                QString name;
                QElapsedTimer timer;
                qint64 committedNanoseconds;

            public:
                ProfileSegment(QString name);

                void startTimer();
                void commitTime();

                QString getName();
                qint64 getCommittedNanoseconds();
        };
    }
}

#endif

