#include <csignal>
#include <iostream>

#include "sandbox.h"

// Dewpsi
#define PD_PROFILE 1
#include <Dewpsi_Debug.h>
#include <spdlog/sinks/stdout_sinks.h>

using Dewpsi::StaticString;

static bool g_bOnce = false;

extern "C" void forcequit(int);
extern "C" void quit();

static int parseArguments(int, const char*[], SandboxData* data);

static Dewpsi::Application* App = nullptr;

int main (int argc, char const* argv[])
{
    Dewpsi::Scope<SandboxData> appData = Dewpsi::CreateScope<SandboxData>();

    PD_PROFILE_BEGIN_SESSION("Startup", "results_startup.json");

    // parse arguments in another function
    if (parseArguments(argc, argv, appData.get()) != PD_OKAY)
        return 1;

    // initialize the logging system
    Dewpsi::Log::Init();

    std::signal(SIGINT, forcequit);
    std::atexit(quit);

    {
        using Dewpsi::SetWindowAttribute;
        Dewpsi::WindowProps props;

        props.x = PD_WINDOWPOS_CENTERED;
        props.y = PD_WINDOWPOS_CENTERED;
        props.width = 640;
        props.height = 480;
        props.index = 0;
        props.title = "Sandbox Application";
        props.flags = Dewpsi::RendererVSync | Dewpsi::WindowOpenGL | Dewpsi::WindowResizable;
        SetWindowAttribute(props, Dewpsi::MajorVersion, 3);
        SetWindowAttribute(props, Dewpsi::MinorVersion, 3);
        SetWindowAttribute(props, Dewpsi::Depth, 24);
        SetWindowAttribute(props, Dewpsi::DoubleBuffer, 1);
        SetWindowAttribute(props, Dewpsi::StencilSize, 8);
        Dewpsi::SetWindowProps(props);
    }

    // start client application
    App = Dewpsi::NewApplication(appData.get());

    PD_PROFILE_END_SESSION();

    // run main loop
    App->Run();

    PD_PROFILE_BEGIN_SESSION("Shutdown", "results_shutdown.json");
    delete App;
    App = nullptr;
    PD_PROFILE_END_SESSION();
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

static constexpr StaticString Usage = R"(
    sandbox -h
    sandbox [-g] [-i FILE]

Options:
    -h      Display this help message.
    -g      Disable the ImGui layer.
    -i FILE Read the ini file FILE. <NULL>
)";

int parseArguments(int argc, const char* argv[], SandboxData* data)
{
    using std::cout;
    using std::cerr;
    using std::endl;

    data->enableImGui = 1;

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
        iCode = Dewpsi::GetOption(argc, (char**) argv, ":a:ghi:");
        if (iCode > 0)
        {
            switch (iCode)
            {
                case 'g':
                    cout << "Disable Dear ImGui layer\n";
                    data->enableImGui = 0;
                    break;

                case 'h':
                    cout << Usage.get() << endl;
                    return PD_INVALID;
                    break;

                case 'i':
                    cout << "Read " << optarg << std::endl;
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
