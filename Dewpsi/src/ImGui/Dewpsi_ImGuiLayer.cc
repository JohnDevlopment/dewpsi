#include "Dewpsi_Application.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_WhichOS.h"
#include "Dewpsi_ImGui_SDL.h"
#include "Dewpsi_ImGui_OpenGL3.h"
#include "Dewpsi_String.h"
#include "Dewpsi_Platform.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_Input.h"
#define _PD_DEBUG_BREAKS
#include "Dewpsi_Debug.h" // TODO: delete

namespace Dewpsi {

ImGuiLayer::ImGuiLayer(const void* data) : Layer("ImGuiLayer"), m_vpData(data),
    m_Window(nullptr), m_Context(nullptr), m_Init(false)
{
    if (! data)
        PD_CORE_WARN("ImGuiLayer: user data is NULL");
}

ImGuiLayer::~ImGuiLayer()
{
    // all fields should be NULL after this
    String::MemSet(this, 0, sizeof(ImGuiLayer));
}

void ImGuiLayer::OnAttach()
{
    // get the native platform window
    {
        Window& win = Application::Get().GetWindow();
        m_Window  = (SDL_Window*) win.GetNativeWindow();
        m_Context = SDL_GL_GetCurrentContext();
    }

    PD_CORE_ASSERT(!m_Init, "ImGui layer already initialized");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    {
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
    }

    // Setup Platform/Renderer bindings
    ImGuiInitData* const data = (ImGuiInitData*) m_vpData;

    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_Context);
    if (data)
    {
        if (! data->glslVersion.empty())
            ImGui_ImplOpenGL3_Init(data->glslVersion.c_str());
        else
            ImGui_ImplOpenGL3_Init(nullptr);
    }
    else
        ImGui_ImplOpenGL3_Init(nullptr);

        m_Init = true;
}

void ImGuiLayer::OnDetach()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnImGuiRender()
{
    ImGui::Begin("Example Window");
    if (ImGui::Button("Click me"))
        PD_CORE_INFO("ImGui layer button pressed");
    ImGui::End();
}

void ImGuiLayer::Begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_Window);
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    ImGuiIO& io = ImGui::GetIO();

    // set display size
    {
        Window& win = Application::Get().GetWindow();
        io.DisplaySize = ImVec2((float) win.GetWidth(), (float) win.GetHeight());
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

}
