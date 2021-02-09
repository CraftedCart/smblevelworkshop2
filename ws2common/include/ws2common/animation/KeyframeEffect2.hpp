#ifndef KEYFRAMEEFFECT2_HPP
#define KEYFRAMEEFFECT2_HPP

#include "ws2common_export.h"
#include <glm/glm.hpp>

namespace WS2Common {
    namespace Animation {
        class WS2COMMON_EXPORT KeyframeEffect2 {
            protected:
                unsigned int frame;
                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
                char unknownByte1 = 0x0;
                char unknownByte2 = 0x0;
                char unknownByte3 = 0x0;
                char unknownByte4 = 0x0;

            public:
                KeyframeEffect2(glm::vec3 position, char unknownByte1, char unknownByte2, char unknownByte3, char unknownByte4);

                glm::vec3 getPosition() const;
                void setPosition(const glm::vec3 &value);

                char getUnknownByte1() const;
                void setUnknownByte1(char value);

                char getUnknownByte2() const;
                void setUnknownByte2(char value);

                char getUnknownByte3() const;
                void setUnknownByte3(char value);

                char getUnknownByte4() const;
                void setUnknownByte4(char value);

                unsigned int getFrame() const;
                void setFrame(unsigned int value);
        };
    }
}

#endif // KEYFRAMEEFFECT2_HPP
