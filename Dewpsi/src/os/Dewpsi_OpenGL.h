#ifndef DEWPSI_OPENGL_H
#define DEWPSI_OPENGL_H

#include <Dewpsi_Core.h>
#include <Dewpsi_ImGui_OpenGL3.h>
#include <glad/glad.h>
//#define PD_GL_FUNC_DECLS
//#include <Dewpsi_GLFuncs.h>

#ifndef glCreateBuffers
    #define glCreateBuffers(cnt, p) glGenBuffers(cnt, p)
#endif

#define GLASSERT(x) if (! (x)) std::abort();

#define GLCall(x)   GLClearError(); \
                    x; \
                    GLASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

bool GLGetProgramError(char** dst, PDuint shader);

void GLPrintActiveUniforms(PDuint shader);

#endif /* DEWPSI_OPENGL_H */
