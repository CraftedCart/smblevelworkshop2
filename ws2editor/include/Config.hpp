/**
 * @file
 * @brief Header for the Config namespace which stores user configurable variables
 */

#ifndef SMBLEVELWORKSHOP2_CONFIG_HPP
#define SMBLEVELWORKSHOP2_CONFIG_HPP

namespace WS2 {
    /**
     * @brief Stores user configurable variables
     */
    namespace Config {
        extern float cameraPosSpeed;
        extern float cameraPosSpeedUpMultiplier;
        extern float cameraPosSlowDownMultiplier;
        extern float cameraRotSpeed;
        extern float cameraInertia;
        extern float cameraFov;
        extern float cameraNear;
        extern float cameraFar;
    }
}

#endif

