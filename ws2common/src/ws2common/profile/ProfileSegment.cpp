#include "ws2common/profile/ProfileSegment.hpp"

namespace WS2Common {
    namespace Profile {
        ProfileSegment::ProfileSegment(QString name) : name(name) {}

        void ProfileSegment::startTimer() {
            timer.start();
        }

        void ProfileSegment::commitTime() {
            committedNanoseconds = timer.nsecsElapsed();
        }

        QString ProfileSegment::getName() {
            return name;
        }

        qint64 ProfileSegment::getCommittedNanoseconds() {
            return committedNanoseconds;
        }
    }
}

