#include "Dewpsi_Application.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_WhichOS.h"
#include "Dewpsi_ImGui_SDL.h"

namespace Dewpsi {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
{
    //
}

ImGuiLayer::~ImGuiLayer()
{
    //
}

void ImGuiLayer::OnAttach()
{
    SDLNativeWindow* const pNativeWindow = (SDLNativeWindow*) Application::Get().GetWindow().GetNativeWindow();
    
    // register SDL2 OpenGL bindings
    ImGui_ImplSDL2_InitForOpenGL(pNativeWindow->window, pNativeWindow->context);
    
    // create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    // set backend flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
}

void ImGuiLayer::OnDetach()
{
    //
}

void ImGuiLayer::OnUpdate()
{
    SDLNativeWindow* const pNativeWindow = (SDLNativeWindow*) Application::Get().GetWindow().GetNativeWindow();
    
    // start frame
    ImGui_ImplSDL2_NewFrame(pNativeWindow->window);
    ImGui::NewFrame();
    
    // show demo window
    static bool State = true;
    ImGui::ShowDemoWindow(&State);
    
    // render gui
    ImGui::Render();
}

void ImGuiLayer::OnEvent(Event& e)
{
    //
}

}
