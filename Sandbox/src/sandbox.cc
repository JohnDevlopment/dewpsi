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
#include <Dewpsi_Except.h>

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
    R"(
        #version 430 core
        layout(location = 0) in vec2 in_Position;
        layout(location = 1) in vec2 in_TexCoord;
        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;
        out vec2 v_TexCoord;

        void main() {
            v_TexCoord =  in_TexCoord;
            gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 0, 1);
        }
    )"
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
    R"(
        #version 430 core
        in vec2 v_TexCoord;
        uniform sampler2D u_Texture;
        out vec4 FragColor;

        void main() {
            FragColor = texture(u_Texture, v_TexCoord.st);
        }
    )"
};

static constexpr float _CamXSpeed = 1.0f;
static constexpr float _CamRotation = 90.0f;

struct IntValue {
    int value;

    IntValue() : value(0) {}

    IntValue(int i) : value(i) {}

    IntValue(const IntValue& src) : value(src.value) {}

    IntValue(IntValue&& src) : value(src.value) {}

    IntValue& operator=(const IntValue& rhs)
    {
        value = rhs.value;
        return *this;
    }

    IntValue& operator=(IntValue&& rhs)
    {
        value = rhs.value;
        return *this;
    }
};

void SandboxLayer::OnAttach()
{
    PD_PROFILE_FUNCTION();

    // set vertex and index buffer data
    m_ColoredQuad = Dewpsi::CreateRef<SandboxShape>();
    m_ColoredQuad->vertices.SetData({
       -0.5f, -0.5f, // 0 (BL)
        0.5f, -0.5f, // 1 (BR)
        0.5f,  0.5f, // 2 (TR)
       -0.5f,  0.5f  // 3 (TL)
    });
    m_ColoredQuad->indices.SetData({0, 1, 2, 2, 3, 0});
    m_ColoredQuad->Init({{Dewpsi::ShaderDataType::Float2, "in_Position"}});

    // color shader
    m_ColorShader = Dewpsi::Shader::Create(VertexShaderOpenGL[0], FragmentShaderOpenGL[0]);
    m_ColorShader->Bind();
    m_ColorShader->SetFloat3("u_Color", m_Color[0], m_Color[1], m_Color[2]);

    // texture shader
    m_TextureShader = Dewpsi::Shader::Create(VertexShaderOpenGL[1], FragmentShaderOpenGL[1]);
    m_TextureShader->Bind();

    // textured quad
    m_TexturedQuad = Dewpsi::CreateRef<SandboxShape>();
    m_TexturedQuad->vertices.SetData({
       -0.5f, -0.5f, 0.0f, 0.0f, // 0 (BL)
        0.5f, -0.5f, 1.0f, 0.0f, // 1 (BR)
        0.5f,  0.5f, 1.0f, 1.0f, // 2 (TR)
       -0.5f,  0.5f, 0.0f, 1.0f  // 3 (TL)
    });
    m_TexturedQuad->indices.SetData({0, 1, 2, 2, 3, 0});

    m_TexturedQuad->Init({
        {Dewpsi::ShaderDataType::Float2, "in_Position"},
        {Dewpsi::ShaderDataType::Float2, "in_TexCoord"}
    });

    // First texture: DM logo
    m_Texture1 = Dewpsi::Texture2D::Create("Sandbox/assets/images/dm.png");
    if (m_Texture1->IsError())
        PD_ERROR("Failed to load Sandbox/assets/images/dm.png: {0}", Dewpsi::GetError());

    {
        PDstring filename = "Sandbox/assets/images/";
        const char* files[] = {
            "checkerboard.png",
            "checkerboard-alpha.png",
            "smile-187x200.bmp"
        };
        int iChoice = 0;
        PD_TRACE("The 'choice' index is {0}, the file is {1}", iChoice, files[iChoice]);
        m_Texture2 = Dewpsi::Texture2D::Create(filename + files[iChoice]);
    }

    if (m_Texture2->IsError())
        PD_ERROR("Failed to load Sandbox/assets/images/checkerboard.png: {0}", Dewpsi::GetError());

    // Set initial positions
    m_ColoredQuad->position = glm::vec3(-0.5f, 0.5f, 0.0f);
    m_TexturedQuad->position = glm::vec3(0.5f, 0.5f, 0.0f);
}

void SandboxLayer::OnDetach()
{
    PD_PROFILE_FUNCTION();
    m_ColoredQuad.reset();
    m_TexturedQuad.reset();

    m_ColorShader.reset();
    m_TextureShader.reset();

    m_Texture1.reset();
    m_Texture2.reset();
}

void SandboxLayer::OnUpdate(Dewpsi::Timestep delta)
{
    PD_PROFILE_FUNCTION();

    glm::vec3 position = m_Camera.GetPosition();
    Dewpsi::KeyMod mod = Dewpsi::Input::GetModState();

    if (Dewpsi::Input::IsKeyPressed(PD_KEY_LEFT))
        position.x -= _CamXSpeed * delta;
    else if (Dewpsi::Input::IsKeyPressed(PD_KEY_RIGHT))
        position.x += _CamXSpeed * delta;
    else if (Dewpsi::Input::IsKeyPressed(PD_KEY_UP))
        position.y += _CamXSpeed * delta;
    else if (Dewpsi::Input::IsKeyPressed(PD_KEY_DOWN))
        position.y -= _CamXSpeed * delta;
    else if (Dewpsi::Input::IsKeyPressed(PD_KEY_L))
    {
        if (mod & PD_MOD_SHIFT)
        {
            position.x = 0.0f;
            position.y = 0.0f;
            position.z = 0.0f;
        }
    }

    m_Camera.SetPosition(position);

    {
        float fAngle = m_Camera.GetRotation();
        if (Dewpsi::Input::IsKeyPressed(PD_KEY_R))
        {
            if (mod & PD_MOD_SHIFT)
                fAngle = 0.0f;
            else
                fAngle -= _CamRotation * delta;
        }

        m_Camera.SetRotation(fAngle);
    }

    using Dewpsi::Renderer;
    Renderer::BeginScene(m_Camera);

    // Draw solid-color square
    Renderer::Submit(m_ColorShader, m_ColoredQuad->vao, m_ColoredQuad->Transform());

    // Bind texture and draw it
    m_Texture1->Bind(0);
    m_TextureShader->SetInt1("u_Texture", 0);
    Renderer::Submit(m_TextureShader, m_TexturedQuad->vao, m_TexturedQuad->Transform());

    // Bind other texture and draw it
    m_Texture2->Bind(1);
    m_TextureShader->SetInt1("u_Texture", 1);
    Renderer::Submit(m_TextureShader, m_TexturedQuad->vao,
        m_TexturedQuad->Transform(glm::vec3(0.5f, -0.5f, 0.0f)));

    Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
    /*ImGui::Begin("Options");

    // Child window for colored quad attributes
    if (ImGui::BeginChild("Colored Quad", ImVec2(0, -100)))
    {
        ImGui::TextUnformatted("Colored Quad");
        ImGui::ColorEdit3("Color", m_Color, ImGuiColorEditFlags_NoTooltip);
        ImGui::SameLine();
        if (ImGui::Button("Set Color"))
            m_ColorShader->SetFloat3("u_Color", m_Color[0], m_Color[1], m_Color[2]);

        ImGui::SliderFloat2("Position", Dewpsi::AddressOf(m_ColoredQuad->position.x), -1.0f, 1.0f);
    }
    ImGui::EndChild();

    // Child window for texured quad attributes
    if (ImGui::BeginChild("Textured Quad"))
    {
        ImGui::TextUnformatted("Textured Quad");
        ImGui::SliderFloat2("Position 1", Dewpsi::AddressOf(m_TexturedQuad->position.x), -1.0f, 1.0f);
    }
    ImGui::EndChild();

    ImGui::End();*/
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
