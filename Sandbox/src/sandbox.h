#ifndef SANDBOX_H
#define SANDBOX_H

#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_WhichOS.h>

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
    virtual void OnEvent(Dewpsi::Event& e) override;
	virtual void OnImGuiRender() override;

	bool OnKeyPressed(Dewpsi::KeyPressedEvent& keyEvent);
};

// sandbox application
class Sandbox : public Dewpsi::Application {
public:
    Sandbox(PDuserdata userdata);

    virtual ~Sandbox()
    {  }
};

#endif /* SANDBOX_H */
