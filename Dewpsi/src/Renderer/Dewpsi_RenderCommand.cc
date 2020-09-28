#include "Dewpsi_WhichOS.h"
#include "Dewpsi_RenderCommand.h"
#include "Dewpsi_Except.h"

namespace Dewpsi {

RendererAPI* RenderCommand::s_RenderingAPI = nullptr;

void RenderCommand::Init()
{
    PD_CORE_ASSERT(s_RenderingAPI == nullptr, "Already initialized rendering API");

    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::OpenGL:
            s_RenderingAPI = new OpenGLRendererAPI;
            break;

        default:
            throw DewpsiError("Unrecognized API");
    }

    s_RenderingAPI->Init();
}

}
