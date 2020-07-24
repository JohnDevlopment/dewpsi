#include <csignal>
#include <iostream>

// Dewpsi includes
#include <Dewpsi_Log.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_Color.h>

static bool g_bOnce = false;

extern "C" void forcequit(int);
extern "C" void quit();

class ExampleLayer : public Dewpsi::Layer {
public:
    ExampleLayer() : Layer("Example")
    {  }
    
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
    std::signal(SIGINT, forcequit);
    std::atexit(quit);
    
    // initialize the logging system
    Dewpsi::Log::Init();
    
    {
        using Dewpsi::WindowFlags;
        using Dewpsi::SetWindowOpenGLAttribute;
        Dewpsi::WindowProps props;
        
        if (argc > 1)
        {
            if (std::strcmp(argv[1], "-l") == 0)
                props.title = "list renderers";
            else
                props.title = "Client Dewpsi Application";
        }
        else
            props.title = "Client Dewpsi Application";
        
        props.x = PD_WINDOWPOS_CENTERED;
        props.y = PD_WINDOWPOS_CENTERED;
        props.width = 640;
        props.height = 480;
        props.index = 0;
        props.flags = WindowFlags::RendererVSync | WindowFlags::WindowOpenGL;
        SetWindowOpenGLAttribute(props, Dewpsi::MajorVersion, 4);
        SetWindowOpenGLAttribute(props, Dewpsi::MinorVersion, 3);
        SetWindowOpenGLAttribute(props, Dewpsi::Depth, 24);
        SetWindowOpenGLAttribute(props, Dewpsi::DoubleBuffer, 1);
        SetWindowOpenGLAttribute(props, Dewpsi::ShareContext, 0);
        Dewpsi::SetWindowProps(props);
    }
    
    // start client application
    App = Dewpsi::NewApplication();
    PD_INFO("Started sandbox application");
    
    {
        Dewpsi::Window& rWindow = App->GetWindow();
        Dewpsi::WindowModeInfo info;
        if (Dewpsi::GetWindowInformation(&rWindow, &info) < 0)
        {
            PD_ERROR("Failed to retrieve window info: {0}", Dewpsi::GetError());
            return 1;
        }
        PD_INFO("\tswap interval: {0}\n\thardware acceleration: {1}\n\twindow width: {2}\n\twindow height: {3}\n\trendering device vendor: {4}\n\trenderer: {5}\n\trender version: {6}\n\trenderer extensions: {7}",
                     info.swap, (info.accel ? "enabled" : "disabled"), info.width, info.height,
                     info.vendor, info.renderer, info.version, info.extensions);
        
        // rgba sizes
        PD_INFO("\twindow red size: {0} bits\n\twindow green size: {1} bits\n\twindow blue size: {2} bits",
                     PD_LONYBBLE(info.red), PD_LONYBBLE(info.green), PD_LONYBBLE(info.blue));
    }
    
    // run main loop
    App->Run();
    
    delete App;
    App = nullptr;
    return 0;
}

Dewpsi::Application* Dewpsi::NewApplication()
{
    return new Sandbox();
}

void quit()
{
    if (! g_bOnce)
    {
        g_bOnce = true;
        delete App;
        App = nullptr;
    }
}

void forcequit(int sig)
{
    if (! g_bOnce)
    {
        g_bOnce = true;
        std::cout << "Application received signal 11 (SIGINT)...exiting program" << std::endl;
        delete App;
        std::exit(0);
    }
}

