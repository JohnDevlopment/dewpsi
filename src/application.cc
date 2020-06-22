#include "base.h"
#include "application.h"
#include "timer.h"
#include "debug.h"

namespace Dewpsi {

Application* Application::s_instance = nullptr;

void Application::Run()
{
    PD_PROFILE_FUNCTION();
    
    Timer::Sleep(3000U);
}

Application::Application(const std::string& sName)
    : m_bRunning(true)
{
    PD_PROFILE_FUNCTION();
    
    s_instance = this;
}

Application::~Application()
{
    PD_PROFILE_FUNCTION();
}

}

