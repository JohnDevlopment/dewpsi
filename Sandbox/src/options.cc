#include "options.h"
#include "sandbox.h"
#include <Dewpsi_String.h>
#include <Dewpsi_Log.h>
#include <Dewpsi_Except.h>
#define INIFILE_FUNCTIONS
#include <inifile.hpp>
#include <tuple>

using Dewpsi::StaticString;

namespace {

auto DefaultArguments = std::make_tuple(PD_WINDOWPOS_CENTERED, 640U, 480U);
constexpr StaticString ShaderFile = "Dewpsi/OpenGL/shaders/shaders.glsl";
constexpr StaticString ImGuiShaderPath = "Dewpsi/OpenGL/shaders";
constexpr StaticString DefaultTitle = "Sandbox Application";

}

static constexpr StaticString OptionChars = "+:?t:x:y:w:h:W:H:d:";
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
    -d INT      A debug option, will be removed. <0>

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

    // Fill in the default parameters
    defaultParameters(data);

    // This will assert if the pointer is NULL.
    // If the path does not exist, this will do nothing.
    if (readIniFile("Sandbox/assets/settings.ini", data))
        PD_TRACE("Read Sandbox/assets/settings.ini");

    int iCode = 1;

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
        readIniFile(argv[optind], data);
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
                    PD_ERROR("Unrecognized option '{0}'", optopt);
                    cout << Usage.get() << endl;
                    return PD_INVALID;
                    break;

                case 't':
                    Dewpsi::String::Copy(data->title, optarg, sizeof(data->title));
                    break;

                case 'x':
                    {
                        PDstring temp = optarg;
                        if (temp == "center" || temp == "centered")
                            data->windowDim.x = PD_WINDOWPOS_CENTERED;
                        else
                        {
                            int temp2 = Dewpsi::String::StringToInt(optarg);
                            if (temp2 > 0)
                                data->windowDim.x = (PDuint) temp2;
                        }
                        break;
                    }

                case 'y':
                    {
                        PDstring temp = optarg;
                        if (temp == "center" || temp == "centered")
                            data->windowDim.y = PD_WINDOWPOS_CENTERED;
                        else
                        {
                            int temp2 = Dewpsi::String::StringToInt(optarg);
                            if (temp2 > 0)
                                data->windowDim.y = (PDuint) temp2;
                        }
                        break;
                    }

                case 'w':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                            data->windowDim.w = (PDuint) temp;
                        break;
                    }

                case 'h':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                            data->windowDim.h = (PDuint) temp;
                        break;
                    }

                case 'W':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                            data->resolution.x = temp;
                        break;
                    }

                case 'H':
                    {
                        int temp = Dewpsi::String::StringToInt(optarg);
                        if (temp > 0)
                            data->resolution.x = temp;
                        break;
                    }

                case ':':
                    PD_ERROR("Missing argument for '-{0}'", optopt);
                    break;

                default:
                    {
                        PDstring message = __PRETTY_FUNCTION__;
                        message += ": default case reached in switch() statement. That is not meant to happen.";
                        throw Dewpsi::DewpsiError(message);
                    }
                    break;
            }
        } // end if (iCode > 0)
    }
    while (iCode > 0);

    return PD_OKAY;
}

bool readIniFile(const char* file, SandboxData* data)
{
    PD_ASSERT(data, "NULL 'data' pointer");
    IniFile ini;

    // Attempt to open file
    if (ini.ReadFile(file) < 0)
        return false;

    // Helper macro
    #define _getoption(str, out, default) \
        PD_TRACE("Process INI option \"{0}\"", str); \
        ini.GetValue(str, out);

    // Window title
    PDstring sTitle;
    if (ini.GetValue("title", sTitle) >= 0)
        Dewpsi::String::Copy(data->title, sTitle.c_str(), sizeof(data->title));

    /*
    Get the x and y offsets for the window. If the option says "center", then the constant
    PD_WINDOWPOS_CENTERED is used. Otherwise extrapolate the integral value of the named
    option key.
    */
    {
        PDstring& temp = sTitle;

        if (ini.GetValue("x", temp) >= 0)
        {
            if (temp == "center" || temp == "centered")
                data->windowDim.x = PD_WINDOWPOS_CENTERED;
            else
                data->windowDim.x = (PDuint) Dewpsi::String::StringToInt(temp.c_str());
        }

        if (ini.GetValue("y", temp) >= 0)
        {
            if (temp == "center" || temp == "centered")
                data->windowDim.y = PD_WINDOWPOS_CENTERED;
            else
                data->windowDim.y = (PDuint) Dewpsi::String::StringToInt(temp.c_str());
        }
    }

    _getoption("width", data->windowDim.w, 640U);
    _getoption("height", data->windowDim.h, 480U);

    _getoption("ax", data->resolution.x, 640U);
    _getoption("ay", data->resolution.y, 480U);

    return true;

    #undef _getoption
}

void defaultParameters(SandboxData* p)
{
    PD_ASSERT(p, "Pointer argument is NULL");
    Dewpsi::String::Copy(p->title, DefaultTitle.get(), sizeof(p->title));
    p->windowDim.x = std::get<0>(DefaultArguments);
    p->windowDim.y = std::get<0>(DefaultArguments);
    p->windowDim.w = std::get<1>(DefaultArguments);
    p->windowDim.h = std::get<2>(DefaultArguments);
    p->resolution.x = std::get<1>(DefaultArguments);
    p->resolution.y = std::get<2>(DefaultArguments);
}
