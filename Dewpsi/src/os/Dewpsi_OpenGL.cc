#include "Dewpsi_OpenGL.h"
#include <DewpsiMath_Util.hpp>

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

#ifndef GL_VERSION_4_5
#warning "[OPENGL] Using Dewpsi to implement glCreateTextures"
void Dewpsi_glCreateTextures(GLenum target, GLsizei n, GLuint *textures)
{
    glGenTextures(n, textures);
    glBindTexture(target, *textures);
}
#endif

#ifndef GL_VERSION_4_2
#warning "[OPENGL] Using Dewpsi to implement glTexStorage2D and glTextureStorage2D"

static void getTexFormat(GLenum internalFormat, GLenum& dataFormat, GLenum& dataType)
{
    #define enumArg(param, fmt, type) \
        case param: \
            dataFormat = fmt; \
            dataType = type; \
            break;

    switch (internalFormat)
    {
        enumArg(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE)   // 24 bit RGB unsigned
        enumArg(GL_RGB8_SNORM, GL_RGB, GL_BYTE)      // 24 bit RGB signed
        enumArg(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE) // 32 bit RGBA unsigned
        enumArg(GL_RGBA8_SNORM, GL_RGBA, GL_BYTE)    // 32 bit RGBA signed
    }

    PD_CORE_ASSERT(false, "Unknown format {0:x}", internalFormat);
}

#define texStorageSig GLenum target, GLsizei levels, GLenum internalformat, GLsizei width,\
                      GLsizei height, int dataFormat, int dataType

static void texStorageTarget2DTexture(texStorageSig)
{
    constexpr GLsizei one = GLsizei(1);
    constexpr GLsizei two = GLsizei(2);

    for (int i = 0; i < levels; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, i, internalformat, width, height, 0, dataFormat, dataType, nullptr));
        width = dm::max(one, width / two);
        height = dm::max(one, height / two);
    }
}

void Dewpsi_glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat,
    GLsizei width, GLsizei height)
{
    void (* int_glTexStorage2D)(texStorageSig) = nullptr;

    GLenum dataFormat, dataType;

    switch(target)
    {
        case GL_TEXTURE_2D:
            int_glTexStorage2D = texStorageTarget2DTexture;
            getTexFormat(internalformat, dataFormat, dataType);
            break;

        default:
            PD_CORE_ASSERT(false, "Unrecoginized target!");
            break;
    }

    PD_CORE_ASSERT(int_glTexStorage2D, "NULL function pointer");

    int_glTexStorage2D(target, levels, internalFormat, width, height, dataFormat, dataType);
}
#endif

#ifndef GL_VERSION_4_5
#warning "[OPENGL] Implementing glTextureStorage2D with Dewpsi, because not using GLAD for OpenGL 4.5"
void Dewpsi_glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat,
    GLsizei width, GLsizei height)
{
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
    GLCall(glTexStorage2D(GL_TEXTURE_2D, levels, internalformat, width, height));
}

#warning "[OPENGL] Not using version 4.5, so glTextureSubImage2D is implemented in Dewpsi"
void Dewpsi_glTextureSubImage2D(GLuint texture, GLsizei level, GLint xoffset,
    GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
    const void* pixels)
{
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, format, type, pixels));
}

#endif // GL_VERSION_4_5
