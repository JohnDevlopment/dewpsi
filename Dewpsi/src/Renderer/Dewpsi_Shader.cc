#include "Dewpsi_Renderer.h"
#include "Dewpsi_Shader.h"
#include "Dewpsi_OpenGLShader.h"
#include "Dewpsi_OpenGL.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_String.h"

static constexpr Dewpsi::StaticString _OpenGL_VertShader = R"(
    #version 430 core
    layout(location = 0) in vec3 In_Position;
    layout(location = 1) in vec3 In_Color;
    out vec3 V_Position;
    out vec3 V_Color;
    void main() {
        V_Position = In_Position;
        V_Color = In_Color;
        gl_Position = vec4(V_Position, 1.0);
    }
)";

static constexpr Dewpsi::StaticString _OpenGL_FragShader = R"(
    #version 430 core
    out vec4 FragColor;
    in vec3 V_Position;
    in vec3 V_Color;
    void main() {
        FragColor = vec4(V_Color.xyz, 1.0);
    }
)";

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

ShaderSourceChunk GetDefaultShader(RendererAPI::API api)
{
    ShaderSourceChunk chunk;

    switch (api)
    {
        case RendererAPI::API::OpenGL:
            chunk.vertexSource = _OpenGL_VertShader.get();
            chunk.fragmentSource = _OpenGL_FragShader.get();
            break;

        default:
            throw DewpsiError("Dewpsi::GetDefaultShader: unknown API");
            break;
    }

    return chunk;
}

}
