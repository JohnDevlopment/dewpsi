#define _PD_DEBUG_DEFINE_VARS
#include "Dewpsi_Application.h"
#include "Dewpsi_Platform.h"
#include "Dewpsi_Debug.h"
#include "Dewpsi_Window.h"
#include "Dewpsi_ApplicationEvent.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_String.h"

#include <SDL.h>
#include <stdexcept>
#include "Dewpsi_OpenGL.h"
#include "Dewpsi_Shader.h"

static constexpr Dewpsi::StaticString _VertShader = R"(
    #version 430 core
    layout(location = 0) in vec3 In_Position;
    out vec3 V_Position;
    void main() {
        V_Position = vec3(In_Position.x, In_Position.y + 0.5, In_Position.z);
        gl_Position = vec4(V_Position, 1.0);
    }
)";

static constexpr Dewpsi::StaticString _FragShader = R"(
    #version 430 core
    layout(location = 0) out vec4 FragColor;
    in vec3 V_Position;
    void main() {
        //FragColor = vec4(0.0, 0.5, 0.2, 1.0);
        FragColor = vec4(V_Position * 0.5 + 0.5, 1.0);
    }
)";

static PDuint _VAO, _VBO, _IBO;

static Dewpsi::Scope<Dewpsi::Shader> _Program;

namespace Dewpsi {

static WindowProps _WindowProperties;

Application* Application::s_instance = nullptr;

Application::Application(const std::string& sName)
    : m_bRunning(true), m_window(), m_guiLayer(), m_fLastFrameTime(0.0f),
      m_UserData(nullptr)
{
    PD_PROFILE_FUNCTION();

    if (! Log::IsInit())
        throw std::runtime_error("Logger has not been initialized prior to application start");

    s_instance = this;

    // create the window
    m_window = Window::Create(_WindowProperties);
    m_window->SetEventCallback(PD_BIND_EVENT_FN(Application::OnEvent));

    // push ImGui layer
    m_guiLayer = new ImGuiLayer(m_UserData);
    PushOverlay(m_guiLayer);

    // TODO: remove this section
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    float faVerticies[9] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(faVerticies), faVerticies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glGenBuffers(1, &_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);

    const PDuint uiaIndices[] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uiaIndices), uiaIndices, GL_STATIC_DRAW);

    using Dewpsi::Shader;
    _Program = Dewpsi::CreateScope<Shader>(_VertShader.get(), _FragShader.get());
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

        // TODO: this window-clear function should be moved to a renderer API
        m_window->Clear();

        // TODO: remove this later
        _Program->Bind();

        // TODO: remove the next couple lines
        glBindVertexArray(_VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

        // TODO: remove this later
        _Program->UnBind();
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
