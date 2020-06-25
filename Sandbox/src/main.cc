#include "dewpsi.h"

class Sandbox : public Dewpsi::Application {
public:
    Sandbox()
    {  }
    virtual ~Sandbox()
    {  }
};

int main (int argc, char const* argv[])
{
    Dewpsi::Application* ptr = Dewpsi::NewApplication();
    Dewpsi::Application::s_instance = ptr;
    
    // initialize logger
    ::Dewpsi::Log::Init();
    PD_CORE_INFO("Starting Dewpsi engine.");
    PD_INFO("Starting sandbox application.");
    
    ptr->Run();
    
    delete ptr;
    return 0;
}

Dewpsi::Application* Dewpsi::NewApplication()
{
    return new Sandbox();
}

