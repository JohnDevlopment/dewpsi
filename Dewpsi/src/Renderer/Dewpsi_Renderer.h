#ifndef DEWPSI_RENDERER_H
#define DEWPSI_RENDERER_H

/** @file Dewpsi_Renderer.h
*	@defgroup core_renderer Rendering API
*	@ingroup  core
*/

#include <Dewpsi_Memory.h>
#include <Dewpsi_RenderCommand.h>
#include <Dewpsi_OrthoCamera.h> // glm

namespace Dewpsi {
    class Shader;

    /** High-level rendering interface.
    *   This class interprets high-level data constructs from
    *   %Dewpsi and delegates taks down the hierchy down to the
    *   actual rendering API.
    *   @ingroup core_renderer
    */
    class Renderer {
    public:
        /// Initialize the renderer.
        static void Init();

        /// Begins a scene for the view @a camera.
        static void BeginScene(OrthoCamera& camera);

        /// Ends a previously begun scene.
        static void EndScene();

        /// Submits a vertex array to the render queue.
        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
            const glm::mat4& transform = glm::mat4(1.0f));

        /// Sets the current rendering API.
        static void SetAPI(RendererAPI::API api) {RendererAPI::SetAPI(api);}

        /// Returns the current rendering API.
        static RendererAPI::API GetAPI() {return RendererAPI::GetAPI();}

    private:
        struct SceneData {
            glm::mat4 viewProjectionMatrix;
        };

        static Scope<SceneData> s_SceneData;
    };

    /// @}
}

#endif /* DEWPSI_RENDERER_H */
