#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Application.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>

static unsigned int g_uiErrorLength = 500U;
static std::unique_ptr<char[]> g_cpError(new char[g_uiErrorLength]);

//static int g_iGlVersion = 0;
static unsigned int g_uiProgram = 0;

unsigned int g_uiVertexBuffer = 0;

struct ShaderProgramSource {
    std::string vertex;
    std::string fragment;
};

enum class ShaderType {
    None     = -1,
    Vertex   =  0,
    Fragment =  1
};

static void ResizeErrorPtr(unsigned int len)
{
    if ((int) g_uiErrorLength < len)
    {
        g_uiErrorLength = len;
        g_cpError.reset(new char[g_uiErrorLength]);
    }
}

static ShaderProgramSource ParseShaderFile(const std::string& filename)
{
    ShaderType type = ShaderType::None;
    std::stringstream ss[2];
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
        }
        else
        {
            ss[(int) type] << sLine << '\n';
        }
    }
    
    return { ss[0].str(), ss[1].str() };
}

static bool CheckProgram(unsigned int program, const char* desc)
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

static bool CheckShader(unsigned int shader, const char* desc)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int uiShader = glCreateShader(type);
    const char* cpSource = source.c_str();
    
    // compile the source code into a shader
    glShaderSource(uiShader, 1, &cpSource, nullptr);
    glCompileShader(uiShader);
    
    if (! CheckShader(uiShader, (type == GL_VERTEX_SHADER ? "vertex" : "fragment")))
    {
        glDeleteShader(uiShader);
        return 0;
    }
    
    return uiShader;
}

static unsigned int CreateShader(const std::string& vertShader, const std::string& fragShader)
{
    unsigned int uiProg, uiVS, uiFS;
    
    // compile shaders
    uiProg = glCreateProgram();
    uiVS = CompileShader(GL_VERTEX_SHADER, vertShader);
    uiFS = CompileShader(GL_FRAGMENT_SHADER, fragShader);
    
    PD_CORE_ASSERT(uiVS && uiFS, "one or both shaders failed to compile");
    
    // link shaders to program
    glAttachShader(uiProg, uiVS);
    glAttachShader(uiProg, uiFS);
    glLinkProgram(uiProg);
    
    if (! CheckProgram(uiProg, "shader program"))
    {
        // free shaders
        glDeleteShader(uiVS);
        glDeleteShader(uiFS);
        glDeleteProgram(uiProg);
    }
    
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
    
    // free shaders
    glDeleteShader(uiVS);
    glDeleteShader(uiFS);
    
    return uiProg;
}

void OpenGL_Test1()
{
    std::string sVertexShaderSource, sFragmentShaderSource;
    
    ShaderProgramSource sources = ParseShaderFile("Dewpsi/OpenGL/shaders/shaders.glsl");
    
    // create the shader program and link it to the rendering context
    g_uiProgram = CreateShader(sources.vertex, sources.fragment);
    glUseProgram(g_uiProgram);
    
    // generate a buffer ID
    // bind the buffer to an array target
    glGenBuffers(1, &g_uiVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_uiVertexBuffer);
    
    // allocate memory for the buffer object and copy the bytes from faVerts
    // the buffer data is not expected to be mmodified
    // the buffer will be used for drawing vertices
    const float faVerts[6] = {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, faVerts, GL_STATIC_DRAW);
    
    /* set the attributes of the vertices: start index 0,
    2 components per vertex,  the attributes are floating points,
    do not normalize the vertices,   */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    // enable the vertex attributes starting from index 0
    glEnableVertexAttribArray(0);
    
    PD_CORE_PRINTFUNC();
}

void OpenGL_Test2()
{
    static bool doneOnce = false;
    if (! doneOnce)
    {
        PD_CORE_PRINTFUNC();
        doneOnce = true;
    }
    // set the shader program
    glUseProgram(g_uiProgram);
    
    // bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, g_uiVertexBuffer);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OpenGL_Test3()
{
    PD_CORE_PRINTFUNC();
    glDeleteBuffers(1, &g_uiVertexBuffer);
    glDeleteProgram(g_uiProgram);
}

