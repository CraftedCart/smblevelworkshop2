#ifndef KEYFRAMEEFFECT1_HPP
#define KEYFRAMEEFFECT1_HPP

#include "ws2common_export.h"
#include <glm/glm.hpp>

namespace WS2Common {
    namespace Animation {
        class WS2COMMON_EXPORT KeyframeEffect1 {
            protected:
                unsigned int frame;
                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                char unknownByte1 = 0x0;
                char unknownByte2 = 0x0;
            public:
                KeyframeEffect1(glm::vec3 position, glm::vec3 rotation, char unknownByte1, char unknownByte2);

                glm::vec3 getPosition() const;
                void setPosition(const glm::vec3 &value);

                glm::vec3 getRotation() const;
                void setRotation(const glm::vec3 &value);

                char getUnknownByte1() const;
                void setUnknownByte1(char value);

                char getUnknownByte2() const;
                void setUnknownByte2(char value);

                unsigned int getFrame() const;
                void setFrame(unsigned int value);
        };
    }
}

#endif // KEYFRAMEEFFECT1_HPP
