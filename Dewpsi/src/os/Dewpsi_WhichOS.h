#ifndef DEWPSI_WHICHOS_H
#define DEWPSI_WHICHOS_H

#if defined(PD_PLATFORM_LINUX) || defined(__linux__)
    #include <Dewpsi_SDLWindow.h>
    #include <Dewpsi_ImGui_SDL.h>
    #include <Dewpsi_OpenGL.h>
    #define PD_GL_FUNC_DECLS
    #include <Dewpsi_GLFuncs.h>
    #include <Dewpsi_ImGui_OpenGL3.h>
#else
    #error Currently only Linux is supported
#endif

#endif /* DEWPSI_WHICHOS_H */
