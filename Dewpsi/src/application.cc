#include "application.h"
#include "timer.h"
#include "debug.h"
#include "window.h"
#include "applicationevent.h"

#include <SDL.h>
#include <stdexcept>

namespace Dewpsi {

static WindowProps _WindowProperties;

Application* Application::s_instance = nullptr;

void Application::Run()
{
    PD_PROFILE_FUNCTION();
    
    while (m_bRunning)
    {
        m_window->Update();
    }
}

Application::Application(const std::string& sName)
    : m_bRunning(true), m_window()
{
    PD_PROFILE_FUNCTION();
    
    if (! Log::IsInit())
    {
        throw std::runtime_error("Logger has not been initialized prior to application start");
    }
    
    s_instance = this;
    
    // initialize SDL2 video
    PD_CORE_TRACE("Initializing SDL2 library with SDL_Init(), flags: SDL_INIT_VIDEO");
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        PD_CORE_CRITICAL("Failed to initialize SDL, {}", SDL_GetError());
        throw std::runtime_error("SDL2 could not be initialized");
    }
    
    // create the window
    m_window = Window::Create(_WindowProperties);
}

Application::~Application()
{
    PD_PROFILE_FUNCTION();
    SDL_Quit();
}

// =================================================

void SetWindowProps(const WindowProps& props)
{
    _WindowProperties = props;
}

}

