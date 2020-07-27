#ifndef DEWPSI_OPENGL_H
#define DEWPSI_OPENGL_H

/**
*   @file   Dewpsi_OpenGL.h
*   @brief  A header for the Dewpsi engine.
*   This header is relevent only for platforms that use SDL. Consequently, when SDL is used
*   OpenGL can be used as well. If 
*
*   @defgroup sdl SDL
*   SDL-specific declarations and functions
*   @ingroup platforms
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Application.h>
#include <glad/glad.h>


namespace Dewpsi {
    /// @addtogroup sdl
    /// @{
    
    /** Initialize OpenGL shaders.
    *   @param  shaderFile  A string containing the path to a shader file
    *   @return             True if successful or false on failure
    */
    PD_CALL bool OpenGL_InitShaders(const char* shaderFile);
    
    /** Frees all memory associated with the shaders created through OpenGL_InitShaders().
    *   Should be called when shutting down the program provided OpenGL_InitShaders() was called.
    */
    PD_CALL void OpenGL_DeInitShaders();
    
    /// Returns the shader program.
    PD_CALL PDuint OpenGL_GetShader();
    
    /// @}
}

#endif /* DEWPSI_OPENGL_H */
