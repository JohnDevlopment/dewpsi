#include <csignal>
#include <iostream>

#include "sandbox.h"
#include "options.h"

// Dewpsi
#define PD_PROFILE 0
#include <Dewpsi_Debug.h>
#include <Dewpsi_Math.h>
#include <Dewpsi_Except.h>
#include <Dewpsi_String.h>
#include <DewpsiMath_Vec2.hpp>
#include <DewpsiMath_Util.hpp>
#include <ext/vec2_util.hpp>

#include <cstdlib>

#include <Dewpsi_Memory.h>
#include <bits/Dewpsi_Bits_Allocator.h>

using Dewpsi::StaticString;

static bool g_bOnce = false;

extern "C" void forcequit(int);
extern "C" void quit();

static Dewpsi::Application* App = nullptr;

struct Entity {
    char name[30];
    int  id;

    Entity() : id(0)
    {
        Dewpsi::String::MemSet(name, 0, 30);
    }

    ~Entity() {}
};

struct TestType1 {
    typedef TestType1* pointer;
};

struct TestType2 {};

int main (int argc, char const* argv[])
{
    PD_PROFILE_BEGIN_SESSION("Startup", "results_startup.json");

    // initialize the logging system
    Dewpsi::Log::Init();


    Dewpsi::PointerTraits<int*>::__ElementType a = 5; // int a = 5
    Dewpsi::PointerTraits<Dewpsi::Scope<int>>::__ElementType b = 10;

    PD_INFO("a = {}, b = {}", a, b);


    return 0;






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
        //props.flags = Dewpsi::RendererVSync | Dewpsi::WindowResizable;
        props.flags = Dewpsi::RendererVSync | Dewpsi::WindowOpenGL | Dewpsi::WindowResizable;
        appData->sizeRatio = dm::vector_cast<float>(dm::ratio(props.width, props.height));
        SetWindowAttribute(props, Dewpsi::MajorVersion, 3);
        SetWindowAttribute(props, Dewpsi::MinorVersion, 3);
        SetWindowAttribute(props, Dewpsi::Depth, 24);
        SetWindowAttribute(props, Dewpsi::DoubleBuffer, 1);
        //SetWindowAttribute(props, Dewpsi::StencilSize, 8);
        Dewpsi::SetWindowProps(props);
    }

    Dewpsi::Renderer::SetAPI(Dewpsi::RendererAPI::API::OpenGL);

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
