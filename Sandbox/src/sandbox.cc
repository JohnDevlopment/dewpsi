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

static const char* VertexShaderOpenGL[2] = {
    R"(
        #version 430 core
        layout(location = 0) in vec2 in_Position;
        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        void main() {
            gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 0, 1);
        }
    )",
    ""
};

static const char* FragmentShaderOpenGL[2] = {
    R"(
        #version 430 core
        uniform vec3 u_Color;
        out vec4 FragColor;

        void main() {
            FragColor = vec4(u_Color, 1);
        }
    )",
    ""
};

static constexpr float _CamXSpeed = 1.0f;
static constexpr float _CamRotation = 90.0f;

void SandboxLayer::OnAttach()
{
    PD_PROFILE_FUNCTION();

    Dewpsi::Renderer::SetAPI(Dewpsi::RendererAPI::API::OpenGL);

    // set vertex and index buffer data
    m_ColoredQuad = Dewpsi::CreateRef<SandboxShape<16, 6>>();
    m_ColoredQuad->vertices.SetData({
       -0.5f, -0.5f, // 0 (BL)
        0.5f, -0.5f, // 1 (BR)
        0.5f,  0.5f, // 2 (TR)
       -0.5f,  0.5f  // 3 (TL)
    });
    m_ColoredQuad->indices.SetData({0, 1, 2, 2, 3, 0});

    // create colored quad
    Dewpsi::BufferLayout layout = {
        {Dewpsi::ShaderDataType::Float2, "in_Position"}
    };
    m_ColoredQuad->Init(layout);

    // color shader
    m_ColorShader = Dewpsi::Shader::Create(VertexShaderOpenGL[0], FragmentShaderOpenGL[0]);
    m_ColorShader->Bind();

    Dewpsi::static_ref_cast<Dewpsi::OpenGLShader>(m_ColorShader)->UploadUniformFloat3(
        "u_Color", m_Color[0], m_Color[1], m_Color[2]
    );

    // textured quad
    /*m_ColoredQuad->vertices.SetData({
       -0.5f, -0.5f, 0.0f, 0.0f, // 0 (BL)
        0.5f, -0.5f, 1.0f, 0.0f, // 1 (BR)
        0.5f,  0.5f, 1.0f, 1.0f, // 2 (TR)
       -0.5f,  0.5f, 0.0f, 1.0f  // 3 (TL)
    });*/

    m_ColoredQuad->vao->UnBind();
}

void SandboxLayer::OnDetach()
{
    PD_PROFILE_FUNCTION();
    m_ColoredQuad.reset();
    m_ColorShader.reset();
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
    Dewpsi::Renderer::Submit(m_ColorShader, m_ColoredQuad->vao, m_ColoredQuad->Transform());
    Dewpsi::Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
    static Dewpsi::OpenGLShader* colorShader = reinterpret_cast<Dewpsi::OpenGLShader*>(m_ColorShader.get());
    ImGui::Begin("Options");

    ImGui::ColorEdit3("Rect Color", m_Color, ImGuiColorEditFlags_NoTooltip);
    if (ImGui::Button("Set Color"))
    {
        m_ColorShader->Bind();
        colorShader->UploadUniformFloat3("u_Color", m_Color[0], m_Color[1], m_Color[2]);
        m_ColorShader->UnBind();
    }

    ImGui::End();
}

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
