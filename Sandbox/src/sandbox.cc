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
#include <Dewpsi_Memory.h>

void SandboxLayer::OnAttach()
{
    PD_INFO("Attaching {0} layer", m_sDebugName); // TODO: delete
    (void) 5;
}

void SandboxLayer::OnEvent(Dewpsi::Event& e)
{
    using namespace Dewpsi;
    EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(PD_BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
}

bool SandboxLayer::OnKeyPressed(Dewpsi::KeyPressedEvent& keyEvent)
{  }

void SandboxLayer::OnImGuiRender()
{  }

// sandbox app
Sandbox::Sandbox(PDuserdata userdata)
{
    m_UserData = userdata;
    SandboxData* data = (SandboxData*) userdata;
    PD_ASSERT(data, "NULL \"data\" pointer");
    PushLayer(new SandboxLayer());
}
