#ifndef DEWPSI_OPENGLSHADER_H
#define DEWPSI_OPENGLSHADER_H

#include "Dewpsi_Core.h"
#include "Dewpsi_Shader.h"

namespace Dewpsi {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();

        void Bind() const;
        void UnBind() const;

    private:
        PDuint m_ShaderID;
    };
}

#endif /* DEWPSI_OPENGLSHADER_H */
