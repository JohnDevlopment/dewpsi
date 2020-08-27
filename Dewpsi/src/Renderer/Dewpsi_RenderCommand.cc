#include "Dewpsi_WhichOS.h"
#include "Dewpsi_RenderCommand.h"

namespace Dewpsi {

RendererAPI* RenderCommand::s_RenderingAPI = new OpenGLRendererAPI;

}
