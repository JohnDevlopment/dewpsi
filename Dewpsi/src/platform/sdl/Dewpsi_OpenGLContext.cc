#include "Dewpsi_OpenGLContext.h"
#include "Dewpsi_WhichOS.h"

namespace Dewpsi {

OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
    : m_WindowHandle(windowHandle)
{
    PD_CORE_ASSERT(windowHandle, "Window handle pointer is NULL");
}

OpenGLContext::~OpenGLContext()
{
    SDL_GL_DeleteContext(m_Context);
    m_Context = nullptr;
    SDL_GL_UnloadLibrary();
}

int OpenGLContext::Init()
{
    // new context; automatically loads default GL libraries
    m_Context = SDL_GL_CreateContext(m_WindowHandle);
    if (! m_Context)
    {
        Dewpsi::SetError("%s", SDL_GetError());
        return PD_INVALID;
    }
    SDL_GL_MakeCurrent(m_WindowHandle, m_Context);

    // GLAD loader
    if (! gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        Dewpsi::SetError("GLAD loader returned an error status");
        return PD_INVALID;
    }
    /*PD_CORE_INFO(
        "OpenGL context\n  vendor: {0}\n  renderer: {1}\n  version: {2}",
        glGetString(GL_VENDOR),
        glGetString(GL_RENDERER),
        glGetString(GL_VERSION)
    );*/

    return PD_OKAY;
}

void OpenGLContext::SwapBuffers()
{
    SDL_GL_SwapWindow(m_WindowHandle);
}

}
