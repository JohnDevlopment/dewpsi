#include "Dewpsi_WhichOS.h"
//#include "Dewpsi_OpenGLBuffer.h"
#include "Dewpsi_Buffer.h"
#include "Dewpsi_Renderer.h"
#include "Dewpsi_Except.h"

#define NEW_VERTEX_BUFFER(type, ...) static_cast<VertexBuffer*>(new type(__VA_ARGS__));
#define NEW_INDEX_BUFFER(type, ...) static_cast<IndexBuffer*>(new type(__VA_ARGS__));

namespace Dewpsi {

VertexBuffer* VertexBuffer::Create(PDsizei size, const PDfloat* data)
{
    #define _ERROR(msg) "VertexBuffer::Create: " msg

    switch (Renderer::GetAPI())
    {
    case RendererAPI::None:
        throw DewpsiError(_ERROR("Renderer API is set to none"));
        break;

    case RendererAPI::OpenGL:
        return NEW_VERTEX_BUFFER(OpenGLVertexBuffer, size, data);
        break;

    default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;

    #undef _ERROR
}

IndexBuffer* IndexBuffer::Create(PDsizei size, const PDuint32* data)
{
    #define _ERROR(msg) "IndexBuffer::Create: " msg

    switch (Renderer::GetAPI())
    {
    case RendererAPI::None:
        throw DewpsiError(_ERROR("Renderer API is set to none"));
        break;

    case RendererAPI::OpenGL:
        return NEW_INDEX_BUFFER(OpenGLIndexBuffer, size, data);
        break;

    default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;

    #undef _ERROR
}

}
