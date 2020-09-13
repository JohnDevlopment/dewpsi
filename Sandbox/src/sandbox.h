#ifndef SANDBOX_H
#define SANDBOX_H

#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Renderer.h>
#include <Dewpsi_Memory.h>
#include <Dewpsi_Shader.h>
#include <Dewpsi_OrthoCamera.h>
#include <Dewpsi_String.h>
#include <Dewpsi_Rect.h>
#include <ext/vec2_float.hpp>

namespace SandboxAppNS {
    struct Vertex {
        Dewpsi::Vector2D position;
        Dewpsi::FColor color;
    };
}

struct SandboxData {
    Dewpsi::Rectui windowDim;
    Dewpsi::ImGuiInitData guiInit;
    char title[50];
    dm::Vec2 sizeRatio;

    SandboxData()
    {
        Dewpsi::String::MemSet(title, 0, sizeof(title));
    }
};

// sandbox application layer
class SandboxLayer : public Dewpsi::Layer {
public:
    SandboxLayer(SandboxData* data)
    : Layer("Sandbox"),
      m_Camera(-data->sizeRatio.x, data->sizeRatio.x, -data->sizeRatio.y, data->sizeRatio.y)
    {}

    ~SandboxLayer()
    {}

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Dewpsi::Timestep delta) override;
	//virtual void OnImGuiRender() override;
    virtual void OnEvent(Dewpsi::Event& e) override;

    bool OnKeyboardPressed(Dewpsi::KeyPressedEvent& e);

private:
    Dewpsi::Ref<Dewpsi::Shader> m_Program;
    Dewpsi::Ref<Dewpsi::VertexArray> m_VertexArray;
    Dewpsi::Ref<Dewpsi::VertexBuffer> m_VertexBuffer;
    Dewpsi::Ref<Dewpsi::IndexBuffer> m_IndexBuffer;
    Dewpsi::OrthoCamera m_Camera;
};

// sandbox application
class Sandbox : public Dewpsi::Application {
public:
    Sandbox(PDuserdata userdata);

    virtual ~Sandbox() {}
};

#endif /* SANDBOX_H */
