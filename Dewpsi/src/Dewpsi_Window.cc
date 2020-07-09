#include "Dewpsi_SDLWindow.h"

namespace Dewpsi {

Scope<Dewpsi::Window> Window::Create(const WindowProps& props)
{
#ifdef PD_PLATFORM_LINUX
    return CreateScope<SDL2Window>(props);
#else
    #error Currently only Linux is supported
#endif
}

}

