#include "Dewpsi_Application.h"
#include "Dewpsi_Application.h"
#include "Dewpsi_Debug.h"
#include "Dewpsi_Window.h"
#include "Dewpsi_ApplicationEvent.h"
#include "Dewpsi_ImGuiLayer.h"

#include <SDL.h>
#include <stdexcept>

namespace Dewpsi {

static WindowProps _WindowProperties;

Application* Application::s_instance = nullptr;

Application::Application(const std::string& sName)
    : m_bRunning(true), m_window()
{
    PD_PROFILE_FUNCTION();
    
    if (! Log::IsInit())
    {
        throw std::runtime_error("Logger has not been initialized prior to application start");
    }
    
    s_instance = this;
    
    // initialize SDL2 video
    PD_CORE_INFO("Initializing SDL2 library with SDL_Init(), flags: SDL_INIT_VIDEO");
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        PD_CORE_CRITICAL("Failed to initialize SDL, {}", SDL_GetError());
        throw std::runtime_error("SDL2 could not be initialized");
    }
    
    // create the window
    m_window = Window::Create(_WindowProperties);
    m_window->SetEventCallback(PD_BIND_EVENT_FN(Application::OnEvent));
    
    // push the ImGui layer
//    PushOverlay(new ImGuiLayer()); // TODO: reenable
}

Application::~Application()
{
    PD_PROFILE_FUNCTION();
    SDL_Quit();
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(PD_BIND_EVENT_FN(Application::OnWindowClosed));
    
    for (auto itr = m_layerStack.rbegin(); itr != m_layerStack.rend(); ++itr)
    {
        if (e.m_handled)
            break;
        (*itr)->OnEvent(e);
    }
}

void Application::PushLayer(Layer* layer)
{
    m_layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* overlay)
{
    m_layerStack.PushOverlay(overlay);
}

void Application::UpdateLayers()
{
    for (auto itr = m_layerStack.rbegin(); itr != m_layerStack.rend(); ++itr)
        (*itr)->OnUpdate();
}

void Application::Run()
{
    PD_PROFILE_FUNCTION();
    
    while (m_bRunning)
    {
        PD_PROFILE_SCOPE("RunLoop");
        m_window->Update();
    }
}

bool Application::OnWindowClosed(WindowCloseEvent& e)
{
    m_bRunning = false;
    return true;
}

// =================================================

void SetWindowProps(const WindowProps& props)
{
    _WindowProperties = props;
}

}
