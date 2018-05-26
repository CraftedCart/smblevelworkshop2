/**
 * @file
 * @brief Header for the Config namespace which stores user configurable variables
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_CONFIG_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_CONFIG_HPP

#include "ws2editor_export.h"
namespace WS2Editor {
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
        extern bool enablePhysicsDebugDrawing;
    }
}

#endif

