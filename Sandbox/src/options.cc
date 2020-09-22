#include "options.h"
#include <Dewpsi_String.h>
#include <Dewpsi_Log.h>
#define INIFILE_FUNCTIONS
#include <inifile.hpp>
#include "sandbox.h"

using Dewpsi::StaticString;

static constexpr StaticString ShaderFile = "Dewpsi/OpenGL/shaders/shaders.glsl";
static constexpr StaticString ImGuiShaderPath = "Dewpsi/OpenGL/shaders";

static constexpr StaticString OptionChars = "+:?t:x:y:w:h:W:H:";

static constexpr StaticString Usage = R"(
    sandbox -h
    sandbox [options] [ini_file]

Sandbox application for the Dewpsi engine. Commandline arguments supercede settings defined in the ini file.

Options:
    -h          Display this help message.
    -t TITLE    Set the title of the window (49 char max)
    -x POS      Set the initial X offset of the window. <centered>
    -y POS      Set the initial Y offset of the window. <centered>
    -w SIZE     Set the width of the window.
    -h SIZE     Set the height of the window.

Ini File:
    title       Analogous to the '-t' option above.
    x           Analogous to '-x'
    y           Analogous to '-y'
    width       Analogous to '-w'
    height      Analogous to '-h'
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

    // resolution
    data->resolution.x = 640;
    data->resolution.y = 480;

    // ImGui information
    data->guiInit.glslPath = ImGuiShaderPath.get();
    data->guiInit.glslVersion = "#version 130";

    // find the first non-option
    // path to an ini file
    do
    {
        iCode = Dewpsi::GetOption(argc, (char**) argv, OptionChars.get());
    }
    while (iCode > 0);
    if (optind == (argc - 1))
    {
        PD_TRACE("Non-option found");
        const char* cpIniFile = nullptr;
        cpIniFile = argv[optind];
        readIniFile(cpIniFile, data);
    }

    // process options
    optind = 0; // resets GetOption
    do
    {
        iCode = Dewpsi::GetOption(argc, (char**) argv, OptionChars.get());
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

                case 'W':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                        {
                            data->resolution.x = temp;
                        }
                        break;
                    }

                case 'H':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                        {
                            data->resolution.x = temp;
                        }
                        break;
                    }

                case ':':
                    PD_ERROR("Missing argument for '-{}'", optopt);
                    break;

                default:
                    PD_ERROR("Unrecognized option '-{}'", optopt);
                    break;
            }
        } // end if (iCode > 0)
    }
    while (iCode > 0);

    return PD_OKAY;
}

bool readIniFile(const char* file, SandboxData* data)
{
    IniFile ini;
    if (ini.ReadFile(file) < 0)
        return false;

    assert(data);

    #define _getoption(str, out, default) \
        PD_TRACE("Process INI option \"{0}\"", str); \
        if (ini.GetValue(str, out) < 0) \
        { \
            out = default; \
        }

    PDstring sTitle;
    if (ini.GetValue("title", sTitle) < 0)
        Dewpsi::String::Copy(data->title, "Sandbox Application", sizeof(data->title));
    else
        Dewpsi::String::Copy(data->title, sTitle.c_str(), sizeof(data->title));

    PDuint uiSize;

    _getoption("x", data->windowDim.x, 300U);
    _getoption("y", data->windowDim.y, 200U);

    _getoption("width", data->windowDim.w, 640U);
    _getoption("height", data->windowDim.h, 480U);

    _getoption("ax", data->resolution.x, 640U);
    _getoption("ay", data->resolution.y, 480U);

    return true;

    #undef _getoption
}
