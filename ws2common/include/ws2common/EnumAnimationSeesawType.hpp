/**
 * @file
 * @brief Header for the EnumAnimationSeesawType enum
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_ENUMANIMATIONSEESAWTYPE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_ENUMANIMATIONSEESAWTYPE_HPP

namespace WS2Common {

    /**
     * @brief An enum to differentiate between animated collision headers and seesaw collision headers types
     *
     * Animated means that it can have an animation, but not necessarily
     */
    enum EnumAnimationSeesawType {
        ANIMATION,
        SEESAW
    };
}

#endif

