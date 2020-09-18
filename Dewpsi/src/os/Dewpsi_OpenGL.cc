#include "Dewpsi_OpenGL.h"

static const char* _whatiserror(GLenum error)
{
    #define charArg(x) case x: return #x; break;

    switch (error)
    {
        charArg(GL_INVALID_ENUM)
        charArg(GL_INVALID_VALUE)
        charArg(GL_INVALID_OPERATION)
        charArg(GL_OUT_OF_MEMORY)
        charArg(GL_STACK_UNDERFLOW)
        charArg(GL_STACK_OVERFLOW)

        default: break;
    }

    return "undefined";
}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    GLenum error = glGetError();

    if (error)
    {
        std::cerr << "[OpenGL Error] (" << error << '=' << _whatiserror(error) << "): "\
            << function << " " << file << ':' << line << '\n';
        return false;
    }

    return true;
}

bool GLGetError(char** dst, PDuint shader)
{
    int iParam;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &iParam);
    PD_CORE_ASSERT(dst, "NULL 'dst' parameter");
}

void GLPrintActiveUniforms(PDuint shader)
{
    GLint iNumUniforms, uniformLength;
    GLenum type;
    GLsizei length;
    char caUniformName[60];
    glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &iNumUniforms);
    for (PDuint i = 0; i < iNumUniforms; ++i)
    {
        glGetActiveUniform(
            shader,
            i,
            sizeof(caUniformName),
            &length,
            &uniformLength,
            &type,
            caUniformName
        );
        if (length)
        {
            PD_CORE_TRACE("Uniform at index {0}, name: {1}", i, caUniformName);
        }
    }
}
