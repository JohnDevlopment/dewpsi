#include "Dewpsi_Renderer.h"
#include "Dewpsi_Shader.h"
#include "Dewpsi_OpenGLShader.h"
#include "Dewpsi_OpenGL.h"
#include "Dewpsi_Except.h"

#define NEW_SHADER(type, ...) static_cast<Shader*>(new type(__VA_ARGS__));

namespace Dewpsi {

Shader* Shader::Create(const std::string& vertSrc, const std::string& fragSrc)
{
    #define _ERROR(msg) "Shader::CreateShader: " msg

    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        throw DewpsiError(_ERROR("Renderer API is set to none"));
        break;

    case RendererAPI::API::OpenGL:
        return NEW_SHADER(OpenGLShader, vertSrc, fragSrc);
        break;

    default: break;
    }

    throw DewpsiError(_ERROR("unrecognized API"));

    return nullptr;
    #undef _ERROR
}

}
