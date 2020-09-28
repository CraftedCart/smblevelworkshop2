#include "ws2common/animation/KeyframeEffect2.hpp"

namespace WS2Common {
    namespace Animation {
        unsigned int KeyframeEffect2::getFrame() const
        {
            return frame;
        }

        void KeyframeEffect2::setFrame(unsigned int value)
        {
            frame = value;
        }

        KeyframeEffect2::KeyframeEffect2(glm::vec3 position, char unknownByte1, char unknownByte2, char unknownByte3, char unknownByte4)
        {
            this->position = position;
            this->unknownByte1 = unknownByte1;
            this->unknownByte2 = unknownByte2;
            this->unknownByte3 = unknownByte3;
            this->unknownByte4 = unknownByte4;
        }

        glm::vec3 KeyframeEffect2::getPosition() const
        {
            return position;
        }

        void KeyframeEffect2::setPosition(const glm::vec3 &value)
        {
            position = value;
        }

        char KeyframeEffect2::getUnknownByte1() const
        {
            return unknownByte1;
        }

        void KeyframeEffect2::setUnknownByte1(char value)
        {
            unknownByte1 = value;
        }

        char KeyframeEffect2::getUnknownByte2() const
        {
            return unknownByte2;
        }

        void KeyframeEffect2::setUnknownByte2(char value)
        {
            unknownByte2 = value;
        }

        char KeyframeEffect2::getUnknownByte3() const
        {
            return unknownByte3;
        }

        void KeyframeEffect2::setUnknownByte3(char value)
        {
            unknownByte3 = value;
        }

        char KeyframeEffect2::getUnknownByte4() const
        {
            return unknownByte4;
        }

        void KeyframeEffect2::setUnknownByte4(char value)
        {
            unknownByte4 = value;
        }
    }
}
