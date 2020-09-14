#include "Dewpsi_WhichOS.h"
#include "Dewpsi_VertexArray.h"
#include "Dewpsi_Renderer.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_Memory.h"

#define NEW_VERTEX_ARRAY(type) static_cast<VertexArray*>(new type());

namespace Dewpsi {

Ref<VertexArray> VertexArray::Create()
{
    #define _ERROR(msg) "VertexArray::Create: " msg

    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        throw DewpsiError(_ERROR("Renderer API is set to none"));
        break;

    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexArray>();
        break;

    default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;

    #undef _ERROR
}

}
