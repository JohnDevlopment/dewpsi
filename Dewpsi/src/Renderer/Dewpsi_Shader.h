#ifndef DEWPSI_SHADER_H
#define DEWPSI_SHADER_H

#include "Dewpsi_Core.h"

namespace Dewpsi {
    class Shader {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void UnBind() const;

    private:
        PDuint m_ShaderID;
    };
}

#endif /* DEWPSI_SHADER_H */
