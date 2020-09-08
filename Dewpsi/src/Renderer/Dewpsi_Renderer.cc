#include "Dewpsi_Renderer.h"
#include "Dewpsi_Shader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Dewpsi {

Scope<Renderer::SceneData> Renderer::s_SceneData(new Renderer::SceneData());

void Renderer::BeginScene(OrthoCamera& camera)
{
    s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{

}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
{
    shader->Bind();
    shader->UploadUniformMat4("U_ViewProjection", glm::value_ptr(s_SceneData->viewProjectionMatrix));
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

}
