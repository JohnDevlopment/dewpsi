#include "Dewpsi_OpenGLRendererAPI.h"

namespace Dewpsi {

void OpenGLRendererAPI::Init()
{
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CCW);

    PD_CORE_TRACE("Initialized OpenGLRendererAPI");
}

void OpenGLRendererAPI::SetClearColor(const Color& color)
{
    m_ClearColor = color;
    FColor temp = color;
    glClearColor(temp.red, temp.green, temp.blue, temp.alpha);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
                  GL_UNSIGNED_INT, nullptr);
}


}
