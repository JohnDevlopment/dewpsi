#include "Dewpsi_Application.h"
#include "Dewpsi_Platform.h"
#include "Dewpsi_Debug.h"
#include "Dewpsi_Window.h"
#include "Dewpsi_ApplicationEvent.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_String.h"
#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Color.h"

#include <SDL.h>
#include <stdexcept>

using Dewpsi::Scope;
using Dewpsi::Ref;

static constexpr Dewpsi::StaticString _VertShader = R"(
    #version 430 core
    layout(location = 0) in vec3 In_Position;
    layout(location = 1) in vec3 In_Color;
    out vec3 V_Position;
    out vec3 V_Color;
    void main() {
        V_Position = In_Position;
        V_Color = In_Color;
        gl_Position = vec4(V_Position, 1.0);
    }
)";

static constexpr Dewpsi::StaticString _FragShader = R"(
    #version 430 core
    out vec4 FragColor;
    in vec3 V_Position;
    in vec3 V_Color;
    void main() {
        FragColor = vec4(V_Color.xyz, 1.0);
    }
)";

// TODO: Remove these
static Ref<Dewpsi::Shader> _Program;
static Ref<Dewpsi::VertexArray> _VAO;

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
    _VAO.reset(VertexArray::Create());

    {
            // create vertex buffer and associate with vertex array
        _VAO->Bind();
        FColor cs[] = {
            DefineColor(239, 41, 41),
            DefineColor(239, 173, 41),
            DefineColor(0, 157, 40),
            DefineColor(157, 157, 40)
        };
        /*
        const float faVerticies[] = {
           -0.5f, -0.5f, 0.0f,   cs[0].red, cs[0].green, cs[0].blue,
            0.5f, -0.5f, 0.0f,   cs[1].red, cs[1].green, cs[1].blue,
            0.0f,  0.5f, 0.0f,   cs[2].red, cs[2].green, cs[2].blue,
           -0.5f,  0.5f, 0.0f,   cs[2].red, cs[2].green, cs[2].blue,
        };
        */
        const float faVerticies[] = {
           -0.75f, -0.5f, 0.0f,   cs[0].red, cs[0].green, cs[0].blue,
            0.75f, -0.5f, 0.0f,   cs[1].red, cs[1].green, cs[1].blue,
            0.75f,  0.5f, 0.0f,   cs[2].red, cs[2].green, cs[2].blue,
           -0.75f,  0.5f, 0.0f,   cs[3].red, cs[3].green, cs[3].blue,
        };
        Ref<VertexBuffer> vbo(VertexBuffer::Create(sizeof(faVerticies), faVerticies));

            // set layout
        BufferLayout layout = {
            { ShaderDataType::Float3, "In_Position" },
            { ShaderDataType::Float3, "In_Color" }
        };
        vbo->SetLayout(layout);
        _VAO->AddVertexBuffer(vbo);
    }

    {
            // create index buffer
        const PDuint uiaIndices[] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> ibo(IndexBuffer::Create(PD_ARRAYSIZE(uiaIndices), uiaIndices));
        _VAO->SetIndexBuffer(ibo);
        _VAO->UnBind();
    }

        // create shader
    _Program.reset(Shader::Create(_VertShader.get(), _FragShader.get()));
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

        // TODO: remove this section
        _Program->Bind();
        _VAO->Bind();
        glDrawElements(GL_TRIANGLES,
                      _VAO->GetIndexBuffer()->GetCount(),
                      GL_UNSIGNED_INT, nullptr);
        ///////////////////////////////////

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

        // TODO: remove this section
        _Program->UnBind();
        _VAO->UnBind();
        //////////////////////////////////
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
