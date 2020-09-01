#ifndef SANDBOX_H
#define SANDBOX_H

#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Renderer.h>
#include <Dewpsi_Memory.h>
#include <Dewpsi_Shader.h>
#include <Dewpsi_Vector.h>

namespace SandboxAppNS {
    struct Vertex {
        Dewpsi::Vector2D position;
        Dewpsi::FColor color;
    };
}

struct SandboxData {
    PDchar enableImGui;
    Dewpsi::ImGuiInitData guiInit;

    SandboxData() : enableImGui(0), guiInit()
    {  }
};

// sandbox application layer
class SandboxLayer : public Dewpsi::Layer {
public:
    SandboxLayer() : Layer("Sandbox")
    {  }

    ~SandboxLayer()
    {  }

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Dewpsi::Timestep delta) override;
	virtual void OnImGuiRender() override;
    virtual void OnEvent(Dewpsi::Event& e) override;

	bool OnKeyPressed(Dewpsi::KeyPressedEvent& keyEvent);

private:
    Dewpsi::Ref<Dewpsi::Shader> m_Program;
    Dewpsi::Ref<Dewpsi::VertexArray> m_VertexArray;
    Dewpsi::Ref<Dewpsi::VertexBuffer> m_VertexBuffer;
    Dewpsi::Ref<Dewpsi::IndexBuffer> m_IndexBuffer;
};

// sandbox application
class Sandbox : public Dewpsi::Application {
public:
    Sandbox(PDuserdata userdata);

    virtual ~Sandbox()
    {  }
};

#endif /* SANDBOX_H */
