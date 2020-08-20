#ifndef DEWPSI_OPENGLCONTEXT_H
#define DEWPSI_OPENGLCONTEXT_H

#include <Dewpsi_Core.h>
#include <Dewpsi_RenderContext.h>
#include <SDL.h>

namespace Dewpsi {
    class OpenGLContext : public RenderContext {
    public:
        OpenGLContext(SDL_Window* windowHandle);
        virtual ~OpenGLContext();

        virtual int Init() override;
        virtual void SwapBuffers() override;

    private:
        SDL_Window* m_WindowHandle;
        SDL_GLContext m_Context;
    };
}

#endif /* DEWPSI_OPENGLCONTEXT_H */
