#ifndef DEWPSI_OPENGL_H
#define DEWPSI_OPENGL_H

#include <Dewpsi_Core.h>
#include <Dewpsi_ImGui_OpenGL3.h>
#include <glad/glad.h>
//#define PD_GL_FUNC_DECLS
//#include <Dewpsi_GLFuncs.h>

#define GLASSERT(x) if (! (x)) std::abort();

#define GLCall(x)   GLClearError(); \
                    x; \
                    GLASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

bool GLGetProgramError(char** dst, PDuint shader);

void GLPrintActiveUniforms(PDuint shader);

// Alias for glGenBuffers
#ifndef glCreateBuffers
    #define glCreateBuffers(cnt, p) glGenBuffers(cnt, p)
#endif

/*
Expose glTexStorage2D; it's natively available from OpenGL 4.2 onward. If we're using an older
version of OpenGL, expose a Dewpsi function that implements roughly the same behavior, then define
a macro pointing to it.
*/
#ifndef GL_VERSION_4_2
    PD_CALL void Dewpsi_glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat,
        GLsizei width, GLsizei height);
    #define glTexStorage2D(target, levels, intfmt, width, height) \
        Dewpsi_glTexStorage2D(target, levels, intfmt, width, height)
#endif

#ifndef GL_VERSION_4_5
    /*
    Expose glCreateTextures to the client. In OpenGL 4.5, it's automatically exposed by the GLAD
    loader, so no further action is neccessary. But otherwise a function is implemented and exposed
    to the client that mimics the behavior of glCreateTextures.
    */
    PD_CALL void Dewpsi_glCreateTextures(GLenum target, GLsizei n, GLuint *textures);
    #define glCreateTextures(target, cnt, p) Dewpsi_glCreateTextures(target, cnt, p)
    PD_CALL void Dewpsi_glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat,
        GLsizei width, GLsizei height);
    /*
    Expose glTextureStorage2D. It's available in OpenGL 4.5. If we are using an older version--even
    like 4.3 or 4.2--then we provide an alternate version of that function. It basically does the
    same thing as glTextureStorage2D.
    */
    #define glTextureStorage2D(target, levels, intfmt, width, height) \
        Dewpsi_glTextureStorage2D(target, levels, intfmt, width, height)
    /*
    Implement glTextureSubImage2D if we're not using the OpenGL 4.5 version.
    */
    PD_CALL void Dewpsi_glTextureSubImage2D(GLuint texture, GLsizei level, GLint xoffset,
        GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
        const void* pixels);
    #define glTextureSubImage2D(texture, level, x, y, w, h, format, type, pixels) \
        Dewpsi_glTextureSubImage2D(texture, level, x, y, w, h, format, type, pixels)
#endif // !defined(GL_VERSION_4_5)

#endif /* DEWPSI_OPENGL_H */
