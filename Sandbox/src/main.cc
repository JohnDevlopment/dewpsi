#include <csignal>
#include <iostream>

// Dewpsi includes
#include <Dewpsi_Log.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_KeyEvent.h>

extern "C" void forcequit(int);

class ExampleLayer : public Dewpsi::Layer {
public:
    ExampleLayer() : Layer("Example")
    {
        PD_INFO("Created new layer, name: {0}", m_sDebugName.c_str());
    }
    
    ~ExampleLayer()
    {  }
    
    virtual void OnUpdate() override
    {
    }
    
    virtual void OnEvent(Dewpsi::Event& e) override;
};

void ExampleLayer::OnEvent(Dewpsi::Event& e)
{
    switch (e.GetEventType())
    {
    case Dewpsi::ET_KeyPressed:
        {
            using Dewpsi::KeyPressedEvent;
            KeyPressedEvent& keyEvent = reinterpret_cast<KeyPressedEvent&>(e);
            PD_INFO("{0}, key translates to '{1}'", keyEvent, char(keyEvent.GetKeyCode()));
            break;
        }
        
        default: break;
    } // end switch
}

class Sandbox : public Dewpsi::Application {
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }
    
    virtual ~Sandbox()
    {  }
};

static Dewpsi::Application* App = nullptr;

int main (int argc, char const* argv[])
{
    // calls a specific function when the user Control-C's the console
    std::signal(2, forcequit);
    
    // initialize the logging system
    Dewpsi::Log::Init();
    
    {
        using Dewpsi::WindowFlags;
        Dewpsi::WindowProps props;
        props.title = "Client Dewpsi Application";
//        props.title = "list renderers";
        props.x = PD_WINDOWPOS_CENTERED;
        props.y = PD_WINDOWPOS_CENTERED;
        props.width = 640;
        props.height = 480;
        props.index = 0;
        props.flags = WindowFlags::RendererAccelerated | WindowFlags::RendererVSync | WindowFlags::WindowOpenGL;
        Dewpsi::SetWindowProps(props);
    }
    
    // start client application
    App = Dewpsi::NewApplication();
    PD_INFO("Started sandbox application");
    
    // run main loop
    App->Run();
    
    delete App;
    return 0;
}

Dewpsi::Application* Dewpsi::NewApplication()
{
    return new Sandbox();
}

void forcequit(int sig)
{
    std::cout << "Application received signal 11 (SIGINT)...exiting program" << std::endl;
    delete App;
    std::exit(0);
}

