#include "Dewpsi_RenderContext.h"
#include "Dewpsi_OpenGLContext.h"

namespace Dewpsi {

Scope<RenderContext> RenderContext::Create(void* window)
{
    return CreateScope<OpenGLContext>((SDL_Window*) window);
}

}
