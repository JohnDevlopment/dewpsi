#include <csignal>
#include <iostream>

#include "sandbox.h"

// Dewpsi
#define PD_PROFILE 0
#include <Dewpsi_Debug.h>
#include <Dewpsi_Math.h>

#include <DewpsiMath_Vec2.hpp>
#include <DewpsiMath_Util.hpp>
#include <ext/vec2_util.hpp>

using Dewpsi::StaticString;

static bool g_bOnce = false;

extern "C" void forcequit(int);
extern "C" void quit();

static int parseArguments(int, const char*[], SandboxData* data);

static Dewpsi::Application* App = nullptr;

int main (int argc, char const* argv[])
{
    PD_PROFILE_BEGIN_SESSION("Startup", "results_startup.json");

    // initialize the logging system
    Dewpsi::Log::Init();

    Dewpsi::Scope<SandboxData> appData = Dewpsi::CreateScope<SandboxData>();

    // parse arguments in another function
    if (parseArguments(argc, argv, appData.get()) != PD_OKAY)
        return 1;

    std::signal(SIGINT, forcequit);
    std::atexit(quit);

    {
        using Dewpsi::SetWindowAttribute;
        Dewpsi::WindowProps props;

        props.x = appData->windowDim.x;
        props.y = appData->windowDim.y;
        props.width = appData->windowDim.w;
        props.height = appData->windowDim.h;
        props.title = appData->title;
        props.flags = Dewpsi::RendererVSync | Dewpsi::WindowOpenGL | Dewpsi::WindowResizable;
        appData->sizeRatio = dm::vector_cast<float>(dm::ratio(props.width, props.height));
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

    PD_PROFILE_BEGIN_SESSION("Update", "results_update.json");
    // run main loop
    App->Run();
    PD_PROFILE_END_SESSION();

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
    sandbox [-t title]

Options:
    -h          Display this help message.
    -t TITLE    Set the title of the window (49 char max)
)";

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

    // default window title
    Dewpsi::String::Copy(data->title, "Sandbox Application");

    int iCode = 1;

    // window size
    data->windowDim.x = PD_WINDOWPOS_CENTERED;
    data->windowDim.y = PD_WINDOWPOS_CENTERED;
    data->windowDim.w = 640U;
    data->windowDim.h = 480U;

    // ImGui information
    data->guiInit.glslPath = ImGuiShaderPath.get();
    data->guiInit.glslVersion = "#version 130";

    do
    {
        iCode = Dewpsi::GetOption(argc, (char**) argv, ":?t:x:y:w:h:");
        if (iCode > 0)
        {
            switch (iCode)
            {
                case '?':
                    cout << Usage.get() << endl;
                    return PD_INVALID;
                    break;

                case 't':
                    Dewpsi::String::Copy(data->title, optarg, 50);
                    break;

                case 'x':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                        {
                            data->windowDim.x = (PDuint) temp;
                        }
                        break;
                    }

                case 'y':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                        {
                            data->windowDim.y = (PDuint) temp;
                        }
                        break;
                    }

                case 'w':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                        {
                            data->windowDim.w = (PDuint) temp;
                        }
                        break;
                    }

                case 'h':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                        {
                            data->windowDim.h = (PDuint) temp;
                        }
                        break;
                    }

                case ':':
                    PD_CORE_ERROR("Missing argument for '-{}'", optopt);
                    break;

                default:
                    PD_CORE_ERROR("Unrecognized option '-{}'", optopt);
                    break;
            }
        } // end if (iCode > 0)
    }
    while (iCode > 0);

    return PD_OKAY;
}
