#include "Dewpsi_Renderer.h"
#include "Dewpsi_Shader.h"
#include "Dewpsi_Except.h"
//#include "Dewpsi_String.h"

#include "Dewpsi_OpenGLShader.h"
#include "Dewpsi_OpenGL.h"

namespace Dewpsi {

Ref<Shader> Shader::Create(const PDstring& vertSrc, const PDstring& fragSrc)
{
    #define _ERROR(msg) "Shader::Shader: " msg

    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            throw DewpsiError(_ERROR("Renderer API is set to none"));
            break;

        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLShader>(vertSrc, fragSrc);
            break;

        default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;
    #undef _ERROR
}

Ref<Shader> Shader::Create(const PDstring& file)
{
    #define _ERROR(msg) "Shader::Shader: " msg

    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            throw DewpsiError(_ERROR("Renderer API is set to none"));
            break;

        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLShader>(file);
            break;

        default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;
    #undef _ERROR
}

}
