#include "Dewpsi_Application.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_WhichOS.h"
#include "Dewpsi_ImGui_SDL.h"
#include "Dewpsi_ImGui_OpenGL3.h"
#include "Dewpsi_String.h"

#include <glad/glad.h>
//#include <cstring>

namespace Dewpsi {

ImGuiLayer::ImGuiLayer(const char* path) : Layer("ImGuiLayer"), m_cpShaderPath()
{
    size_t szCopyLen = 0;
    
    m_cpShaderPath = Dewpsi::String::New(std::strlen(path)+1);
    PD_CORE_ASSERT(m_cpShaderPath, "failed to allocate string: {0}", Dewpsi::GetError());
    
    szCopyLen = Dewpsi::String::Copy(m_cpShaderPath, path, std::strlen(path)+1);
    PD_CORE_ASSERT(szCopyLen != PD_INVALID, "ERROR: ImGuiLayer constructor: failed to copy string: {0}", Dewpsi::GetError());
}

ImGuiLayer::~ImGuiLayer()
{
    delete[] m_cpShaderPath;
    m_cpShaderPath = nullptr;
}

void ImGuiLayer::OnAttach()
{
    SDLNativeWindow* const pNativeWindow = (SDLNativeWindow*) Application::Get().GetWindow().GetNativeWindow();
    
    PD_CORE_INFO("Attaching ImGuiLayer..."); // TODO: remove line
    
    // create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();
    
    // set backend flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    
    // register SDL2 OpenGL bindings
    ImGui_OpenGL_Init(m_cpShaderPath, "#version 130");
    ImGui_ImplSDL2_InitForOpenGL(pNativeWindow->window, pNativeWindow->context);
    
    PD_CORE_INFO("OpenGL/SDL2 bindings for ImGui initialized"); // TODO: remove line
}

void ImGuiLayer::OnDetach()
{
    PD_CORE_INFO("Detaching ImGuiLayer..."); // TODO: remove line
    ImGui_OpenGL_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate(Timestep delta)
{
    SDLNativeWindow* const pNativeWindow = (SDLNativeWindow*) Application::Get().GetWindow().GetNativeWindow();
    
    // start frame
    ImGui_OpenGL_NewFrame();
    ImGui_ImplSDL2_NewFrame(pNativeWindow->window);
    ImGui::NewFrame();
    
    // show demo window
    static bool State = true;
    if (State)
        ImGui::ShowDemoWindow(&State);
    
    // render gui
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_OpenGL_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& e)
{
    //
}

}
