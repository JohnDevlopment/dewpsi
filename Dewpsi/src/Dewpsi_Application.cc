//#define _PD_DEBUG_DEFINE_VARS
#include "Dewpsi_Application.h"
#include "Dewpsi_Platform.h"
#include "Dewpsi_Debug.h"
#include "Dewpsi_Window.h"
#include "Dewpsi_ApplicationEvent.h"
#include "Dewpsi_ImGuiLayer.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_String.h"
#include "Dewpsi_WhichOS.h"

#include <SDL.h>
#include <stdexcept>
#include "Dewpsi_OpenGL.h"
#include "Dewpsi_Shader.h"

using Dewpsi::Scope;

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

static Scope<Dewpsi::Shader> _Program;
static Scope<Dewpsi::VertexArrayBuffer> _VAO;
static Scope<Dewpsi::VertexBuffer> _VBO;
static Scope<Dewpsi::IndexBuffer> _IBO;

static GLenum ShaderType2OpenGLEnum(Dewpsi::ShaderDataType type)
{
    using Dewpsi::ShaderDataType;

    switch (type)
    {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
    }

    return (GLenum) 0;
}

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
    _VAO.reset(VertexArrayBuffer::Create());

    float faVerticies[] = {
        -0.5f, -0.5f, 0.0f, 0.2f, 0 , 0.3f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.5f , 0.3f,
        0.0f, 0.5f, 0.0f, 0.2f, 0 , 0.3f
    };

    _VBO.reset(VertexBuffer::Create(sizeof(faVerticies), faVerticies));

    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "In_Position" },
            { ShaderDataType::Float3, "In_Color" }
        };
        _VBO->SetLayout(layout);
    }

    {
        PDuint32 uiOffset = 0;
        const BufferLayout& layout = _VBO->GetLayout();

        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(uiOffset);
            glVertexAttribPointer(uiOffset, element.GetComponentCount(),
                                  ShaderType2OpenGLEnum(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                                  (void*) element.offset);
            ++uiOffset;
        }
    }

    const PDuint uiaIndices[] = { 0, 1, 2 };
    _IBO.reset(IndexBuffer::Create(PD_ARRAYSIZE(uiaIndices), uiaIndices));

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
        glDrawElements(GL_TRIANGLES, _IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
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
