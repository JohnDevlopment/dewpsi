#include <csignal>
#include <iostream>

// Dewpsi includes
#include <Dewpsi_Log.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_MouseEvent.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_Color.h>
#include <Dewpsi_String.h>
#include <Dewpsi_Vector.h>
#include <Dewpsi_ImGuiLayer.h>

#define _PD_DEBUG_BREAKS
#include <Dewpsi_Debug.h>

#include "OpenGLLayer.h"

using Dewpsi::StaticString;

static constexpr StaticString ShaderFile = "Dewpsi/OpenGL/shaders/shaders.glsl";

static constexpr StaticString ImGuiShaderPath = "Dewpsi/OpenGL/shaders";

static bool g_bOnce = false;

static Dewpsi::ImGuiInitData GuiInit;

extern "C" void forcequit(int);
extern "C" void quit();

static int parseArguments(int, const char*[]);

// sandbox application layer
class SandboxLayer : public Dewpsi::Layer {
public:
    SandboxLayer() : Layer("Sandbox")
    {  }

    ~SandboxLayer()
    {  }

    virtual void OnAttach() override
    {
        PD_INFO("Attaching {0} layer", m_sDebugName); // TODO: delete
    }

    virtual void OnEvent(Dewpsi::Event& e) override
    {
        //
    }
};

/*
void SandboxLayer::OnEvent(Dewpsi::Event& e)
{

}
*/

// sandbox application
class Sandbox : public Dewpsi::Application {
public:
    Sandbox()
    {
        PushOverlay(new Dewpsi::ImGuiLayer(&GuiInit));
        PushLayer(new SandboxLayer());
    }

    virtual ~Sandbox()
    {  }
};

static Dewpsi::Application* App = nullptr;

int main (int argc, char const* argv[])
{
    // parse arguments in another function
    if (parseArguments(argc, argv))
        return 1;

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
        props.flags = WindowFlags::RendererVSync | WindowFlags::WindowOpenGL | WindowFlags::WindowResizable;
        SetWindowOpenGLAttribute(props, Dewpsi::MajorVersion, 3);
        SetWindowOpenGLAttribute(props, Dewpsi::MinorVersion, 3);
        SetWindowOpenGLAttribute(props, Dewpsi::Depth, 24);
        SetWindowOpenGLAttribute(props, Dewpsi::DoubleBuffer, 1);
        SetWindowOpenGLAttribute(props, Dewpsi::StencilSize, 8);
        Dewpsi::SetWindowProps(props);
    }

    // ImGui information
    GuiInit.glslPath = ImGuiShaderPath.get();
    GuiInit.glslVersion = "#version 130";

    // start client application
    App = Dewpsi::NewApplication();
#ifdef PD_DEBUG
    PD_INFO("Started sandbox application");
#endif

    // create window
    {
        _PD_DEBUG_BREAK();
        Dewpsi::Window& rWindow = App->GetWindow();
        Dewpsi::WindowModeInfo info;
        if (Dewpsi::GetWindowInformation(&rWindow, &info) < 0)
        {
            PD_ERROR("Failed to retrieve window info: {0}", Dewpsi::GetError());
            return 1;
        }
#ifdef PD_DEBUG
        PD_INFO("\tswap interval: {0}\n\thardware acceleration: {1}\n\twindow width: {2}\n\twindow height: {3}\n\trendering device vendor: {4}\n\trenderer: {5}\n\trender version: {6}\n\trenderer extensions: {7}",
                     info.swap, (info.accel ? "enabled" : "disabled"), info.width, info.height,
                     info.vendor, info.renderer, info.version, info.extensions);

        // rgba sizes
        PD_INFO("\twindow red size: {0} bits\n\twindow green size: {1} bits\n\twindow blue size: {2} bits",
                     PD_LONYBBLE(info.red), PD_LONYBBLE(info.green), PD_LONYBBLE(info.blue));
#endif
        rWindow.SetClearColor(Dewpsi::DefineColor(0.5f, 0.5f, 0.5f));
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

int parseArguments(int argc, const char* argv[])
{
    using std::cout;
    using std::cerr;
    using std::endl;



    return 0;
}
