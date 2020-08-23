#ifndef _DEWPSI_INT_SHADER_H
#define _DEWPSI_INT_SHADER_H

#include <Dewpsi_Core.h>

namespace Dewpsi {
    namespace Internal {
        // shader source strings
        struct ShaderProgramSource {
            std::string vertex;
            std::string fragment;
        };

        // shader type
        enum class ShaderType {
            None     = -1,
            Vertex   =  0,
            Fragment =  1
        };
    }
}

// delete a list of shaders
template<typename... Args>
void DeleteShaders(PDuint& shader, Args&&... args)
{
    if (shader)
    {
        glDeleteShader(shader);
        shader = 0;
    }
    DeleteShaders(std::forward<Args>(args)...);
}

template<>
inline void DeleteShaders(PDuint& shader)
{
    if (shader)
    {
        glDeleteShader(shader);
        shader = 0;
    }
}

#endif /* _DEWPSI_INT_SHADER_H */
