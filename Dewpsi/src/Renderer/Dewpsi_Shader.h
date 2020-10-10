#ifndef DEWPSI_SHADER_H
#define DEWPSI_SHADER_H

/** @file Dewpsi_Shader.h
*	@ref core_renderer
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_RendererAPI.h>
#include <Dewpsi_Memory.h>
#include <glm/glm.hpp>

namespace Dewpsi {
    /** Defines an interface to shaders for the native rendering API.
    *	@ingroup core_renderer
    *
    *   # Functions #
    *
    *   ## Set ##
    *   Set - Specify the value of a shader-specific uniform variable
    *   @code
    *       void SetInt1(const PDstring& name, PDint v0);
    *       void SetInt2(const PDstring& name, PDint v1, PDint v2);
    *       void SetInt3(const PDstring& name, PDint v1, PDint v2, PDint v3);
    *       void SetInt4(const PDstring& name, PDint v1, PDint v2, PDint v3, PDint v4);
    *       void SetUInt1(const PDstring& name, PDuint v0);
    *       void SetUInt2(const PDstring& name, PDuint v1, PDuint v2);
    *       void SetUInt3(const PDstring& name, PDuint v1, PDuint v2, PDuint v3);
    *       void SetUInt4(const PDstring& name, PDuint v1, PDuint v2, PDuint v3, PDuint v4);
    *       void SetFloat1(const PDstring& name, PDfloat v0);
    *       void SetFloat2(const PDstring& name, PDfloat v0, PDfloat v1);
    *       void SetFloat3(const PDstring& name, PDfloat v0, PDfloat v1, PDfloat v2);
    *       void SetFloat4(const PDstring& name, PDfloat v0, PDfloat v1, PDfloat v2, PDfloat v3);
    *       void SetMat4(const PDstring& name, PDsizei count, const glm::mat4* value, bool transpose);
    *   @endcode
    *   @par Parameters
    *   - @a name
    *       - Specifies the name of the variable as it's called in the shader.
    *   - @a v0,v1,v2,v3
    *       - For scalar variables, sets the new values for the specified variable.
    *   - @a value
    *       - For matrices and vectors, accepts a pointer to @a count elements that are used to
    *         update the variable specified by @a name.
    *   - @a count
    *       - For the vector (__SetVec\*__) commands, specifies the number of elements to be
    *         modified. Put 1 if the variable is not an array, or 2 or more if it is an array.
    *       - For matricx (__SetMat\*__) commands, specifies the number of matrices to be modified.
    *         Put 1 if the variable is not an array, or 2 or more if it is an array.
    *   - @a transpose
    *       - This is for matrix commands. If true, the values in the matrix/matrices are transposed
    *         as they are loaded into the variable. _If omitted, this defaults to_ @c false.
    *
    *   @par Description
    *   @doxfunc{Shader::Set} modifies the value of a uniform variable or a uniform variable array.
    *   The location of the uniform is determined from the name of the variable, which is specified
    *   by @a name.
    *
    *   @par
    *   The commands <b><code>Set{Int|Float}{1|2|3|4}</code></b> are used to modify the value of the
    *   variable specified with @a name. The number in the command name should specify how many
    *   components the variable has. For example, a single float can be modified with @doxfunc{SetFloat1}
    *   and a vector of two floats can be modified with @doxfunc{SetFloat2}. *Table 1* below shows
    *   the functions and their corresponding type.
    *
    *   @par Table 1
    *   Function    | Type
    *   ----------- | ----
    *   `SetInt1`   | Single @c PDint scalar
    *   `SetInt2`   | Two-component @c PDint vector
    *   `SetInt3`   | Three-component @c PDint vector
    *   `SetInt4`   | Four-component @c PDint vector
    *   `SetUInt1`  | Single @c PDuint scalar
    *   `SetUInt2`  | Two-component @c PDuint vector
    *   `SetUInt3`  | Three-component @c PDuint vector
    *   `SetUInt4`  | Four-component @c PDuint vector
    *   `SetFloat1` | Single @c PDfloat scalar
    *   `SetFloat2` | Two-component @c PDfloat vector
    *   `SetFloat3` | Three-component @c PDfloat vector
    *   `SetFloat4` | Four-component @c PDfloat vector
    *   `SetMat4`   | 4x4 matrix (@c float)
    */
    class Shader {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        /// Binds the shader.
        virtual void Bind() const = 0;

        /// Unbinds the shader.
        virtual void UnBind() const = 0;

        // These are documented up above in the class doc.
        virtual void SetInt1(const PDstring& name, PDint v0) = 0;
        virtual void SetInt2(const PDstring& name, PDint v1, PDint v2) = 0;
        virtual void SetInt3(const PDstring& name, PDint v1, PDint v2, PDint v3) = 0;
        virtual void SetInt4(const PDstring& name, PDint v1, PDint v2, PDint v3, PDint v4) = 0;

        virtual void SetUInt1(const PDstring& name, PDuint v0) = 0;
        virtual void SetUInt2(const PDstring& name, PDuint v1, PDuint v2) = 0;
        virtual void SetUInt3(const PDstring& name, PDuint v1, PDuint v2, PDuint v3) = 0;
        virtual void SetUInt4(const PDstring& name, PDuint v1, PDuint v2, PDuint v3, PDuint v4) = 0;

        virtual void SetFloat1(const PDstring& name, float v1) = 0;
        virtual void SetFloat2(const PDstring& name, float v1, float v2) = 0;
        virtual void SetFloat3(const PDstring& name, float v1, float v2, float v3) = 0;
        virtual void SetFloat4(const PDstring& name, float v1, float v2, float v3, float v4) = 0;
        virtual void SetMat4(const PDstring& name, PDsizei count, const glm::mat4* value,
            bool transpose = false) = 0;

        /** Creates a shader program and returns a pointer to it.
        *	The exact kind of shader that is created, and what language the
        *	source code is in, depends on the currently selected API.
        *	@param    vertSrc A string containing the source code of the vertex shader
        *	@param    fragSrc A string containing the source code of the fragment shader
        *	@return           A pointer to the platform-dependent shader object
        *	@throw    DewpsiError     Thrown if an unsupported API is detected.
        */
        static Ref<Shader> Create(const PDstring& vertSrc, const PDstring& fragSrc);

        /** Creates a shader program and returns a pointer to it.
        *	The exact kind of shader that is created, and what language the
        *	source code is in, depends on the currently selected API.
        *	@param    file    A string containing the path to a file with the source
        *                     code for the vertex and fragment shaders.
        *	@return           A pointer to the platform-dependent shader object
        /
        *	@throw DewpsiError Thrown if an unsupported API is detected.
        */
        static Ref<Shader> Create(const PDstring& file);
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
}

#endif /* DEWPSI_SHADER_H */
