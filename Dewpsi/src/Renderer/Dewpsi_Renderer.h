#ifndef DEWPSI_RENDERER_H
#define DEWPSI_RENDERER_H

/** @file     Dewpsi_Renderer.h
*   @brief    @doxfb
*	@defgroup renderer Rendering API
*	@c %Renderer provides an interface to rendering graphics.
*	@ingroup  core
*/

#include <Dewpsi_Memory.h>
#include <Dewpsi_RenderCommand.h>

namespace Dewpsi {
    /** @addtogroup renderer
    *   @{
    */

    /** High-level rendering interface.
    *   This class interprets high-level data constructs from
    *   %Dewpsi and delegates taks down the hierchy down to the
    *   actual rendering API.
    */
    class Renderer {
    public:
        /// Begins a scene.
        static void BeginScene();

        /// Ends a scene.
        static void EndScene();

        /// Submits a vertex array to the render queue.
        static void Submit(const Ref<VertexArray>& vertexArray);

        /// Sets the current rendering API.
        static void SetAPI(RendererAPI::API api) {RendererAPI::SetAPI(api);}

        /// Returns the current rendering API.
        static RendererAPI::API GetAPI() {return RendererAPI::GetAPI();}
    };

    /// @}
}

#endif /* DEWPSI_RENDERER_H */
