#define GLCall_IMPL
#include "Dewpsi_Shader.h"
#include "Dewpsi_OpenGLShader.h"
#include "Dewpsi_Memory.h"
#include "Dewpsi_String.h"
#include "_Dewpsi_Int_Shader.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_Array.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>

using Dewpsi::Internal::ShaderProgramSource;
using Dewpsi::Internal::ShaderType;

namespace Dewpsi {

OpenGLShader::OpenGLShader(const PDstring& vertexSrc, const PDstring& fragmentSrc)
{
    SourceMap sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    bool success = CompileShader(sources);
    PD_CORE_ASSERT(success, "Unable to compile a shader: {}", GetError());
    /*if (! success)
    {
        PDstring temp = PDstring("OpenGL shader has failed to compile") + GetError();
        throw DewpsiError(temp);
    }*/
}

OpenGLShader::OpenGLShader(const PDstring& file)
{
    SourceMap sources = PreProcess(ReadFile(file));
    CompileShader(sources);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_ShaderID);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_ShaderID);
}

void OpenGLShader::UnBind() const
{
    glUseProgram(0);
}

void OpenGLShader::SetInt1(const PDstring& name, int v0)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform1i(iLocation, v0));
}

void OpenGLShader::SetInt2(const PDstring& name, int v0, int v1)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform2i(iLocation, v0, v1));
}

void OpenGLShader::SetInt3(const PDstring& name, int v0, int v1, int v2)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform3i(iLocation, v0, v1, v2));
}

void OpenGLShader::SetInt4(const PDstring& name, int v0, int v1, int v2, int v3)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform4i(iLocation, v0, v1, v2, v3));
}

void OpenGLShader::SetUInt1(const PDstring& name, PDuint v0)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform1ui(iLocation, v0));
}

void OpenGLShader::SetUInt2(const PDstring& name, PDuint v0, PDuint v1)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform2ui(iLocation, v0, v1));
}

void OpenGLShader::SetUInt3(const PDstring& name, PDuint v0, PDuint v1, PDuint v2)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform3ui(iLocation, v0, v1, v2));
}

void OpenGLShader::SetUInt4(const PDstring& name, PDuint v0, PDuint v1, PDuint v2, PDuint v3)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform4ui(iLocation, v0, v1, v2, v3));
}

void OpenGLShader::SetFloat1(const PDstring& name, float v1)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform1f(iLocation, v1));
}

void OpenGLShader::SetFloat2(const PDstring& name, float v1, float v2)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform2f(iLocation, v1, v2));
}

void OpenGLShader::SetFloat3(const PDstring& name, float v1, float v2, float v3)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform3f(iLocation, v1, v2, v3));
}

void OpenGLShader::SetFloat4(const PDstring& name, float v1, float v2, float v3, float v4)
{
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    GLCall(glUniform4f(iLocation, v1, v2, v3, v4));
}

void OpenGLShader::SetMat4(const PDstring& name, PDsizei count, const glm::mat4* value,
    bool transpose)
{
    PD_CORE_ASSERT(value, "NULL 'values' parameter");
    if (! value)
    {
        PD_CORE_WARN("NULL 'value' parameter in Shader::SetMat4");
        PD_BADPARAM("value");
        return;
    }
    GLCall(glUseProgram(m_ShaderID));

    GLint iLocation = GetUniformLocation(name);
    PD_CORE_ASSERT(iLocation >= 0, "Could not find uniform '{0}'", name);
    glUniformMatrix4fv(iLocation, count, (transpose ? GL_TRUE : GL_FALSE), glm::value_ptr(*value));
}

int OpenGLShader::GetUniformLocation(const PDstring& name)
{
    PD_CORE_ASSERT(m_ShaderID, "Shader program not initialized!");
    glUseProgram(m_ShaderID);

    int result = -1;
    auto found = m_UniformCache.find(name);

    if (found != m_UniformCache.end())
        return found->second;

    int iLocation = glGetUniformLocation(m_ShaderID, name.c_str());
    m_UniformCache[name] = iLocation;

    return iLocation;
}

PDstring OpenGLShader::ReadFile(const PDstring& path) const
{
    PDstring result;
    std::ifstream in(path);
    PD_CORE_ASSERT(in, "Could not open '{0}' for reading", path);
    if (in)
    {
        in.seekg(0, in.end);
        PDsizei szLength = in.tellg();
        if (szLength > 0)
        {
            in.seekg(0, in.beg);
            result.resize(szLength);
            in.read(result.data(), szLength);
        }
        else
        {
            PD_CORE_ERROR("Failed to read input stream...file: {0}", path);
        }
    }
    else
    {
        PD_CORE_ERROR("Could not open '{0}' for reading", path);
    }

    return PD_MOVE(result);
}

OpenGLShader::SourceMap OpenGLShader::PreProcess(const PDstring& source) const
{
    SourceMap result;

    const char* cpToken = "#type";
    const size_t szTokenLength = strlen(cpToken);
    size_t szPos = source.find(cpToken, 0);
    PD_CORE_ASSERT(szPos != std::string::npos, "Syntax Error: " \
        "source contains no #type declarations");

    while (szPos != std::string::npos)
    {
        const size_t eol = source.find_first_of("\r\n", szPos); // End of #type declaration
        PD_CORE_ASSERT(eol != std::string::npos, "Syntax Error: no text after #type declaration");

        const size_t szBegin = szPos + szTokenLength + 1; // Position after "#type "
        PDstring sType = source.substr(szBegin, eol - szBegin);
        PD_CORE_ASSERT(GetShaderType(sType), "Invalid shader type '{0}'", sType);

        size_t szNextLinePos = source.find_first_not_of("\r\n ", eol); // Find first line of actual code
        PD_CORE_ASSERT(szNextLinePos != std::string::npos, "Syntax Error: no code after #type declaration");
        szPos = source.find(cpToken, szNextLinePos);

        result[GetShaderType(sType)] = (szPos == std::string::npos) ? source.substr(szNextLinePos)
            : source.substr(szNextLinePos, szPos - szNextLinePos);
    }

    return PD_MOVE(result);
}

GLenum OpenGLShader::GetShaderType(const PDstring& name) const
{
    if (name == "vertex")
        return GL_VERTEX_SHADER;
    else if (name == "fragment" || name == "pixel")
        return GL_FRAGMENT_SHADER;

    return 0;
}

const char* OpenGLShader::GetShaderType(GLenum type) const
{
    if (type == GL_VERTEX_SHADER)
        return "vertex";
    else
        return "fragment";
}

bool OpenGLShader::CompileShader(const OpenGLShader::SourceMap& sources)
{
    PD_CORE_ASSERT(sources.size() <= 2, "Only two shaders supported");

    Dewpsi::Array<GLuint, 2> shaderIds = {0, 0};
    short int iShaderIdIndex = 0;
    bool result = true;

    auto clearShaders = [&shaderIds](GLuint program) -> void {
        for (auto& val : shaderIds)
        {
            if (val)
            {
                glDetachShader(program, val);
                glDeleteShader(val);
                val = 0;
            }
        }
    };

    // compile each shader
    GLuint program = glCreateProgram();

    for (auto& src : sources)
    {
        const char* cpShaderType = GetShaderType(src.first);

        // src: pair: first=GLenum, second=PDstring
        GLuint shader = glCreateShader(src.first);
        PD_CORE_ASSERT(shader, "Failed to create {} shader", cpShaderType);

        // compile source code
        const char* cpShaderSource = src.second.c_str();
        GLCall(glShaderSource(shader, 1, &cpShaderSource, nullptr));
        GLCall(glCompileShader(shader));

        // error check
        if (CheckShader(shader, cpShaderType))
        {
            glAttachShader(program, shader);
            shaderIds[iShaderIdIndex++] = shader;
        }
        else
        {
            result = false;
            SetError("%s", m_ErrorLog.data());
            break;
        }
    }

    // delete program if this fails
    if (! result)
    {
        glDeleteProgram(program);
        clearShaders(program);
    }
    else
    {
        // link shader objects into program object
        m_ShaderID = program;
        program = 0;
        glLinkProgram(m_ShaderID);
        if (! CheckProgram(m_ShaderID))
        {
            clearShaders(m_ShaderID);
            glDeleteProgram(m_ShaderID);
            m_ShaderID = 0;
        }
        clearShaders(m_ShaderID);
    }

    m_ErrorLog.clear();
    return result;
}

bool OpenGLShader::CheckShader(GLuint shader, const char* desc)
{
    int iStatus = 0, iLogLen = 0;

    std::vector<char> errorLog;

    // get the compile status and info string length
    glGetShaderiv(shader, GL_COMPILE_STATUS, &iStatus);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &iLogLen);

    // there is an error
    if (iStatus == GL_FALSE)
    {
        // print the error
        if (iLogLen > 1)
        {
            m_ErrorLog.resize(static_cast<PDuint>(iLogLen + 1));
            glGetShaderInfoLog(shader, iLogLen, nullptr, m_ErrorLog.data());
            PD_CORE_ERROR("Failed to compile {0} shader: {1}", desc, m_ErrorLog.data());
        }
    }

    return (bool) iStatus == GL_TRUE;
}

bool OpenGLShader::CheckProgram(PDuint program)
{
    int iStatus, iLogLen;

    // get link status of the program
    glGetProgramiv(program, GL_LINK_STATUS, &iStatus);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &iLogLen);

    // there is an error
    if (iStatus == GL_FALSE)
    {
        // print the error
        if (iLogLen > 1)
        {
            m_ErrorLog.resize(static_cast<PDuint>(iLogLen + 1));
            glGetProgramInfoLog(program, iLogLen, nullptr, m_ErrorLog.data());
            PD_CORE_ERROR("ERROR: failed to link shader program: {1}", m_ErrorLog.data());
        }
    }

    return (bool) iStatus == GL_TRUE;
}

}
