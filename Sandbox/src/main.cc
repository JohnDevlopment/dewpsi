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
#include <Dewpsi_Memory.h>

#define _PD_DEBUG_BREAKS
#include <Dewpsi_Debug.h>

#include <spdlog/sinks/stdout_sinks.h>

#include "OpenGLLayer.h"

using Dewpsi::StaticString;

struct SandboxData {
    PDchar enableImGui;
    Dewpsi::ImGuiInitData guiInit;

    SandboxData() : enableImGui(0), guiInit()
    {  }
};

static bool g_bOnce = false;

extern "C" void forcequit(int);
extern "C" void quit();

static int parseArguments(int, const char*[], SandboxData* data);

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

// sandbox application
class Sandbox : public Dewpsi::Application {
public:
    Sandbox(PDuserdata userdata);

    virtual ~Sandbox()
    {  }
};

Sandbox::Sandbox(PDuserdata userdata)
{
    m_UserData = userdata;
    SandboxData* data = (SandboxData*) userdata;
    PD_ASSERT(data, "NULL \"data\" pointer");
    if (data->enableImGui)
    {
        PushOverlay(new Dewpsi::ImGuiLayer(&data->guiInit));
        PD_INFO("Enabled Dear ImGui layer");
    }
    PushLayer(new SandboxLayer());
}

static Dewpsi::Application* App = nullptr;

int main (int argc, char const* argv[])
{
    Dewpsi::Scope<SandboxData> appData = Dewpsi::CreateScope<SandboxData>();

    // parse arguments in another function
    if (parseArguments(argc, argv, appData.get()) != PD_OKAY)
        return 1;

    // initialize the logging system
    Dewpsi::Log::Init();

    std::signal(SIGINT, forcequit);
    std::atexit(quit);

    {
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
        props.flags = Dewpsi::RendererVSync | Dewpsi::WindowOpenGL | Dewpsi::WindowResizable;
        SetWindowOpenGLAttribute(props, Dewpsi::MajorVersion, 3);
        SetWindowOpenGLAttribute(props, Dewpsi::MinorVersion, 3);
        SetWindowOpenGLAttribute(props, Dewpsi::Depth, 24);
        SetWindowOpenGLAttribute(props, Dewpsi::DoubleBuffer, 1);
        SetWindowOpenGLAttribute(props, Dewpsi::StencilSize, 8);
        Dewpsi::SetWindowProps(props);
    }

    // start client application
    App = Dewpsi::NewApplication(appData.get());
#ifdef PD_DEBUG
    PD_INFO("Started sandbox application");
#endif

    // set window clear color
    {
        Dewpsi::Window& rWindow = Dewpsi::Application::Get().GetWindow();
        rWindow.SetClearColor(Dewpsi::DefineColor(0.5f, 0.5f, 0.5f));
    }

    // run main loop
    App->Run();

    delete App;
    App = nullptr;
    return 0;
}

Dewpsi::Application* Dewpsi::NewApplication(PDuserdata userdata)
{
    return new Sandbox(userdata);
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

static constexpr StaticString ShaderFile = "Dewpsi/OpenGL/shaders/shaders.glsl";
static constexpr StaticString ImGuiShaderPath = "Dewpsi/OpenGL/shaders";

static constexpr StaticString Usage = \
    "sandbox -h    Display this help message\n" \
    "sandbox [-g]  Launch sandbox application with/without gui layer";

int parseArguments(int argc, const char* argv[], SandboxData* data)
{
    using std::cout;
    using std::cerr;
    using std::endl;

    // base name of the program
    const char* cpBaseName = Dewpsi::String::StringRevChar(argv[0], '/');
    if (! cpBaseName)
        cpBaseName = argv[0];
    else
        ++cpBaseName;

    // error logger
    auto err_logger = Dewpsi::Log::NewStderrLogger("Argument Error");
    err_logger->set_level(spdlog::level::info);

    int iCode = 1;

    // ImGui information
    data->guiInit.glslPath = ImGuiShaderPath.get();
    data->guiInit.glslVersion = "#version 130";

    do
    {
        iCode = Dewpsi::GetOption(argc, (char**) argv, ":a:bgh");
        if (iCode > 0)
        {
            switch (iCode)
            {
                case 'g':
                    cout << "Enable Dear ImGui layer\n";
                    data->enableImGui = 1;
                    break;

                case 'h':
                    //printUsage(cpBaseName);
                    cout << Usage.get() << endl;
                    return PD_INVALID;
                    break;

                case ':':
                    err_logger->error("Missing argument for '-{0}'", (char) optopt);
                    break;

                default:
                    err_logger->error("Unrecognized option '-{0}'", (char) iCode);
                    break;
            }
        } // end if (iCode > 0)
    }
    while (iCode > 0);

    if (optind < argc)
        cout << "Printing non-options\n";

    while (optind < argc)
    {
        cout << "argv[" << optind << "] = " << argv[optind++] << '\n';
    }

    return PD_OKAY;
}
