#include "Dewpsi_Texture.h"
#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_Renderer.h"

namespace Dewpsi {

Ref<Texture2D> Texture2D::Create(const PDstring& file)
{
    #define _ERROR(msg) "Texture2D::Create: " msg

    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            throw DewpsiError(_ERROR("renderer API is set to none"));
            break;

        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTexture2D>(file);
            break;

        default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;
    #undef _ERROR
}

}
