#ifndef DEWPSI_OPENGLSHADER_H
#define DEWPSI_OPENGLSHADER_H

#include "Dewpsi_Core.h"
#include "Dewpsi_Shader.h"

namespace Dewpsi {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void UploadUniformMat4(const PDstring& name, const float* values) override;

    private:
        PDuint m_ShaderID;
    };
}

#endif /* DEWPSI_OPENGLSHADER_H */
