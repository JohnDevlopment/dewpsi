#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Application.h"
#include "Dewpsi_String.h"

#include <fstream>
#include <sstream>
#include <string>
#include <utility>


#define PD_GL_FUNC_DEFS
#include "Dewpsi_GLFuncs.h"

static unsigned int g_uiErrorLength = 500U;
static std::unique_ptr<char[]> g_cpError(new char[g_uiErrorLength]);
static unsigned int g_uiProgram = 0;

using namespace Dewpsi_ImGui_Impl;

static constexpr Dewpsi::StaticString ShaderStrings[] = {
    "vertex",
    "fragment",
    "geometry"
};

namespace Dewpsi_ImGui_Impl {

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

unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int uiShader = 0;
    
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
        
        case GL_GEOMETRY_SHADER:
            eType = ShaderType::Geometry;
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

unsigned int CreateShader(const ShaderProgramSource& source, PDuint* pVert, PDuint* pFrag)
{
    unsigned int uiProg, uiVS, uiFS, uiGS;
    
    // compile shaders
    uiProg = glCreateProgram();
    uiVS = CompileShader(GL_VERTEX_SHADER, source.vertex);
    uiFS = CompileShader(GL_FRAGMENT_SHADER, source.fragment);
    uiGS = CompileShader(GL_GEOMETRY_SHADER, source.geometry);
    PD_CORE_ASSERT(uiVS && uiFS, "one or both shaders failed to compile");
    
    // link shaders to program
    glAttachShader(uiProg, uiVS);
    glAttachShader(uiProg, uiFS);
    glAttachShader(uiProg, uiGS);
    
    glLinkProgram(uiProg);
    
    if (! CheckProgram(uiProg, "shader program"))
    {
        DeleteShaders(uiVS, uiFS, uiGS);
        glDeleteProgram(uiProg);
    }
    
    // validate shader program
    {
        int iCode;
        glValidateProgram(uiProg);
        glGetProgramPipelineiv(uiProg, GL_VALIDATE_STATUS, &iCode);
        if (iCode == GL_FALSE)
        {
            int iLogLen;
            glGetProgramPipelineiv(uiProg, GL_INFO_LOG_LENGTH, &iLogLen);
            if (iLogLen > 1)
            {
                ResizeErrorPtr(static_cast<unsigned int>(iLogLen + 1));
                glGetProgramPipelineInfoLog(uiProg, iLogLen, nullptr, g_cpError.get());
                PD_CORE_ERROR("ERROR: validation failed, reason:\n{0}", g_cpError.get());
                glDeleteProgram(uiProg);
                uiProg = 0;
            }
        }
    }
    
    // return handle to vertex shader
    if (pVert)
        *pVert = uiVS;
    else
        glDeleteShader(uiVS);
    
    // return handle to fragment shader
    if (pFrag)
        *pFrag = uiVS;
    else
        glDeleteShader(uiFS);
    
    glDeleteShader(uiGS);
    
    return uiProg;
}

void ResizeErrorPtr(unsigned int len)
{
    if ((int) g_uiErrorLength < len)
    {
        g_uiErrorLength = len;
        g_cpError.reset(new char[g_uiErrorLength]);
    }
}

bool CheckProgram(unsigned int program, const char* desc)
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
            ResizeErrorPtr(static_cast<unsigned int>(iLogLen + 1));
            glGetProgramInfoLog(program, iLogLen, nullptr, g_cpError.get());
            PD_CORE_ERROR("ERROR: failed to link {0}: {1}", desc, g_cpError.get());
        }
    }
    
    return (bool) iStatus == GL_TRUE;
}

bool CheckShader(unsigned int shader, const char* desc)
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
            ResizeErrorPtr(static_cast<unsigned int>(iLogLen + 1));
            glGetShaderInfoLog(shader, iLogLen, nullptr, g_cpError.get());
            PD_CORE_ERROR("ERROR: failed to compile {0} shader: {1}", desc, g_cpError.get());
        }
    }
    
    return (bool) iStatus == GL_TRUE;
}

}

static void BackupState(OpenGLState* state)
{
    if (! state)
    {
        PD_BADPARAM("state");
        PD_CORE_ERROR("{0}: must be a non-null pointer", Dewpsi::GetError());
        return;
    }
    
    // last active texture unit
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*) &state->lastActiveTexture);
    glActiveTexture(GL_TEXTURE0);
    
    // program pipeline
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &state->program);
    
    // last bound texture
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*) &state->texture);
    
    // bound sampler object
#ifdef GL_SAMPLER_BINDING
    glGetIntegerv(GL_SAMPLER_BINDING, (GLint*) &state->lastSampler);
#endif
    
    // array buffer
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &state->lastArrayBuffer);
    
    // get polygon mode
#ifdef GL_POLYGON_MODE
    glGetIntegerv(GL_POLYGON_MODE, (GLint*) state->lastPolygonMode);
#endif
    
    // viewport
    glGetIntegerv(GL_VIEWPORT, state->viewport);
    
    // scissor box
    glGetIntegerv(GL_SCISSOR_BOX, state->scissorBox);
    
    // source blend function for RGB
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&state->lastSourceRGB);
    
    // destination blend function for RGB
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&state->lastDestRGB);
    
    // source blend function for alpha
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&state->lastSourceAlpha);
    
    // destination blend function for alpha
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&state->lastDestAlpha);
    
    // last blend equation of RGB
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&state->lastBlendEqRGB);
    
    // last blend equation of alpha
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&state->lastBlendEqAlpha);
    
    // boolean flags
    state->isBlend = glIsEnabled(GL_BLEND);
    state->isCullFace = glIsEnabled(GL_CULL_FACE);
    state->isDepthTest = glIsEnabled(GL_DEPTH_TEST);
    state->isScissorTest = glIsEnabled(GL_SCISSOR_TEST);
}

static void RestoreState(const OpenGLState* state)
{
    glUseProgram(state->program);
    glBindTexture(GL_TEXTURE_2D, state->texture);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, state->lastSampler);
#endif
    glActiveTexture(state->lastActiveTexture);
    glBindVertexArray(state->lastArrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, state->lastArrayBuffer);
    glBlendEquationSeparate(state->lastBlendEqRGB, state->lastBlendEqAlpha);
    glBlendFuncSeparate(state->lastSourceRGB, state->lastDestRGB, state->lastSourceAlpha, state->lastDestAlpha);
    
    if (state->isBlend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    
    if (state->isCullFace)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    
    if (state->isDepthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    
    if (state->isScissorTest)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
    
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum) state->lastPolygonMode[0]);
#endif
    
    glViewport(state->viewport[0], state->viewport[1], (GLsizei) state->viewport[2], (GLsizei) state->viewport[3]);
    glScissor(state->scissorBox[0], state->scissorBox[1], (GLsizei) state->scissorBox[2], (GLsizei) state->scissorBox[3]);
}

namespace Dewpsi {

bool OpenGL_InitShaders(const char* shaderFile)
{
    ShaderProgramSource sources = ParseShaderFile(shaderFile);
    g_uiProgram = CreateShader(sources);
    
    return g_uiProgram ? true : false;
}

void OpenGL_DeInitShaders()
{
    glDeleteProgram(g_uiProgram);
}

PDuint OpenGL_GetShader()
{
    return g_uiProgram;
}

void OpenGL_BackupState(OpenGLState* state)
{
    BackupState(state);
}

void OpenGL_RestoreState(const OpenGLState* state)
{
    RestoreState(state);
}

}
