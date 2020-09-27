#include "Dewpsi_Application.h"
#include "Dewpsi_Platform.h"
#include "Dewpsi_Window.h"
#include "Dewpsi_ApplicationEvent.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_String.h"
#include "Dewpsi_Renderer.h"

// These includes are just to get make to update them
#include "Dewpsi_Rect.h"

#ifdef PD_DEBUG
#include "Dewpsi_Debug.h"
#endif

#include <SDL.h>
#include <stdexcept>

using Dewpsi::Scope;
using Dewpsi::Ref;

namespace Dewpsi {

static WindowProps _WindowProperties;

Application* Application::s_instance = nullptr;

Application::Application(const std::string& sName)
    : m_bRunning(true), m_window(), m_guiLayer(), m_fLastFrameTime(0.0f),
      m_UserData(nullptr)
{
    if (! Log::IsInit())
        throw std::runtime_error("Logger has not been initialized prior to application start");

    s_instance = this;

    // create the window
    m_window = Window::Create(_WindowProperties);
    m_window->SetEventCallback(PD_BIND_EVENT_FN(Application::OnEvent));

    // set the clear color
    RenderCommand::SetClearColor(DefineColor(127, 127, 127));

    // push ImGui layer
    m_guiLayer = new ImGuiLayer(m_UserData);
    PushOverlay(m_guiLayer);
}

Application::~Application()
{
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
    while (m_bRunning)
    {
        float fTime = Platform::GetTime();
        Timestep delta = fTime - m_fLastFrameTime;
        m_fLastFrameTime = fTime;

        // clear buffers
        RenderCommand::Clear();

        // update each layer
        for (auto itr = m_layerStack.begin(); itr != m_layerStack.end(); ++itr)
            (*itr)->OnUpdate(delta);

        // render ImGui on all the layers
        m_guiLayer->Begin();
        for (auto itr = m_layerStack.begin(); itr != m_layerStack.end(); ++itr)
            (*itr)->OnImGuiRender();
        m_guiLayer->End();

        // update the window
        m_window->OnUpdate();
    }
}

bool Application::OnWindowClosed(WindowCloseEvent& e)
{
    // FIXME: #1 Event not detected on Release builds

    /*  It's possible for there to be multiple windows to be closed,
        so the application must know whether it's the main window being
        closed. As such, check the ID of the window being closed to
        to make sure that it's the main window being closed. */
    if (e.GetWindowID() == m_window->GetWindowID())
    {
        // main window
        if (! m_bRunning)
            return true;
#ifdef PD_DEBUG
        PD_CORE_TRACE("Requested window event close for main window {0}", e.GetWindowID());
#endif
        m_bRunning = false;
    }
    else
    {
#ifdef PD_DEBUG
        PD_CORE_TRACE("Requested window event close for window {0}", e.GetWindowID());
#endif
    }

    return true;
}

bool Application::OnWindowResized(WindowResizeEvent& e)
{
    _WindowProperties.width = e.GetWidth();
    _WindowProperties.height = e.GetHeight();

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
