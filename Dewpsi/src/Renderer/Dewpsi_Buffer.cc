#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Buffer.h"
#include "Dewpsi_Renderer.h"
#include "Dewpsi_Except.h"

#define FLOAT_DATA(n) (sizeof(float) * (n))
#define INT_DATA(n) (sizeof(int) * (n))

#define NEW_VERTEX_BUFFER(type, ...) static_cast<VertexBuffer*>(new type(__VA_ARGS__));
#define NEW_INDEX_BUFFER(type, ...) static_cast<IndexBuffer*>(new type(__VA_ARGS__));

namespace Dewpsi {

uint32_t ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:
            return sizeof(float);
        case ShaderDataType::Float2:
            return FLOAT_DATA(2);
        case ShaderDataType::Float3:
            return FLOAT_DATA(3);
        case ShaderDataType::Float4:
            return FLOAT_DATA(4);
        case ShaderDataType::Mat3:
            return FLOAT_DATA(3 * 3);
        case ShaderDataType::Mat4:
            return FLOAT_DATA(4 * 4);
        case ShaderDataType::Int:
            return sizeof(int);
        case ShaderDataType::Int2:
            return INT_DATA(2);
        case ShaderDataType::Int3:
            return INT_DATA(3);
        case ShaderDataType::Int4:
            return INT_DATA(4);
        case ShaderDataType::Bool:
            return 1;
        default:
            break;
    }

    throw DewpsiError("Unkown data type");
    return 0;
}

PDuint32 BufferElement::GetComponentCount() const
{
    switch (type)
    {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return (3 * 3);
        case ShaderDataType::Mat4:
            return (4 * 4);
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            break;
    }

    throw DewpsiError("BufferElement::GetElementCount: Unknown shader type");
    return 0;
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elms)
{
    for (auto& elm : elms)
    {
        m_Elements.push_back(elm);
    }
    CalculateOffsetsAndStride();
}

void BufferLayout::CalculateOffsetsAndStride()
{
    PDsizei szOffset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements)
    {
        element.offset = szOffset;
        szOffset += element.size;
        m_Stride += element.size;
    }
}

Ref<VertexBuffer> VertexBuffer::Create(PDsizei size, const PDfloat* data)
{
    #define _ERROR(msg) "VertexBuffer::Create: " msg

    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            throw DewpsiError(_ERROR("Renderer API is set to none"));
            break;

        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexBuffer>(size, data);
            break;

        default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;

    #undef _ERROR
}

Ref<IndexBuffer> IndexBuffer::Create(PDsizei size, const PDuint32* data)
{
    #define _ERROR(msg) "IndexBuffer::Create: " msg

    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            throw DewpsiError(_ERROR("Renderer API is set to none"));
            break;

        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLIndexBuffer>(size, data);
            break;

        default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;

    #undef _ERROR
}

}
