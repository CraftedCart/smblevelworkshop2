/**
 * @file
 * @brief Header for the Profiler class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_PROFILE_PROFILER_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_PROFILE_PROFILER_HPP

#include "ws2common/profile/ProfileSegment.hpp"
#include <QVector>
#include <QObject>

namespace WS2Common {
    namespace Profile {
        class Profiler : public QObject {
            Q_OBJECT

            protected:
                /**
                 * @brief Timing segments for the current frame
                 */
                QVector<ProfileSegment*> segments;

                /**
                 * @brief Timing segments for the previous frame
                 */
                QVector<ProfileSegment*> committedSegments;

            public:
                /**
                 * @brief Commits this frame's segments and begins the next frame
                 */
                void nextFrame();

                /**
                 * @brief Commits the current segment (if any) and begins the next one in the frame
                 *
                 * @param name The segment name
                 */
                void nextSegment(QString name);

                /**
                 * @brief Getter for committedSegments
                 */
                QVector<ProfileSegment*>& getCommittedSegments();

                /**
                 * @brief Get the total time profiled
                 *
                 * @return The sum of all committed times for each committed segment
                 */
                qint64 getTotalCommittedNanoseconds();

            signals:
                /**
                 * @brief Emitted whenever nextFrame has been called, at the end of the function
                 *
                 * @param profiler A reference to this profiler
                 */
                void onProfilerNextFrame(Profiler &profiler);
        };
    }
}

#endif

