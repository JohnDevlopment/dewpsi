#include <csignal>
#include <iostream>
#include <log.h>
#include <whichos.h>
#include <window.h>
#include <application.h>

extern "C" void forcequit(int);

class Sandbox : public Dewpsi::Application {
public:
    Sandbox()
    {  }
    virtual ~Sandbox()
    {  }
};

static Dewpsi::Application* App = nullptr;

int main (int argc, char const* argv[])
{
    // calls a specific function when the user Control-C's the console
    std::signal(2, forcequit);
    
    // initialize the logging system
    ::Dewpsi::Log::Init();
    
    {
        Dewpsi::WindowProps props;
        props.title = "Client Dewpsi Application";
        props.width = 640;
        props.height = 480;
        props.flags[1] = 0;
        props.flags[2] = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
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

