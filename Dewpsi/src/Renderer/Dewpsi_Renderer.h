#ifndef DEWPSI_RENDERER_H
#define DEWPSI_RENDERER_H

/** @file     Dewpsi_Renderer.h
*   @brief    @doxfb
*	@defgroup renderer Rendering API
*	@c %Renderer provides an interface to rendering graphics.
*	@ingroup  core
*/

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /** @addtogroup renderer
    *   @{
    */

    /// Rendering API
    enum class RendererAPI {
        None,       ///< No API selected
        OpenGL      ///< OpenGL API
    };

    /** Rendering interface.
    *   Selects the API used internally.
    */
    class Renderer {
    public:
        /// Returns the current API.
        static RendererAPI GetAPI() { return s_API; }

        /// Changes the API that is used.
        static void SetAPI(RendererAPI api) {
            s_API = api;
        }

    private:
        static RendererAPI s_API;
    };

    /// @}
}

#endif /* DEWPSI_RENDERER_H */
