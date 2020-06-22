#include <iostream>
#include <thread>
#include "dewpsi.h"

class Sandbox : public Dewpsi::Application {
public:
    Sandbox()
    {  }
    
    ~Sandbox()
    {  }
};

Dewpsi::Application* Dewpsi::NewApplication()
{
    return new Sandbox();
}

int main (int argc, char const* argv[])
{
    //Dewpsi::Timer tm;
    
    Dewpsi::Log::Init();
    
    //Dewpsi::Log::GetCoreLogger()->warn("Logging system initialized");
    //Dewpsi::Log::GetClientLogger()->info("Logging system initialized");
    
    PD_CORE_WARN("Logging system initialized");
    int a = 5;
    PD_INFO("Logging system initialized, var={0}", a);
    
    auto app = Dewpsi::NewApplication();
    app->Run();
    delete app;
    
    return 0;
}

