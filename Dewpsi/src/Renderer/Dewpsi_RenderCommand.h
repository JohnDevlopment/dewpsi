#ifndef DEWPSI_RENDERCOMMAND_H
#define DEWPSI_RENDERCOMMAND_H

#include <Dewpsi_Memory.h>
#include <Dewpsi_Color.h>
#include <Dewpsi_RendererAPI.h>

namespace Dewpsi {
    class RenderCommand {
    public:
        /// Initialize the rendering API.
        static void Init();

        /// Sets the clear color.
        static void SetClearColor(const Color& color)
        {
            s_RenderingAPI->SetClearColor(color);
        }

        /// Clears the buffers.
        static void Clear()
        {
            s_RenderingAPI->Clear();
        }

        /// Draws the given vertex array.
        static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RenderingAPI->DrawIndexed(vertexArray);
        }

    private:
        static RendererAPI* s_RenderingAPI;
    };
}

#endif /* DEWPSI_RENDERCOMMAND_H */
