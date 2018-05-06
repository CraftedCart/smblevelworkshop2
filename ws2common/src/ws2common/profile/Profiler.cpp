#include "ws2common/profile/Profiler.hpp"
#include <QDebug>

namespace WS2Common {
    namespace Profile {
        void Profiler::nextFrame() {
            //Commit the last segment
            if (segments.size() > 0) segments.last()->commitTime();

            qDeleteAll(committedSegments);
            committedSegments.clear();
            committedSegments.append(segments);
            segments.clear();
        }

        void Profiler::nextSegment(QString name) {
            if (segments.size() > 0) segments.last()->commitTime();
            ProfileSegment *segment = new ProfileSegment(name);
            segment->startTimer();
            segments.append(segment);
        }
    }
}

