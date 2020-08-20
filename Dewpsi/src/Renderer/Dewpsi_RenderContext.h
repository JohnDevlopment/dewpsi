#ifndef DEWPSI_RENDERCONTEXT_H
#define DEWPSI_RENDERCONTEXT_H

#include <Dewpsi_Core.h>
#include <Dewpsi_Memory.h>

namespace Dewpsi {
    class RenderContext {
    public:
        virtual ~RenderContext()
        {  }

        virtual int Init() = 0;
        virtual void SwapBuffers() = 0;

        // TODO: expand RenderContext::Create for OpenGL and Vulkan support
        static Scope<RenderContext> Create(void* window);
    };
}

#endif /* DEWPSI_RENDERCONTEXT_H */
