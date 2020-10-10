#include "Dewpsi_Renderer.h"
#include "Dewpsi_Shader.h"
#include "Dewpsi_OpenGLShader.h"
#include "Dewpsi_Texture.h"

namespace Dewpsi {

Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

void Renderer::Init()
{
    RenderCommand::Init();
}

void Renderer::BeginScene(OrthoCamera& camera)
{
    s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
    const glm::mat4& transform)
{
    shader->Bind();
    vertexArray->Bind();
    shader->SetMat4("u_ViewProjection", 1, &s_SceneData->viewProjectionMatrix);
    shader->SetMat4("u_Transform", 1, &transform);
    RenderCommand::DrawIndexed(vertexArray);
}

}
