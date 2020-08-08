#include "Dewpsi_Application.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_WhichOS.h"
#include "Dewpsi_ImGui_SDL.h"
#include "Dewpsi_ImGui_OpenGL3.h"
#include "Dewpsi_String.h"
#include "Dewpsi_Platform.h"
#include "Dewpsi_Except.h"

#include <glad/glad.h>

#define g_TextInput1_Size   16

//static std::unique_ptr<char[]> g_TextInput1(new char[g_TextInput1_Size]);

static char g_TextInput1[g_TextInput1_Size];

static void ShowMainWindow();

namespace Dewpsi {

ImGuiLayer::ImGuiLayer(const void* data) : Layer("ImGuiLayer"), m_vpData(data)
{  }

ImGuiLayer::~ImGuiLayer()
{
    m_vpData = nullptr;
}

void ImGuiLayer::OnAttach()
{
    // create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // register SDL2 OpenGL bindings
    PDenum eResult = Platform::InitImGui(m_vpData);

    if (eResult == PD_INVALID)
    {
        PD_CORE_ERROR("Failed to initialize ImGui: {0}", GetError());
        DewpsiError e;
        throw e;
    }
}

void ImGuiLayer::OnDetach()
{
    Platform::QuitImGui();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate(Timestep delta)
{
    ImGuiIO& io = ImGui::GetIO();
    Window& win = Application::Get().GetWindow();

    // start frame
    ImGui_OpenGL_NewFrame();
    ImGui_ImplSDL2_NewFrame(
        reinterpret_cast<SDLNativeWindow*>(Application::Get().GetWindow().GetNativeWindow())->window
    );
    ImGui::NewFrame();

    // show widgets
    ImGui::Begin("Test Window");
    ShowMainWindow();
    ImGui::End();

    // finalize and render commands
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    Application::Get().GetWindow().Clear();
    ImGui_OpenGL_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseMovedEvent>(PD_BIND_EVENT_FN(ImGuiLayer::OnMouseMotion));
    dispatcher.Dispatch<MousePressedEvent>(PD_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<MouseReleasedEvent>(PD_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleased));
    dispatcher.Dispatch<WindowResizeEvent>(PD_BIND_EVENT_FN(ImGuiLayer::OnWindowResized));
    dispatcher.Dispatch<KeyTypedEvent>(PD_BIND_EVENT_FN(ImGuiLayer::OnKeyTyped));
    dispatcher.Dispatch<KeyPressedEvent>(PD_BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));
    dispatcher.Dispatch<KeyReleasedEvent>(PD_BIND_EVENT_FN(ImGuiLayer::OnKeyReleased));
}

bool ImGuiLayer::OnMouseMotion(MouseMovedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(e.GetX(), e.GetY());
}

bool ImGuiLayer::OnMouseButtonPressed(MousePressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[(int)e.GetMouseCode()] = true;
    PD_CORE_TRACE("io.MouseDown[{0}] = true", (int)e.GetMouseCode());

    auto pair = Input::GetMousePosition();
    io.MousePos = ImVec2(pair.first, pair.second);
    PD_CORE_TRACE("io.MousePos = ({0},{1})", io.MousePos.x, io.MousePos.y);

    return false;
}

bool ImGuiLayer::OnMouseButtonReleased(MouseReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[(int)e.GetMouseCode()] = false;
    PD_CORE_TRACE("io.MouseDown[{0}] = false", (int)e.GetMouseCode());

    return false;
}

bool ImGuiLayer::OnWindowResized(WindowResizeEvent& e)
{
    Window& win = Application::Get().GetWindow();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_UpdateIntState(PD_GET_SDL_WINDOW(&win)); // TODO: platform-indepedent window

    return false;
}

bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharactersUTF8(e.GetText());

    return true;
}

bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    KeyCode key = e.GetKeyCode();
    io.KeysDown[(int)key] = true;

    return false;
}

bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    KeyCode key = e.GetKeyCode();
    io.KeysDown[(int)key] = false;

    return false;
}

}

void ShowMainWindow()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::InputText("default", g_TextInput1, g_TextInput1_Size);
}
