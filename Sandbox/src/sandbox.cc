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

void SandboxLayer::OnAttach()
{
    PD_PROFILE_FUNCTION();

    // shader
    Dewpsi::Renderer::SetAPI(Dewpsi::RendererAPI::API::OpenGL);
    //m_Program = Dewpsi::Shader::Create(_VertShaderOpenGL.get(), _FragShaderOpenGL.get());
    m_Program = Dewpsi::Shader::Create("Sandbox/assets/shaders/opengl.glsl");
    m_Program->Bind();

    /*Dewpsi::static_ref_cast<Dewpsi::OpenGLShader>(m_Program)->UploadUniformFloat4(
        "U_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]
    );*/

    // vertex array
    m_VertexArray = Dewpsi::VertexArray::Create();
    m_VertexArray->Bind();

    {
        PD_PROFILE_SCOPE("Vertex/index buffer");
        using Dewpsi::ShaderDataType;

        // vertex buffer
        const float faVertices[] = {
           -0.5f, -0.5f, 0.0f, 0.0f, // 0 (BL)
            0.5f, -0.5f, 1.0f, 0.0f, // 1 (BR)
            0.5f,  0.5f, 1.0f, 1.0f, // 2 (TR)
           -0.5f,  0.5f, 0.0f, 1.0f  // 3 (TL)
        };
        m_VertexBuffer = Dewpsi::VertexBuffer::Create(sizeof(faVertices), faVertices);
        Dewpsi::BufferLayout layout = {
            { ShaderDataType::Float2, "In_Position" },
            { ShaderDataType::Float2, "In_TexCoord" }
        };
        m_VertexBuffer->SetLayout(layout);

        // index buffer
        const PDuint uiaIndices[] = {0, 1, 2, 2, 3, 0};
        m_IndexBuffer = Dewpsi::IndexBuffer::Create(PD_ARRAYSIZE(uiaIndices), uiaIndices);

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    // texture
    m_Texture = Dewpsi::Texture::CreateTexture("Sandbox/assets/proj.png");
    m_Texture->Bind(0);
    Dewpsi::static_ref_cast<Dewpsi::OpenGLShader>(m_Program)->UploadUniformInt1("U_Texture", 0);

    m_VertexArray->UnBind();
    m_Program->UnBind();
}

void SandboxLayer::OnDetach()
{
    PD_PROFILE_FUNCTION();
    m_Texture.reset();
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

    m_Texture->Bind();

    Dewpsi::Renderer::BeginScene(m_Camera);
    Dewpsi::Renderer::Submit(m_Program, m_VertexArray);
    Dewpsi::Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
    /*ImGui::Begin("Options");

    ImGui::ColorEdit3("Rect Color", m_Color, ImGuiColorEditFlags_NoTooltip);
    if (ImGui::Button("Set Color"))
    {
        Dewpsi::static_ref_cast<Dewpsi::OpenGLShader>(m_Program)->UploadUniformFloat4(
            "U_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]
        );
    }

    ImGui::InputInt("Active Texture", &m_ActiveTexture);
    if (ImGui::Button("Set Active Texture"))
    {
        m_Texture->Bind(m_ActiveTexture);
        Dewpsi::static_ref_cast<Dewpsi::OpenGLShader>(m_Program)->UploadUniformInt1(
            "U_Texture", m_ActiveTexture
        );
    }

    ImGui::End();*/
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
