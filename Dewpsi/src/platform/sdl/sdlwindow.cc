#include "whichos.h"
#include "core.h"
#include "applicationevent.h"

#include <SDL.h>
#include <csignal>
#include <cstdlib>

#ifdef PD_DEBUG
static void list_sdl_renderers();
#endif

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
    SDL_PumpEvents();
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
    
    // for debugging purposes
#ifdef PD_DEBUG
    if (props.title == "list renderers")
    {
        list_sdl_renderers();
    }
#endif
    
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
        
        PD_CORE_INFO("Renderer created\n    name: {}\n    # of texture formats: {}\n    hardware acceleration: {}\n    vsync enabled: {}\n    rendering to texture: {}",
                     info.name, info.num_texture_formats,
                     caBools[IsNonzero(info.flags & SDL_RENDERER_ACCELERATED)],
                     caBools[IsNonzero(info.flags & SDL_RENDERER_PRESENTVSYNC)],
                     caBools[IsNonzero(info.flags & SDL_RENDERER_TARGETTEXTURE)] );
    }
    
    if (props.flags[2] & SDL_RENDERER_PRESENTVSYNC)
    {
        m_data.vsync = true;
    }
    
    // register event callback
    SDL_AddEventWatch(OnEvent, &m_data);
}

void SDL2Window::Shutdown()
{
    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);
    
    if (m_window)
        SDL_DestroyWindow(m_window);
}

int SDL2Window::OnEvent(void* udata, SDL_Event* event)
{
    WindowData* const pWinData = reinterpret_cast<WindowData*>(udata);
    
    switch (event->type)
    {
    case SDL_QUIT:
        {
            WindowCloseEvent e;
            pWinData->callback(e);
            break;
        }
        
    case SDL_WINDOWEVENT:
        {
            if (event->window.event == SDL_WINDOWEVENT_RESIZED)
            {
                WindowResizeEvent e;
                pWinData->callback(e);
            }
            break;
        }
        
        default: break;
    }
    
    return 0;
}

} // end namespace Dewpsi

#ifdef PD_DEBUG
void list_sdl_renderers()
{
    using Dewpsi::IsNonzero;
    
    SDL_RendererInfo info;
    const char* caBools[2] = {
        "false", "true"
    };
    
    int iNumDrivers = SDL_GetNumRenderDrivers();
    PD_CORE_ASSERT(iNumDrivers >= 0, "Failed to query renderer driver count: {}", SDL_GetError());
    
    for (int x = 0; x < iNumDrivers; ++x)
    {
        int iCode = SDL_GetRenderDriverInfo(x, &info);
        PD_CORE_ASSERT(iCode == 0, "Failed to query driver info: {}", SDL_GetError());
        
        PD_CORE_INFO("Render driver index {}\n    name: {}\n    # of texture formats: {}\n    hardware acceleration: {}\n    vsync: {}\n    rendering to texture: {}",
                     x, info.name, info.num_texture_formats,
                     caBools[IsNonzero(info.flags & SDL_RENDERER_ACCELERATED)],
                     caBools[IsNonzero(info.flags & SDL_RENDERER_PRESENTVSYNC)],
                     caBools[IsNonzero(info.flags & SDL_RENDERER_TARGETTEXTURE)] );
    }
    
    std::raise(SIGINT);
}
#endif
