#ifndef DEWPSI_SHADER_H
#define DEWPSI_SHADER_H

/** @file    Dewpsi_Shader.h
*	@brief   @doxfb
*	@ingroup renderer
*/

#include "Dewpsi_Core.h"

namespace Dewpsi {
    /** Defines an interface to shaders for the native rendering API.
    *	@ingroup renderer
    */
    class Shader {
    public:
        Shader() {  }
        virtual ~Shader() {  }

        /// Binds the shader.
        virtual void Bind() const = 0;

        /// Unbinds the shader.
        virtual void UnBind() const = 0;

        /** Creates a shader program and returns a pointer to it.
        *	The exact kind of shader that is created, and what language the
        *	source code is in, depends on the currently selected API.
        *	@param    vertSrc A string containing the source code of the vertex shader
        *	@param    fragSrc A string containing the source code of the fragment shader
        *	@return           A pointer to the platform-dependent shader object
        *	@throw    DewpsiError     Thrown if an unsupported API is detected.
        */
        static Shader* Create(const std::string& vertSrc, const std::string& fragSrc);
    };
}

#endif /* DEWPSI_SHADER_H */
