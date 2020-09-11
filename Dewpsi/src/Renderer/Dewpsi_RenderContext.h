#ifndef DEWPSI_RENDERCONTEXT_H
#define DEWPSI_RENDERCONTEXT_H

/** @file Dewpsi_RenderContext.h
*   @brief @doxfb
*   Contains a render context definition.
*   @ingroup renderer
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Memory.h>

namespace Dewpsi {
    /** A render context.
    *   This is automatically created when the Window is created.
    *   What type of context is created is dependent on what API
    *   is in use.
    *   @ingroup renderer
    */
    class RenderContext {
    public:
        RenderContext() = default;

        virtual ~RenderContext()
        {  }

        /// Initialize the context.
        virtual int Init() = 0;

        /// Swap the frame buffers.
        virtual void SwapBuffers() = 0;

        /// Create a new render context.
        static Scope<RenderContext> Create(void* window);
    };
}

#endif /* DEWPSI_RENDERCONTEXT_H */
