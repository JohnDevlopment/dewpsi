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
    : m_bRunning(true), m_window(), m_fLastFrameTime(0.0f)
{
    PD_PROFILE_FUNCTION();
    
    if (! Log::IsInit())
        throw std::runtime_error("Logger has not been initialized prior to application start");
    
    s_instance = this;
    
    // initialize SDL2 video
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        PD_CORE_CRITICAL("Failed to initialize SDL, {}", SDL_GetError());
        throw std::runtime_error("SDL2 could not be initialized");
    }
    
    // create the window
    m_window = Window::Create(_WindowProperties);
    m_window->SetEventCallback(PD_BIND_EVENT_FN(Application::OnEvent));
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

void Application::Run()
{
    PD_PROFILE_FUNCTION();
    
    while (m_bRunning)
    {
        PD_PROFILE_SCOPE("RunLoop");
        float fTime = (float) SDL_GetTicks() / 1000.0f; // TODO: change to platform-independent function
        Timestep delta = fTime - m_fLastFrameTime;
        m_fLastFrameTime = fTime;
        
        // update each layer
        for (auto itr = m_layerStack.begin(); itr != m_layerStack.end(); ++itr)
            (*itr)->OnUpdate(delta);
        
        // update the window
        m_window->OnUpdate();
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
