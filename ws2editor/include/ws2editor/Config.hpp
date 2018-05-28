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
        WS2EDITOR_EXPORT extern float cameraPosSpeed;
        WS2EDITOR_EXPORT extern float cameraPosSpeedUpMultiplier;
        WS2EDITOR_EXPORT extern float cameraPosSlowDownMultiplier;
        WS2EDITOR_EXPORT extern float cameraRotSpeed;
        WS2EDITOR_EXPORT extern float cameraInertia;
        WS2EDITOR_EXPORT extern float cameraFov;
        WS2EDITOR_EXPORT extern float cameraNear;
        WS2EDITOR_EXPORT extern float cameraFar;
        WS2EDITOR_EXPORT extern bool enablePhysicsDebugDrawing;
    }
}

#endif

