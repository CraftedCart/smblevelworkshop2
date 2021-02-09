#include "ws2common/animation/KeyframeEffect1.hpp"

namespace WS2Common {
    namespace Animation {

        unsigned int KeyframeEffect1::getFrame() const
        {
            return frame;
        }

        void KeyframeEffect1::setFrame(unsigned int value)
        {
            frame = value;
        }

        KeyframeEffect1::KeyframeEffect1(glm::vec3 position, glm::vec3 rotation, char unknownByte1, char unknownByte2)
        {
            this->position = position;
            this->rotation = rotation;
            this->unknownByte1 = unknownByte1;
            this->unknownByte2 = unknownByte2;
        }

        glm::vec3 KeyframeEffect1::getPosition() const
        {
            return position;
        }

        void KeyframeEffect1::setPosition(const glm::vec3 &value)
        {
            position = value;
        }

        glm::vec3 KeyframeEffect1::getRotation() const
        {
            return rotation;
        }

        void KeyframeEffect1::setRotation(const glm::vec3 &value)
        {
            rotation = value;
        }

        char KeyframeEffect1::getUnknownByte1() const
        {
            return unknownByte1;
        }

        void KeyframeEffect1::setUnknownByte1(char value)
        {
            unknownByte1 = value;
        }

        char KeyframeEffect1::getUnknownByte2() const
        {
            return unknownByte2;
        }

        void KeyframeEffect1::setUnknownByte2(char value)
        {
            unknownByte2 = value;
        }

    }
}
