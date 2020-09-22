#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Application.h"

namespace Dewpsi {

Scope<Input> Input::s_Instance = Input::Create();

Scope<Input> Input::Create()
{
    PD_CORE_ASSERT(! (s_Instance), "Input poller already initialized");
#ifdef PD_PLATFORM_LINUX
    return CreateScope<SDLInput>();
#else
    PD_CORE_ASSERT(false, "This platform is not supported");
    return nullptr;
#endif
}

}
