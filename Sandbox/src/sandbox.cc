#include "sandbox.h"
#include <Dewpsi_Log.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_Application.h>
#include <Dewpsi_MouseEvent.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_Color.h>
#include <Dewpsi_String.h>
#include <Dewpsi_Vector.h>
#include <Dewpsi_ImGuiLayer.h>
#define PD_PROFILE 0
#include <Dewpsi_Debug.h>

#include <glm/gtc/type_ptr.hpp>

static constexpr float _CamXSpeed = 1.0f;
static constexpr float _CamRotation = 90.0f;

static constexpr Dewpsi::StaticString _VertShaderOpenGL = R"(
    #version 430 core
    layout(location = 0) in vec2 In_Position;
    layout(location = 1) in vec3 In_Color;
    uniform mat4 U_ViewProjection;
    out vec3 V_Color;
    void main() {
        gl_Position = U_ViewProjection * vec4(In_Position, 0.0, 1.0);
        V_Color = In_Color;
    }
)";

static constexpr Dewpsi::StaticString _FragShaderOpenGL = R"(
    #version 430 core
    in vec3 V_Color;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(V_Color, 1.0);
    }
)";

void SandboxLayer::OnAttach()
{
    PD_PROFILE_FUNCTION();

    Dewpsi::Renderer::SetAPI(Dewpsi::RendererAPI::API::OpenGL);
    m_Program.reset(Dewpsi::Shader::Create(_VertShaderOpenGL.get(), _FragShaderOpenGL.get()));
    m_VertexArray.reset(Dewpsi::VertexArray::Create());
    m_VertexArray->Bind();

    // create vertex buffer
    {
        PD_PROFILE_SCOPE("Vertex buffer");
        using Dewpsi::DefineColor;
        using Dewpsi::ShaderDataType;
        Dewpsi::FColor colors[] = {
            DefineColor(127, 0, 127),
            DefineColor(0, 127, 127),
            DefineColor(127, 127, 0),
        };
        const float faVertices[] = {
           -0.5f, -0.5f,   PD_COLOR_UNPACK3(colors[0]),
            0.5f, -0.5f,   PD_COLOR_UNPACK3(colors[1]),
            0.0f,  0.5f,   PD_COLOR_UNPACK3(colors[1])
        };
        m_VertexBuffer.reset(Dewpsi::VertexBuffer::Create(sizeof(faVertices), faVertices));

        // layout
        Dewpsi::BufferLayout layout = {
            { ShaderDataType::Float2, "In_Position" },
            { ShaderDataType::Float3, "In_Color" }
        };
        m_VertexBuffer->SetLayout(layout);
    }

    // create index buffer
    {
        PD_PROFILE_SCOPE("Index buffer");
        const PDuint uiaIndices[] = {0, 1, 2};
        m_IndexBuffer.reset(Dewpsi::IndexBuffer::Create(sizeof(uiaIndices), uiaIndices));
    }

    // supply vertex array with buffers
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    m_VertexArray->UnBind();
}

void SandboxLayer::OnDetach()
{
    PD_PROFILE_FUNCTION();
    m_VertexBuffer.reset();
    m_VertexArray.reset();
    m_Program.reset();
}

void SandboxLayer::OnUpdate(Dewpsi::Timestep delta)
{
    PD_PROFILE_FUNCTION();

    glm::vec3 position = m_Camera.GetPosition();

    if (Dewpsi::Input::IsKeyPressed(PD_KEY_LEFT))
        position.x -= _CamXSpeed * delta;
    else if (Dewpsi::Input::IsKeyPressed(PD_KEY_RIGHT))
        position.x += _CamXSpeed * delta;
    else if (Dewpsi::Input::IsKeyPressed(PD_KEY_UP))
        position.y += _CamXSpeed * delta;
    else if (Dewpsi::Input::IsKeyPressed(PD_KEY_DOWN))
        position.y -= _CamXSpeed * delta;

    m_Camera.SetPosition(position);

    {
        float fAngle = m_Camera.GetRotation();
        if (Dewpsi::Input::IsKeyPressed(PD_KEY_R))
            fAngle -= _CamRotation * delta;

        m_Camera.SetRotation(fAngle);
    }

    Dewpsi::Renderer::BeginScene(m_Camera);

    Dewpsi::Renderer::Submit(m_Program, m_VertexArray);
    m_VertexArray->UnBind();

    Dewpsi::Renderer::EndScene();
}

//void SandboxLayer::OnImGuiRender()
//{}

void SandboxLayer::OnEvent(Dewpsi::Event& e)
{
    //Dewpsi::EventDispatcher dispatcher(e);
    //dispatcher.Dispatch<Dewpsi::KeyPressedEvent>(PD_BIND_EVENT_FN(SandboxLayer::OnKeyboardPressed));
}

// sandbox app
Sandbox::Sandbox(PDuserdata userdata)
{
    PD_PROFILE_FUNCTION();

    m_UserData = userdata;
    SandboxData* data = (SandboxData*) userdata;
    PD_ASSERT(data, "NULL \"data\" pointer");
    data->sizeRatio = (data->sizeRatio / 10.0f) + 1.0f;
    PushLayer(new SandboxLayer(data));

    PD_INFO("Window ratio is {0}", data->sizeRatio);
}
