#include "Dewpsi_Application.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_WhichOS.h"
#include "Dewpsi_ImGui_SDL.h"
#include "Dewpsi_ImGui_OpenGL3.h"

#include <glad/glad.h>

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
    
    PD_CORE_INFO("Attaching ImGuiLayer...");
    
//    ImGui_ImplOpenGL3_Init();
    
    // create ImGui context
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui::StyleColorsClassic();
//    
//    // set backend flags
//    ImGuiIO& io = ImGui::GetIO();
//    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
//    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
//    
//    PD_CORE_TRACE("Stopped at ImGuiLayer::OnAttach");
//    PD_DEBUGBREAK();
//    
//    // register SDL2 OpenGL bindings
//    ImGui_ImplSDL2_InitForOpenGL(pNativeWindow->window, pNativeWindow->context);
//    ImGui_ImplOpenGL3_Init(nullptr);
}

void ImGuiLayer::OnDetach()
{
    PD_CORE_TRACE("Detaching ImGuiLayer...");
//    ImGui::DestroyContext();
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplSDL2_Shutdown();
}

void ImGuiLayer::OnUpdate()
{
    SDLNativeWindow* const pNativeWindow = (SDLNativeWindow*) Application::Get().GetWindow().GetNativeWindow();
    
//    // start frame
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplSDL2_NewFrame(pNativeWindow->window);
//    ImGui::NewFrame();
//    
//    // show demo window
//    static bool State = true;
//    if (State)
//        ImGui::ShowDemoWindow(&State);
//    
//    // render gui
//    ImGuiIO& io = ImGui::GetIO();
//    ImGui::Render();
//    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& e)
{
    //
}

}
