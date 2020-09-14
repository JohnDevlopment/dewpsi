#include "Dewpsi_Renderer2D.h"
#include "Dewpsi_Memory.h"
#include "Dewpsi_VertexArray.h"

#if 0

namespace Dewpsi {

struct Render2DSceneData {
    Ref<Shader> shader;
    Ref<VertexArray> vertexArray;

    Render2DSceneData() : shader(nullptr), vertexArray(nullptr) {}

    ~Render2DSceneData()
    {
        delete shader;
        delete vertexArray;
    }
};

static Render2DSceneData* _Data = nullptr;

void Renderer2D::Init()
{
    _Data = new Render2DSceneData;
    _Data->vertexArray = VertexArray::Create();

    {
        const float faVertices[] = {
           -0.5f,  0.0f, 0.0f,
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
           -0.5f,  0.5f, 0.0f
        };

        Ref<VertexBuffer> vbo(VertexBuffer::Create(sizeof(faVertices), faVertices));
        vbo->SetLayout({
            ShaderDataType::Float3, "A_Position"
        });
        _Data->vertexArray->AddVertexBuffer(vbo);
    }

    {
        const PDuint32 uiaIndices[] = {
            0, 1, 2,
            2, 3, 0
        };
        Ref<IndexBuffer> ibo(IndexBuffer::Create(PD_ARRAYSIZE(uiaIndices), uiaIndices));
        _Data->vertexArray->SetIndexBuffer(ibo);
    }
}

}

#endif // 0
