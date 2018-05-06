/**
 * @file
 * @brief Header for the Profiler class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_PROFILE_PROFILER_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_PROFILE_PROFILER_HPP

#include "ws2common/profile/ProfileSegment.hpp"
#include <QVector>

namespace WS2Common {
    namespace Profile {
        class Profiler {
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
        };
    }
}

#endif

