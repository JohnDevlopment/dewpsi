#ifndef SANDBOX_H
#define SANDBOX_H

#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Renderer.h>
#include <Dewpsi_Memory.h>
#include <Dewpsi_Shader.h>
#include <Dewpsi_String.h>
#include <Dewpsi_Rect.h>
#include <ext/vec2_float.hpp>
#include <ext/vec2_unsigned_int.hpp>

#include "sandbox_shape.h" // includes buffers, texture, vertex array, and camera

struct SandboxData {
    Dewpsi::Rectui windowDim;
    Dewpsi::ImGuiInitData guiInit;
    char title[50];
    dm::Vec2 sizeRatio;
    dm::UVec2 resolution;

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
      m_Camera(-data->sizeRatio.x, data->sizeRatio.x, -data->sizeRatio.y, data->sizeRatio.y),
      m_Color {0.5f, 0.0f, 0.0f} {}

    ~SandboxLayer() {}

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Dewpsi::Timestep delta) override;
	virtual void OnImGuiRender() override;
    virtual void OnEvent(Dewpsi::Event& e) override;

    bool OnKeyboardPressed(Dewpsi::KeyPressedEvent& e);

private:
    Dewpsi::OrthoCamera m_Camera;
    float m_Color[3];
    Dewpsi::Ref<Dewpsi::Shader> m_ColorShader;
    Dewpsi::Ref<Dewpsi::Shader> m_TextureShader;
    Dewpsi::Ref<SandboxShape<8, 6>> m_ColoredQuad;
    Dewpsi::Ref<SandboxShape<16, 6>> m_TexturedQuad;
};

// sandbox application
class Sandbox : public Dewpsi::Application {
public:
    Sandbox(PDuserdata userdata);

    virtual ~Sandbox() {}
};

#endif /* SANDBOX_H */
