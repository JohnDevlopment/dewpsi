#ifndef DEWPSI_OPENGLSHADER_H
#define DEWPSI_OPENGLSHADER_H

#include "Dewpsi_Core.h"
#include "Dewpsi_Shader.h"
#include "Dewpsi_OpenGL.h"

#include <unordered_map>

namespace Dewpsi {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const PDstring& vertexSrc, const PDstring& fragmentSrc);
        OpenGLShader(const PDstring& file);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void SetInt1(const PDstring& name, int v0) override;
        virtual void SetInt2(const PDstring& name, int v1, int v2) override;
        virtual void SetInt3(const PDstring& name, int v1, int v2, int v3) override;
        virtual void SetInt4(const PDstring& name, int v1, int v2, int v3, int v4) override;
        virtual void SetFloat1(const PDstring& name, float v1) override;
        virtual void SetFloat2(const PDstring& name, float v1, float v2) override;
        virtual void SetFloat3(const PDstring& name, float v1, float v2, float v3) override;
        virtual void SetFloat4(const PDstring& name, float v1, float v2, float v3, float v4) override;
        virtual void SetMat4(const PDstring& name, PDsizei count, const glm::mat4* value,
            bool transpose = false) override;

    private:
        typedef std::unordered_map<GLenum, PDstring> SourceMap;

        PDstring ReadFile(const PDstring& path) const;
        SourceMap PreProcess(const PDstring& source) const;
        GLenum GetShaderType(const PDstring& name) const;
        bool CompileShader(const SourceMap& sources);
        int GetUniformLocation(const PDstring& name);
        bool CheckShader(GLuint shader, const char* desc);
        bool CheckProgram(PDuint program);

        PDuint m_ShaderID = 0;
        std::unordered_map<PDstring, int> m_UniformCache;
        std::vector<char> m_ErrorLog;
    };
}

#endif /* DEWPSI_OPENGLSHADER_H */
