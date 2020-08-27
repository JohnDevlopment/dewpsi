#include "Dewpsi_OpenGLRendererAPI.h"

namespace Dewpsi {

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
