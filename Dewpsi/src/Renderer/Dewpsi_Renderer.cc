#include "Dewpsi_Renderer.h"
#include "Dewpsi_Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "Dewpsi_OpenGLShader.h"

static bool Begin = false;

namespace Dewpsi {

Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

void Renderer::BeginScene(OrthoCamera& camera)
{
    s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
    Begin = true;
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
    const glm::mat4& transform)
{
    static Shader* CurrentShader = nullptr;

    if (CurrentShader != shader.get())
    {
        CurrentShader = shader.get();
        CurrentShader->Bind();
    }

    if (Begin)
    {
        Begin = false;
        static_cast<OpenGLShader*>(CurrentShader)->UploadUniformMat4(
            "u_ViewProjection",
            glm::value_ptr(s_SceneData->viewProjectionMatrix)
        );
    }

    vertexArray->Bind();
    static_cast<OpenGLShader*>(CurrentShader)->UploadUniformMat4(
        "u_Transform",
        glm::value_ptr(transform)
    );
    RenderCommand::DrawIndexed(vertexArray);
}

}
