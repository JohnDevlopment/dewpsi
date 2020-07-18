#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Core.h"
#include "Dewpsi_Application.h"
#include "Dewpsi_ApplicationEvent.h"
#include "Dewpsi_MouseEvent.h"
#include "Dewpsi_KeyEvent.h"
#include "Dewpsi_String.h"
#include "Dewpsi_ImGui_SDL.h"
//#include "Dewpsi_ImGui_OpenGL3.h"

#include <glad/glad.h>

#include <SDL.h>
#include <csignal>
#include <cstdlib>
#include <unordered_map>

#define space1          "    "
#define space2          "        "
#define space3          "            "
#define space(x)        space##x
#define newl(x)         "\n" space(x)
#define newlstr(x, s)   "\n" space(x) s
#define spacestr(x, s)  space(x) s

void OpenGL_Test1();
void OpenGL_Test2();
void OpenGL_Test3();

static bool UseOpenGL = false;

static constexpr Dewpsi::StaticString FalseTrueStrings[2] = { "false", "true" };

static std::unordered_map<uint32_t, Dewpsi::KeyCode> KeyCodeMap = {
    { SDLK_0,               PD_KEY_0 },
    { SDLK_1,               PD_KEY_1 },
    { SDLK_2,               PD_KEY_2 },
    { SDLK_3,               PD_KEY_3 },
    { SDLK_4,               PD_KEY_4 },
    { SDLK_5,               PD_KEY_5 },
    { SDLK_6,               PD_KEY_6 },
    { SDLK_7,               PD_KEY_7 },
    { SDLK_8,               PD_KEY_8 },
    { SDLK_9,               PD_KEY_9 },
    { SDLK_a,               PD_KEY_A },
    { SDLK_b,               PD_KEY_B },
    { SDLK_c,               PD_KEY_C },
    { SDLK_d,               PD_KEY_D },
    { SDLK_e,               PD_KEY_E },
    { SDLK_f,               PD_KEY_F },
    { SDLK_g,               PD_KEY_G },
    { SDLK_h,               PD_KEY_H },
    { SDLK_i,               PD_KEY_I },
    { SDLK_j,               PD_KEY_J },
    { SDLK_k,               PD_KEY_K },
    { SDLK_l,               PD_KEY_L },
    { SDLK_m,               PD_KEY_M },
    { SDLK_n,               PD_KEY_N },
    { SDLK_o,               PD_KEY_O },
    { SDLK_p,               PD_KEY_P },
    { SDLK_q,               PD_KEY_Q },
    { SDLK_r,               PD_KEY_R },
    { SDLK_s,               PD_KEY_S },
    { SDLK_t,               PD_KEY_T },
    { SDLK_u,               PD_KEY_U },
    { SDLK_v,               PD_KEY_V },
    { SDLK_w,               PD_KEY_W },
    { SDLK_x,               PD_KEY_X },
    { SDLK_y,               PD_KEY_Y },
    { SDLK_z,               PD_KEY_Z },
    { SDLK_SEMICOLON,       PD_KEY_SEMICOLON },
    { SDLK_EQUALS,          PD_KEY_EQUAL },
    { SDLK_UNKNOWN,         PD_KEY_UNKNOWN },
    { SDLK_RETURN,          PD_KEY_ENTER },
    { SDLK_ESCAPE,          PD_KEY_ESCAPE },
    { SDLK_BACKSPACE,       PD_KEY_BACKSPACE },
    { SDLK_TAB,             PD_KEY_TAB },
    { SDLK_SPACE,           PD_KEY_SPACE },
    { SDLK_EXCLAIM,         PD_KEY_EXCLAIM },
    { SDLK_QUOTEDBL,        PD_KEY_DOUBLEQUOTE },
    { SDLK_HASH,            PD_KEY_HASH },
    { SDLK_PERCENT,         PD_KEY_PERCENT },
    { SDLK_DOLLAR,          PD_KEY_DOLLAR },
    { SDLK_AMPERSAND,       PD_KEY_AMPERSAND },
    { SDLK_QUOTE,           PD_KEY_APOSTROPHE },
    { SDLK_LEFTPAREN,       PD_KEY_LEFTPAREN },
    { SDLK_RIGHTPAREN,      PD_KEY_RIGHTPAREN },
    { SDLK_ASTERISK,        PD_KEY_ASTERISK },
    { SDLK_PLUS,            PD_KEY_PLUS },
    { SDLK_COMMA,           PD_KEY_COMMA },
    { SDLK_MINUS,           PD_KEY_MINUS },
    { SDLK_PERIOD,          PD_KEY_PERIOD },
    { SDLK_SLASH,           PD_KEY_SLASH },
    { SDLK_COLON,           PD_KEY_COLON },
    { SDLK_SEMICOLON,       PD_KEY_SEMICOLON },
    { SDLK_LESS,            PD_KEY_LESS },
    { SDLK_EQUALS,          PD_KEY_EQUAL },
    { SDLK_GREATER,         PD_KEY_GREATER },
    { SDLK_QUESTION,        PD_KEY_QUESTION },
    { SDLK_AT,              PD_KEY_AT },
    { SDLK_LEFTBRACKET,     PD_KEY_LEFTBRACKET },
    { SDLK_BACKSLASH,       PD_KEY_BACKSLASH },
    { SDLK_RIGHTBRACKET,    PD_KEY_RIGHTBRACKET },
    { SDLK_CARET,           PD_KEY_CARET },
    { SDLK_UNDERSCORE,      PD_KEY_UNDERSCORE },
    { SDLK_BACKQUOTE,       PD_KEY_GRAVEACCENT },
    { SDLK_F1,              PD_KEY_F1 },
    { SDLK_F2,              PD_KEY_F2 },
    { SDLK_F3,              PD_KEY_F3 },
    { SDLK_F4,              PD_KEY_F4 },
    { SDLK_F5,              PD_KEY_F5 },
    { SDLK_F6,              PD_KEY_F6 },
    { SDLK_F7,              PD_KEY_F7 },
    { SDLK_F8,              PD_KEY_F8 },
    { SDLK_F9,              PD_KEY_F9 },
    { SDLK_F10,             PD_KEY_F10 },
    { SDLK_F11,             PD_KEY_F11 },
    { SDLK_F12,             PD_KEY_F12 },
    { SDLK_PRINTSCREEN,     PD_KEY_PRINTSCREEN },
    { SDLK_CAPSLOCK,        PD_KEY_CAPSLOCK },
    { SDLK_SCROLLLOCK,      PD_KEY_SCROLLLOCK },
    { SDLK_PAUSE,           PD_KEY_PAUSE },
    { SDLK_INSERT,          PD_KEY_INSERT },
    { SDLK_HOME,            PD_KEY_HOME },
    { SDLK_PAGEUP,          PD_KEY_PAGEUP },
    { SDLK_DELETE,          PD_KEY_DELETE },
    { SDLK_END,             PD_KEY_END },
    { SDLK_PAGEDOWN,        PD_KEY_PAGEDOWN },
    { SDLK_RIGHT,           PD_KEY_RIGHT },
    { SDLK_LEFT,            PD_KEY_LEFT },
    { SDLK_DOWN,            PD_KEY_DOWN },
    { SDLK_UP,              PD_KEY_UP },
    { SDLK_NUMLOCKCLEAR,    PD_KEY_NUMLOCK },
    
    { SDLK_KP_DIVIDE,       PD_KEY_KPDIVIDE },
    { SDLK_KP_MULTIPLY,     PD_KEY_KPMULTIPLY },
    { SDLK_KP_MINUS,        PD_KEY_KPSUBTRACT },
    { SDLK_KP_PLUS,         PD_KEY_KPADD },
    { SDLK_KP_ENTER,        PD_KEY_KPENTER },
    { SDLK_KP_PERIOD,       PD_KEY_KPPERIOD },
    
    { SDLK_KP_1,            PD_KEY_KP1 },
    { SDLK_KP_2,            PD_KEY_KP2 },
    { SDLK_KP_3,            PD_KEY_KP3 },
    { SDLK_KP_4,            PD_KEY_KP4 },
    { SDLK_KP_5,            PD_KEY_KP5 },
    { SDLK_KP_6,            PD_KEY_KP6 },
    { SDLK_KP_7,            PD_KEY_KP7 },
    { SDLK_KP_8,            PD_KEY_KP8 },
    { SDLK_KP_9,            PD_KEY_KP9 },
    { SDLK_KP_0,            PD_KEY_KP0 },
    
    { SDLK_LCTRL,           PD_KEY_LEFTCONTROL },
    { SDLK_LSHIFT,          PD_KEY_LEFTSHIFT },
    { SDLK_LALT,            PD_KEY_LEFTALT },
    { SDLK_LGUI,            PD_KEY_LEFTGUI },
    { SDLK_RGUI,            PD_KEY_RIGHTGUI },
    { SDLK_RCTRL,           PD_KEY_RIGHTCONTROL },
    { SDLK_RSHIFT,          PD_KEY_RIGHTSHIFT },
    { SDLK_RALT,            PD_KEY_RIGHTALT }
};

#if 0


#endif

#ifdef PD_DEBUG
    #ifdef PD_LIST_RENDERERS
    static void list_sdl_renderers();
    #endif
    #ifdef PD_PRINT_WINDOW
    static void print_window_information(const Dewpsi::WindowProps&, SDL_Window*);
    #endif
    #ifdef PD_PRINT_RENDERER
    static void print_renderer_information(SDL_Renderer*);
    #endif
#endif

static Dewpsi::KeyCode GetKeyCode(int kc);

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
    
    static bool bInit = false;
    
    if (! UseOpenGL)
    {
        SDL_RenderClear(m_renderer);
        Application::Get().UpdateLayers();
        SDL_RenderPresent(m_renderer);
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
        Application::Get().UpdateLayers();
        if (! bInit)
        {
            OpenGL_Test1();
            bInit = true;
        }
        OpenGL_Test2();
        SDL_GL_SwapWindow(m_window);
    }
}

void SDL2Window::SetVSync(bool bEnable)
{
    //
}

bool SDL2Window::IsVSync() const
{
    return false;
}

void SDL2Window::SetClearColor(const Color& color)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1);
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
    
    // for debugging purposes
#if defined(PD_DEBUG) && defined(PD_LIST_RENDERERS)
    if (props.title == "list renderers")
    {
        list_sdl_renderers();
    }
#endif
    
    // set OpenGL flags prior to window creation
    if (props.flags & WindowOpenGL)
    {
        for (int x = 0; x < int(OpenGLAttributes::Count); ++x)
        {
            int iAttrVal = 0;
            SDL_GLattr eAttr;
            OpenGLAttributes eMyAttr;
            
            GetWindowOpenGLAttribute(props, eMyAttr, iAttrVal, static_cast<unsigned int>(x));
            
            // no attribute given
            if (eMyAttr == OpenGLAttributes::Empty)
                continue;
            
            switch (eMyAttr)
            {
            case OpenGLAttributes::Depth:
                eAttr = SDL_GL_DEPTH_SIZE;
                break;
            
            case OpenGLAttributes::DoubleBuffer:
                eAttr = SDL_GL_DOUBLEBUFFER;
                PD_CORE_TRACE("SDL_GL_DOUBLEBUFFER"); // TODO: remove
                break;
            
            case OpenGLAttributes::RedSize:
                eAttr = SDL_GL_RED_SIZE;
                break;
            
            case OpenGLAttributes::GreenSize:
                eAttr = SDL_GL_GREEN_SIZE;
                break;
            
            case OpenGLAttributes::BlueSize:
                eAttr = SDL_GL_BLUE_SIZE;
                break;
            
            case OpenGLAttributes::AlphaSize:
                eAttr = SDL_GL_ALPHA_SIZE;
                break;
            
            case OpenGLAttributes::AccelerationRequired:
                eAttr = SDL_GL_ACCELERATED_VISUAL;
                PD_CORE_TRACE("SDL_GL_ACCELERATED_VISUAL"); // TODO: remove
                break;
            
            case OpenGLAttributes::MajorVersion:
                eAttr = SDL_GL_CONTEXT_MAJOR_VERSION;
                break;
            
            case OpenGLAttributes::MinorVersion:
                eAttr = SDL_GL_CONTEXT_MINOR_VERSION;
                break;
            
            case OpenGLAttributes::ContextFlags:
                eAttr = SDL_GL_CONTEXT_FLAGS;
                break;
            
            default: break;
            }
            
            SDL_GL_SetAttribute(eAttr, iAttrVal);
        }
        
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        //SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    }    
    
    // create a window
    {
        uint32_t uiFlags = 0;
        std::vector<uint32_t> vSDLFlags = {
            SDL_WINDOW_FULLSCREEN,
            SDL_WINDOW_FULLSCREEN_DESKTOP,
            SDL_WINDOW_OPENGL,
            SDL_WINDOW_VULKAN,
            SDL_WINDOW_SHOWN,
            SDL_WINDOW_HIDDEN,
            SDL_WINDOW_BORDERLESS,
            SDL_WINDOW_RESIZABLE,
            SDL_WINDOW_MINIMIZED,
            SDL_WINDOW_MAXIMIZED,
            SDL_WINDOW_INPUT_GRABBED,
            SDL_WINDOW_INPUT_FOCUS,
            SDL_WINDOW_MOUSE_FOCUS,
            SDL_WINDOW_FOREIGN,
            SDL_WINDOW_ALLOW_HIGHDPI,
            SDL_WINDOW_MOUSE_CAPTURE,
            SDL_WINDOW_ALWAYS_ON_TOP,
            SDL_WINDOW_SKIP_TASKBAR,
            SDL_WINDOW_UTILITY,
            SDL_WINDOW_TOOLTIP,
            SDL_WINDOW_POPUP_MENU
        };
        std::vector<uint32_t> vWinFlags = {
            WindowFullscreen,
            WindowFullDesktop,
            WindowOpenGL,
            WindowVulkan,
            WindowShown,
            WindowHidden,
            WindowBorderless,
            WindowResizable,
            WindowMinimized,
            WindowMaximized,
            WindowInputGrabbed,
            WindowInputFocus,
            WindowMouseFocus,
            WindowForeign,
            WindowAllowHighDPI,
            WindowMouseCapture,
            WindowAlwaysOnTop,
            WindowSkipTaskbar,
            WindowUtility,
            WindowTooltip,
            WindowPopupMenu
        };
        
        // for each window flag that's set, add an SDL flag
        auto itrSDLFlag = vSDLFlags.begin();
        for (auto itrWinFlag : vWinFlags)
        {
            // if the Dewpsi window bit is set,
            // the corresponding SDL window flag is set
            if (props.flags & itrWinFlag)
                uiFlags |= *itrSDLFlag;
            ++itrSDLFlag;
        }
        
        // create a window (SDL_WINDOWPOS_CENTERED = center)
        m_window = SDL_CreateWindow(props.title.c_str(), props.x, props.y,
                                    props.width, props.height, uiFlags);
        
        PD_CORE_ASSERT(m_window, "Failed to create window: {0}", SDL_GetError());
        
        #if defined(PD_DEBUG) && defined(PD_PRINT_WINDOW)
        print_window_information(props, m_window);
        #endif
        
        m_native.window = m_window;
    }
    
    // create rendering context
    if (! (props.flags & WindowOpenGL)) {
        uint32_t uiFlags = 0;
        std::vector<uint32_t> vWinFlags = {
            RendererSoftware,
            RendererAccelerated,
            RendererVSync,
            RendererTargetTexture
        };
        std::vector<uint32_t> vSDLFlags = {
            SDL_RENDERER_SOFTWARE,
            SDL_RENDERER_ACCELERATED,
            SDL_RENDERER_PRESENTVSYNC,
            SDL_RENDERER_TARGETTEXTURE
        };
        
        // for each render flag that's set, add an SDL flag
        auto itrSDLFlag = vSDLFlags.begin();
        for (auto itrWinFlag : vWinFlags)
        {
            // if the Dewpsi render bit is set,
            // the corresponding SDL window flag is set
            if (props.flags & itrWinFlag)
                uiFlags |= *itrSDLFlag;
            ++itrSDLFlag;
        }
        
        // create renderer
        m_renderer = SDL_CreateRenderer(m_window, props.index, uiFlags);
        PD_CORE_ASSERT(m_renderer, "Failed to create renderer: {0}", SDL_GetError());
        
        #if defined(PD_DEBUG) && defined(PD_PRINT_RENDERER)
        print_renderer_information(m_renderer);
        #endif
        
        m_native.renderer = m_renderer;
    }
    
    // mark vsync as enabled
    if (props.flags & RendererVSync)
        m_data.vsync = true;
    
    // OpenGL context
    if (props.flags & WindowOpenGL)
    {
        // new OpenGL context
        m_context = SDL_GL_CreateContext(m_window);
        PD_CORE_ASSERT(m_context, "Failed to create OpenGL context: {0}", SDL_GetError());
        m_native.context = m_context;
        
        // associate window with context
        SDL_GL_MakeCurrent(m_window, m_context);
        
        // GLAD loader
        int iCode = gladLoadGLLoader(SDL_GL_GetProcAddress);
        PD_CORE_ASSERT(iCode, "Failed to load GLAD");
        
        PD_CORE_TRACE("OpenGL vendor: {0}, renderer: {1}, version: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
        
        // enable vsync
        if (m_data.vsync)
        {
            SDL_GL_SetSwapInterval(1);
            PD_CORE_TRACE("Enabled vertical sync with OpenGL context");
        }
        
        UseOpenGL = true;
    }
    
    // register event callback
    SDL_AddEventWatch(OnEvent, &m_data);
}

void SDL2Window::Shutdown()
{
    OpenGL_Test3();
    
    if (m_context)
    {
        SDL_GL_DeleteContext(m_context);
        m_context = nullptr;
    }
    
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
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
        
    case SDL_MOUSEMOTION:
        {
            SDL_MouseMotionEvent& motion = event->motion;
            MouseMovedEvent e(float(motion.x), float(motion.y));
            pWinData->callback(e);
            break;
        }
        
    case SDL_MOUSEWHEEL:
        {
            SDL_MouseWheelEvent& wheel = event->wheel;
            int32_t iMulitplier = (wheel.direction == SDL_MOUSEWHEEL_NORMAL) ? 1 : -1;
            MouseScrolledEvent e(static_cast<float>(wheel.x * iMulitplier), static_cast<float>(wheel.y * iMulitplier));
            pWinData->callback(e);
            break;
        }
        
    case SDL_KEYDOWN:
        {
            SDL_KeyboardEvent& key = event->key;
            KeyPressedEvent e(GetKeyCode(key.keysym.sym), int(key.repeat));
            pWinData->callback(e);
            break;
        }
        
    case SDL_KEYUP:
        {
            SDL_KeyboardEvent& key = event->key;
            KeyReleasedEvent e(GetKeyCode(key.keysym.sym));
            pWinData->callback(e);
            break;
        }
        
        default: break;
    }
    
    return 0;
}

} // end namespace Dewpsi

Dewpsi::KeyCode GetKeyCode(int kc)
{
    Dewpsi::KeyCode ret;
    
    auto found = KeyCodeMap.find(static_cast<uint32_t>(kc));
    if (found == KeyCodeMap.end())
    {
        PD_CORE_ERROR("No key for '{0}'", kc);
        return PD_KEY_UNKNOWN;
    }
    
    return static_cast<Dewpsi::KeyCode>(found->second);
}

// == DEBUG == //

#ifdef PD_DEBUG

#ifdef PD_LIST_RENDERERS
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
#endif /* PD_LIST_RENDERERS */

#ifdef PD_PRINT_WINDOW
static constexpr Dewpsi::StaticString WinFlagStrings[] = {
    "fullscreen window",
    "fullscreen window with desktop resolution",
    "window usable with an OpenGL context",
    "window usable with a Vulkan instance",
    "window is visible",
    "window is invisible",
    "window has no border",
    "window can be resized",
    "window is minimized",
    "window is maximized",
    "window grabs input",
    "window has input focus",
    "window has mouse focus",
    "window was not created with SDL",
    "window created in high-DPI mode",
    "window captures the mouse",
    "window displayed on top of all other windows",
    "window was not added to the taskbar",
    "window is a utility window",
    "window is a tooltip",
    "window is a popup window"
};

void print_window_information(const Dewpsi::WindowProps& props, SDL_Window* win)
{
    using std::cout;
    using std::endl;
    
    PD_CORE_TRACE("Printing window information...");
    
    cout << "Window properties:\n";
    
    {
        SDL_Rect rect;
        SDL_GetWindowPosition(win, &rect.x, &rect.y);
        SDL_GetWindowSize(win, &rect.w, &rect.h);
        
        cout << spacestr(1, "x position: ") << rect.x \
        << newlstr(1, "y position: ") << rect.y << newlstr(1, "width: ") \
        << rect.w << newlstr(1, "height: ") << rect.h << endl;
    }
    
    cout << spacestr(1, "title: ") << SDL_GetWindowTitle(win) << '\n';
    
    {
        const uint32_t uiaFlags[] = {
            SDL_WINDOW_FULLSCREEN,
            SDL_WINDOW_FULLSCREEN_DESKTOP,
            SDL_WINDOW_OPENGL,
            SDL_WINDOW_VULKAN,
            SDL_WINDOW_SHOWN,
            SDL_WINDOW_HIDDEN,
            SDL_WINDOW_BORDERLESS,
            SDL_WINDOW_RESIZABLE,
            SDL_WINDOW_MINIMIZED,
            SDL_WINDOW_MAXIMIZED,
            SDL_WINDOW_INPUT_GRABBED,
            SDL_WINDOW_INPUT_FOCUS,
            SDL_WINDOW_MOUSE_FOCUS,
            SDL_WINDOW_FOREIGN,
            SDL_WINDOW_ALLOW_HIGHDPI,
            SDL_WINDOW_MOUSE_CAPTURE,
            SDL_WINDOW_ALWAYS_ON_TOP,
            SDL_WINDOW_SKIP_TASKBAR,
            SDL_WINDOW_UTILITY,
            SDL_WINDOW_TOOLTIP,
            SDL_WINDOW_POPUP_MENU
        };
        
        cout << spacestr(1, "flags:") << endl;
        
        const uint32_t uiFlags = SDL_GetWindowFlags(win);
        for (int x = 0; x < 21; ++x)
        {
            if (uiFlags & uiaFlags[x])
            {
                cout << space(2) << WinFlagStrings[x].get() << '\n';
            }
        }
        cout << std::flush;
    } // end block
}
#endif /* PD_PRINT_WINDOW */

#ifdef PD_PRINT_RENDERER
static constexpr Dewpsi::StaticString RenFlagStrings[4] = {
    "renderer is a software fallback",
    "renderer supports hardware acceleration",
    "present is synced with the refresh rate (vsync)",
    "renderer supports rendering to texture"
};

static constexpr Dewpsi::StaticString PixelTypeStrings[] = {
    "unknown",
    "indexed 1 bit",
    "indexed 4 bit",
    "indexed 8 bit",
    "packed 1 bit",
    "packed 4 bit",
    "packed 8 bit",
    "array of unsigned 8-bit  pixels",
    "array of unsigned 16-bit pixels",
    "array of unsigned 32-bit pixels",
    "array of floating-point 16-bit pixels",
    "array of floating-point 32-bit pixels"
};

static constexpr Dewpsi::StaticString PixelLayoutStrings[] = {
    "none",
    "3-3-2",
    "4-4-4-4",
    "1-5-5-5",
    "5-5-5-1",
    "5-6-5",
    "8-8-8-8",
    "2-10-10-10",
    "10-10-10-2"
};

static constexpr Dewpsi::StaticString PixelOrderStrings[] = {
    "none",
    "packed [empty], red, green, blue",
    "packed red, green, blue, [empty]",
    "packed alpha, red, green, blue",
    "packed red, green, blue, alpha",
    "packed [empty], blue, green, red",
    "packed blue, green, red, [empty]",
    "packed alpha, blue, green, red",
    "packed blue, green, red, alpha",
    
    "SDL_BITMAPORDER_NONE",
    "SDL_BITMAPORDER_4321",
    "SDL_BITMAPORDER_1234",
    
    "SDL_ARRAYORDER_RGB",
    "SDL_ARRAYORDER_RGBA",
    "SDL_ARRAYORDER_ARGB",
    "SDL_ARRAYORDER_BGR",
    "SDL_ARRAYORDER_BGRA",
    "SDL_ARRAYORDER_ABGR"
};

static const char* what_pixel_order(uint32_t order, uint32_t type)
{
    uint16_t uiIndex = 0;
    
    if (type == SDL_PIXELTYPE_PACKED8 || type == SDL_PIXELTYPE_PACKED16 || type == SDL_PIXELTYPE_PACKED32)
    {
        switch (order)
        {
        case SDL_PACKEDORDER_XRGB:
            uiIndex = 1;
            break;
        
        case SDL_PACKEDORDER_RGBX:
            uiIndex = 2;
            break;
        
        case SDL_PACKEDORDER_ARGB:
            uiIndex = 3;
            break;
        
        case SDL_PACKEDORDER_RGBA:
            uiIndex = 4;
            break;
        
        case SDL_PACKEDORDER_XBGR:
            uiIndex = 5;
            break;
        
        case SDL_PACKEDORDER_BGRX:
            uiIndex = 6;
            break;
        
        case SDL_PACKEDORDER_ABGR:
            uiIndex = 7;
            break;
        
        case SDL_PACKEDORDER_BGRA:
            uiIndex = 8;
            break;
        
        default: break;
        }
    }
    else if (type == SDL_PIXELTYPE_INDEX1 || type == SDL_PIXELTYPE_INDEX4 || type == SDL_PIXELTYPE_INDEX8)
    {
        switch (order)
        {
        case SDL_BITMAPORDER_4321:
            uiIndex = 9;
            break;
            
        case SDL_BITMAPORDER_1234:
            uiIndex = 10;
            
        default: break;
        }
    }
    else {
        switch (order)
        {
        case SDL_ARRAYORDER_RGB:
            uiIndex = 12;
            break;
            
        case SDL_ARRAYORDER_RGBA:
            uiIndex = 13;
            break;
            
        case SDL_ARRAYORDER_ARGB:
            uiIndex = 14;
            break;
            
        case SDL_ARRAYORDER_BGR:
            uiIndex = 15;
            break;
            
        case SDL_ARRAYORDER_BGRA:
            uiIndex = 16;
            break;
            
        case SDL_ARRAYORDER_ABGR:
            uiIndex = 17;
            break;
            
        default: break;
        }
    }
    
    return PixelOrderStrings[uiIndex].get();
}

void print_renderer_information(SDL_Renderer* ren)
{
    using std::cout;
    using std::endl;
    
    SDL_RendererInfo info;
    int iCode = SDL_GetRendererInfo(ren, &info);
    
    PD_CORE_ASSERT(iCode == 0, "failed to get rendering context info");
    
    // basic information
    PD_CORE_TRACE("Printing rendering context information...");
    
    cout << "Rendering context properties:" << newlstr(1, "number of supported texture formats: ") \
    << info.num_texture_formats << newlstr(1, "maximum texture width: ") << info.max_texture_width \
    << newlstr(1, "maximum texture height: ") << info.max_texture_height << endl;
    
    // flags
    {
        const uint32_t uiaFlags[] = {
            SDL_RENDERER_SOFTWARE,
            SDL_RENDERER_ACCELERATED,
            SDL_RENDERER_PRESENTVSYNC,
            SDL_RENDERER_TARGETTEXTURE
        };
        
        cout << spacestr(1, "flags: ");
        
        // get each flag
        for (int x = 0; x < 4; ++x)
        {
            if (info.flags & uiaFlags[x])
            {
                cout << newl(2) << RenFlagStrings[x].get();
            }
        }
        cout << endl;
    }
    
    // texture formats
    cout << spacestr(1, "texture formats: ") << endl;
    
    for (int x = 0; x < int(info.num_texture_formats); ++x)
    {
        const uint32_t uiFormat = info.texture_formats[x];
        
        cout << spacestr(2, "#") << (x+1) << ":\n" << spacestr(3, "type: ") \
            << PixelTypeStrings[SDL_PIXELTYPE(uiFormat)].get();
        cout << newlstr(3, "pixel order: ") << what_pixel_order(SDL_PIXELTYPE(uiFormat), SDL_PIXELORDER(uiFormat));
        cout << newlstr(3, "pixel layout: ") << PixelLayoutStrings[SDL_PIXELLAYOUT(uiFormat)].get();
        cout << newlstr(3, "bits per pixel: ") << SDL_BITSPERPIXEL(uiFormat) << endl;
        
        if (SDL_ISPIXELFORMAT_INDEXED(uiFormat))
        {
            cout << newlstr(3, "this format is indexed");
        }
        
        if (SDL_ISPIXELFORMAT_ALPHA(uiFormat))
        {
            cout << newlstr(3, "this format has an alpha channel");
        }
        
        if (SDL_ISPIXELFORMAT_FOURCC(uiFormat))
        {
            cout << newlstr(3, "this format is unique");
        }
        
        cout << endl;
    }
}
#endif /* PD_PRINT_RENDERER */

#endif /* PD_DEBUG */

#undef space1
#undef space2
#undef space3
#undef space
#undef newl
#undef newlstr
#undef spacestr
