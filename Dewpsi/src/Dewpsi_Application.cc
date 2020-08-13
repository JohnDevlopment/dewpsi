#define _PD_DEBUG_DEFINE_VARS
#include "Dewpsi_Application.h"
#include "Dewpsi_Platform.h"
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
    : m_bRunning(true), m_window(), m_fLastFrameTime(0.0f),
      m_UserData(nullptr)
{
    PD_PROFILE_FUNCTION();

    if (! Log::IsInit())
        throw std::runtime_error("Logger has not been initialized prior to application start");

    s_instance = this;

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
    if (_WindowProperties.flags & WindowFlags::WindowResizable)
        dispatcher.Dispatch<WindowResizeEvent>(PD_BIND_EVENT_FN(Application::OnWindowResized));

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
        float fTime = Platform::GetTime();
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

bool Application::OnWindowResized(WindowResizeEvent& e)
{
    _WindowProperties.width = e.GetWidth();
    _WindowProperties.height = e.GetHeight();
    PD_CORE_INFO("Window resized to {0}x{1}", _WindowProperties.width, _WindowProperties.height); // TODO: remove (PD_CORE_INFO)

    return false;
}

// =================================================

void SetWindowProps(const WindowProps& props)
{
    _WindowProperties = props;
}

WindowProps GetWindowProperties()
{
    return PD_MOVE(_WindowProperties);
}

}
