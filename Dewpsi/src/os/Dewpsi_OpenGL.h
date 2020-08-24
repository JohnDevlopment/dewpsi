#ifndef DEWPSI_OPENGL_H
#define DEWPSI_OPENGL_H

#include <Dewpsi_ImGui_OpenGL3.h>
#include <glad/glad.h>
//#define PD_GL_FUNC_DECLS
//#include <Dewpsi_GLFuncs.h>

#ifndef glCreateBuffers
    #define glCreateBuffers(cnt, p) glGenBuffers(cnt, p)
#endif

#endif /* DEWPSI_OPENGL_H */
