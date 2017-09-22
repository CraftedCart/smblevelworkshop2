/**
 * @file
 * @brief Includes the GL header, and a few GL helper headers from other libraries
 *
 * Oh Apple, why do you put your GL header in a different place
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_GLPLATFORM_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_GLPLATFORM_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#endif

