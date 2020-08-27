#include "Dewpsi_Renderer.h"

namespace Dewpsi {

void Renderer::BeginScene()
{

}

void Renderer::EndScene()
{

}

void Renderer::Submit(const Ref<VertexArray>& vertexArray)
{
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

}
