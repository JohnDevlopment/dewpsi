#ifndef DEWPSI_SHADER_H
#define DEWPSI_SHADER_H

/** @file    Dewpsi_Shader.h
*	@brief   @doxfb
*	@ingroup renderer
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_RendererAPI.h>

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

    /// A chunk with the source code for shaders.
    struct ShaderSourceChunk {
        PDstring vertexSource;   ///< Vertex shader source
        PDstring fragmentSource; ///< Fragment shader source

        /// Default constructor.
        ShaderSourceChunk() = default;

        /// Construct a source chunk with the vertex and fragment shaders.
        ShaderSourceChunk(const PDstring& vertex, const PDstring& fragment)
            : vertexSource(vertex), fragmentSource(fragment) {}

        /// Copy constructor
        ShaderSourceChunk(const ShaderSourceChunk& src)
            : vertexSource(src.vertexSource),
              fragmentSource(src.fragmentSource) {}

        /// Move constructor.
        ShaderSourceChunk(ShaderSourceChunk&& src)
            : vertexSource(PD_MOVE(src.vertexSource)),
              fragmentSource(PD_MOVE(src.fragmentSource)) {}

        /// Copy assignment.
        ShaderSourceChunk& operator=(const ShaderSourceChunk& rhs)
        {
            vertexSource = rhs.vertexSource;
            fragmentSource = rhs.fragmentSource;
            return *this;
        }

        /// Move assignment.
        ShaderSourceChunk& operator=(ShaderSourceChunk&& rhs)
        {
            vertexSource = PD_MOVE(rhs.vertexSource);
            fragmentSource = PD_MOVE(rhs.fragmentSource);
            return *this;
        }
    };

    /// Returns the default shader.
    /// @ingroup renderer
    PD_CALL ShaderSourceChunk GetDefaultShader(RendererAPI::API api);
}

#endif /* DEWPSI_SHADER_H */
