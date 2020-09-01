#include "Dewpsi_Shader.h"
#include "Dewpsi_OpenGLShader.h"
#include "Dewpsi_Memory.h"
#include "Dewpsi_String.h"
#include "Dewpsi_OpenGL.h"
#include "_Dewpsi_Int_Shader.h"
#include "Dewpsi_Except.h"

using Dewpsi::Internal::ShaderProgramSource;
using Dewpsi::Internal::ShaderType;

static Dewpsi::Scope<char[]> g_Error;
static PDuint g_ErrorLen = 0;

static PDuint CreateShader(const ShaderProgramSource& source,
    PDuint* pVert = nullptr, PDuint* pFrag = nullptr);
static bool CheckShader(PDuint shader, const char* desc);
static bool CheckProgram(PDuint program, const char* desc);
static void ResizeErrorPtr(PDuint len);

namespace Dewpsi {

OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    ShaderProgramSource sources = { vertexSrc, fragmentSrc };
    m_ShaderID = CreateShader(sources);
    PD_CORE_ASSERT(m_ShaderID, "Failed to link shader program");
    if (! m_ShaderID)
        throw DewpsiError("OpenGL shader has failed to compile");
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

}

// INTERNAL

/*
ShaderProgramSource ParseShaderFile(const std::string& filename)
{
    ShaderType type = ShaderType::None;
    std::stringstream ss[3];
    std::string sLine;

    std::ifstream ifs(filename);
    PD_CORE_ASSERT(ifs.is_open(), "Failed to open {0}", filename);

    while (std::getline(ifs, sLine))
    {
        if (sLine.find("#shader") != std::string::npos)
        {
            if (sLine.find("vertex") != std::string::npos)
                type = ShaderType::Vertex;
            else if (sLine.find("fragment") != std::string::npos)
                type = ShaderType::Fragment;
            else if (sLine.find("geometry") != std::string::npos)
                type = ShaderType::Geometry;
        }
        else
        {
            ss[(int) type] << sLine << '\n';
        }
    }

    return { ss[0].str(), ss[1].str(), ss[2].str() };
}
*/

PDuint CompileShader(PDuint type, const std::string& source)
{
    PDuint uiShader = 0;

    static constexpr Dewpsi::StaticString ShaderStrings[2] = {
        "vertex",
        "fragment"
    };

    if (! source.empty())
    {
        uiShader = glCreateShader(type);
        const char* cpSource = source.c_str();

        // shader type
        ShaderType eType = ShaderType::None;
        switch (type)
        {
        case GL_VERTEX_SHADER:
            eType = ShaderType::Vertex;
            break;

        case GL_FRAGMENT_SHADER:
            eType = ShaderType::Fragment;
            break;

        default: break;
        }

        // compile the source code into a shader
        glShaderSource(uiShader, 1, &cpSource, nullptr);
        glCompileShader(uiShader);

        if (! CheckShader(uiShader, ShaderStrings[(int)eType].get()))
        {
            glDeleteShader(uiShader);
            return 0;
        }
    }

    return uiShader;
}

PDuint CreateShader(const ShaderProgramSource& source, PDuint* pVert, PDuint* pFrag)
{
    PDuint uiProg, uiVS, uiFS;

#define _DETACH_SHADERS() glDetachShader(uiProg, uiVS); \
                          glDetachShader(uiProg, uiFS);

    // compile shaders
    uiProg = glCreateProgram();
    uiVS = CompileShader(GL_VERTEX_SHADER, source.vertex);
    uiFS = CompileShader(GL_FRAGMENT_SHADER, source.fragment);
    PD_CORE_ASSERT(uiVS && uiFS, "one or both shaders failed to compile");

    // link shaders to program
    glAttachShader(uiProg, uiVS);
    glAttachShader(uiProg, uiFS);
    glLinkProgram(uiProg);

    if (! CheckProgram(uiProg, "shader program"))
    {
        _DETACH_SHADERS()
        DeleteShaders(uiVS, uiFS);
        glDeleteProgram(uiProg);
    }

    // validate shader program
    if (reinterpret_cast<void*>(glGetProgramPipelineiv) != nullptr) {
        int iCode;
        glValidateProgram(uiProg);
        glGetProgramPipelineiv(uiProg, GL_VALIDATE_STATUS, &iCode);
        if (iCode == GL_FALSE)
        {
            int iLogLen;
            glGetProgramPipelineiv(uiProg, GL_INFO_LOG_LENGTH, &iLogLen);
            if (iLogLen > 1)
            {
                ResizeErrorPtr(static_cast<PDuint>(iLogLen + 1));
                glGetProgramPipelineInfoLog(uiProg, iLogLen, nullptr, g_Error.get());
                PD_CORE_ERROR("ERROR: validation failed, reason:\n{0}", g_Error.get());
                // delete shader program
                _DETACH_SHADERS()
                DeleteShaders(uiVS, uiFS);
                glDeleteProgram(uiProg);
                uiProg = 0;
            }
        }
    }

#undef _DETACH_SHADERS

    return uiProg;
}

void ResizeErrorPtr(PDuint len)
{
    if ((int) g_ErrorLen < len)
    {
        g_ErrorLen = len;
        g_Error = Dewpsi::CreateScope<char[]>((size_t) g_ErrorLen);
    }
}

bool CheckProgram(PDuint program, const char* desc)
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
            ResizeErrorPtr(static_cast<PDuint>(iLogLen + 1));
            glGetProgramInfoLog(program, iLogLen, nullptr, g_Error.get());
            PD_CORE_ERROR("ERROR: failed to link {0}: {1}", desc, g_Error.get());
        }
    }

    return (bool) iStatus == GL_TRUE;
}

bool CheckShader(PDuint shader, const char* desc)
{
    int iStatus = 0, iLogLen = 0;

    // get the compile status and info string length
    glGetShaderiv(shader, GL_COMPILE_STATUS, &iStatus);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &iLogLen);

    // there is an error
    if (iStatus == GL_FALSE)
    {
        // print the error
        if (iLogLen > 1)
        {
            ResizeErrorPtr(static_cast<PDuint>(iLogLen + 1));
            glGetShaderInfoLog(shader, iLogLen, nullptr, g_Error.get());
            PD_CORE_ERROR("ERROR: failed to compile {0} shader: {1}", desc, g_Error.get());
        }
    }

    return (bool) iStatus == GL_TRUE;
}
