#ifndef DEWPSI_WHICHOS_H
#define DEWPSI_WHICHOS_H

#if defined(PD_PLATFORM_LINUX) || defined(__linux__)
    #include <Dewpsi_SDLWindow.h>
    #include <Dewpsi_SDLPlatform.h>
    #include <Dewpsi_SDLInput.h>
    #include <Dewpsi_ImGui_SDL.h>
    #include <Dewpsi_OpenGL.h>
    #include <Dewpsi_OpenGLContext.h>
    #include <Dewpsi_OpenGLBuffer.h>
    #include <Dewpsi_OpenGLTexture.h>
    #include <Dewpsi_OpenGLVertexArray.h>
    #include <Dewpsi_OpenGLShader.h>
    #include <Dewpsi_OpenGLRendererAPI.h>
#else
    #error Currently only Linux is supported
#endif

#endif /* DEWPSI_WHICHOS_H */
