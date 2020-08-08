#include "Dewpsi_Platform.h"
#include "Dewpsi_Application.h"

static Dewpsi::ImGuiInitData _Data;

namespace Dewpsi {

namespace Platform {

PDfloat GetTime()
{
    return (float) SDL_GetTicks() / 1000.0f;
}

PDenum InitImGui(const void* data)
{
    bool bInit;
    ImGuiInitData* const pData = (ImGuiInitData*) data;
    SDLNativeWindow* const pNativeWindow = (SDLNativeWindow*) Application::Get().GetWindow().GetNativeWindow();
    
    // initialize ImGui OpenGL renderer
    bInit = ImGui_OpenGL_Init(pData->glslPath.c_str(), pData->glslVersion.c_str());
    if (! bInit)
    {
        PD_CORE_ERROR("Failed to initialize ImGui: {0}", Dewpsi::GetError());
        return PD_INVALID;
    }
    
    // SDL2 bindings
    ImGui_ImplSDL2_InitForOpenGL(pNativeWindow->window, pNativeWindow->context);
    
    return PD_OKAY;
}

void QuitImGui()
{
    ImGui_OpenGL_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}

void* GetImGuiInitData()
{
    return &_Data;
}

}

}
