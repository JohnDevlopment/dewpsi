#include "Dewpsi_OpenGLVertexArray.h"

static GLenum ShaderType2OpenGLEnum(Dewpsi::ShaderDataType type);

namespace Dewpsi {

OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &m_ArrayID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteBuffers(1, &m_ArrayID);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(m_ArrayID);
}

void OpenGLVertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
    const BufferLayout& layout = vertexBuffer->GetLayout();
    PD_CORE_ASSERT(layout.GetElements().size(), "No layout defined");

    glBindVertexArray(m_ArrayID);
    vertexBuffer->Bind();

    PDuint32 uiOffset = 0;

    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(uiOffset);
        glVertexAttribPointer(uiOffset, element.GetComponentCount(),
                              ShaderType2OpenGLEnum(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                              (void*) element.offset);
        ++uiOffset;
    }
    m_VertexBuffers.push_back(vertexBuffer);
    vertexBuffer->UnBind();
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
    glBindVertexArray(m_ArrayID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}

}

GLenum ShaderType2OpenGLEnum(Dewpsi::ShaderDataType type)
{
    using Dewpsi::ShaderDataType;

    switch (type)
    {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
    }

    return (GLenum) 0;
}
