#ifndef DEWPSI_OPENGLSHADER_H
#define DEWPSI_OPENGLSHADER_H

#include "Dewpsi_Core.h"
#include "Dewpsi_Shader.h"

#include <unordered_map>

namespace Dewpsi {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const PDstring& vertexSrc, const PDstring& fragmentSrc);
        OpenGLShader(const PDstring& file);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        void UploadUniformInt1(const PDstring& name, int value);
        void UploadUniformFloat3(const PDstring& name, float val1, float val2, float val3);
        void UploadUniformFloat4(const PDstring& name, float val1, float val2, float val3, float val4);
        void UploadUniformMat4(const PDstring& name, const float* values);

    private:
        int GetUniformLocation(const PDstring& name);

        PDuint m_ShaderID;
        std::unordered_map<PDstring, int> m_UniformCache;
    };
}

#endif /* DEWPSI_OPENGLSHADER_H */
