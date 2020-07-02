#include "whichos.h"
#include "core.h"

#include <SDL.h>

namespace Dewpsi {

SDL2Window::SDL2Window(const WindowProps& props)
    : m_window(nullptr), m_renderer(nullptr), m_data()
{
    Init(props);
}

SDL2Window::~SDL2Window()
{
    Shutdown();
}

void SDL2Window::Update()
{
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
}

void SDL2Window::SetVSync(bool bEnable)
{
    //
}

bool SDL2Window::IsVSync() const
{
    return false;
}

void SDL2Window::Init(const WindowProps& props)
{
    m_data = props;
    
    // SDL2 video not initialized
    if (! SDL_WasInit(SDL_INIT_VIDEO))
    {
        PD_CORE_ERROR("SDL2 video was not initialized");
        throw std::runtime_error("SDL2 video was not initialized");
    }
    
    PD_CORE_ASSERT(! m_window, "Window already created");
    
    // create a window
    m_window = SDL_CreateWindow(props.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                props.width, props.height, props.flags[0]);
    PD_CORE_ASSERT(m_window, "Failed to create window: {}", SDL_GetError());
    
    // create renderer
    m_renderer = SDL_CreateRenderer(m_window, props.flags[1], props.flags[2]);
    PD_CORE_ASSERT(m_renderer, "Failed to create renderer: {}", SDL_GetError());
    
    {
        SDL_RendererInfo info;
        const char* caBools[2] = {
            "false", "true"
        };
        
        int iRes = SDL_GetRendererInfo(m_renderer, &info);
        PD_CORE_ASSERT(iRes == 0, "Failed to retrieve information on the renderer");
        
        PD_CORE_INFO("Renderer created: name = {}, number of texture formats: {}", info.name, info.num_texture_formats);
        PD_CORE_INFO("    Hardware acceleration: {}\n    VSync enabled: {}\n    Supports rendering to texture: {}", 
                     caBools[IsNonzero(info.flags & SDL_RENDERER_ACCELERATED)],
                     caBools[IsNonzero(info.flags & SDL_RENDERER_PRESENTVSYNC)],
                     caBools[IsNonzero(info.flags & SDL_RENDERER_TARGETTEXTURE)] );
        
        //PD_CORE_INFO();
    }
    
    if (props.flags[2] & SDL_RENDERER_PRESENTVSYNC)
    {
        m_data.vsync = true;
    }
}

void SDL2Window::Shutdown()
{
    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);
    
    if (m_window)
        SDL_DestroyWindow(m_window);
}

}
