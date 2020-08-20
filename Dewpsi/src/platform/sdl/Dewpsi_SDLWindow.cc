#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Core.h"
#include "Dewpsi_Application.h"
#include "Dewpsi_ApplicationEvent.h"
#include "Dewpsi_MouseEvent.h"
#include "Dewpsi_KeyEvent.h"
#include "Dewpsi_String.h"
#include "Dewpsi_ImGui_SDL.h"
#include "Dewpsi_Vector.h"
#include "Dewpsi_OpenGL.h"
#include "Dewpsi_Debug.h"
#include "Dewpsi_Except.h"
#include "Dewpsi_OpenGLContext.h"

#include <SDL.h>
#include <csignal>
#include <cstdlib>
#include <cstddef>
#include <unordered_map>

#define space1          "    "
#define space2          "        "
#define space3          "            "
#define space(x)        space##x
#define newl(x)         "\n" space(x)
#define newlstr(x, s)   "\n" space(x) s
#define spacestr(x, s)  space(x) s

static constexpr Dewpsi::StaticString FalseTrueStrings[2] = { "false", "true" };

#include "Dewpsi_KeyCodeMap.cxx"

#ifdef PD_DEBUG
    #ifdef PD_PRINT_WINDOW
    static void print_window_information(const Dewpsi::WindowProps&, SDL_Window*);
    #endif
    #ifdef PD_PRINT_OPENGL_ATTRIBUTES
    static void print_opengl_attributes(SDL_Window*);
    #endif
#endif

static SDL_GLattr Dewpsi2SDL_GL_Attrib(Dewpsi::OpenGLAttributes attr);
static void SeparateFormat(PDuint32, Dewpsi::WindowModeInfo&);
static int SDL_GL_Loader();

Dewpsi::KeyCode SDL2DewpsiKeyCode(int kc);
uint32_t Dewpsi2SDLKeyCode(Dewpsi::KeyCode code);

Dewpsi::MouseCode SDL2DewpsiMouseCode(int mc);
int Dewpsi2SDLMouseCode(Dewpsi::MouseCode mc);

namespace Dewpsi {

SDL2Window::SDL2Window(const WindowProps& props)
    : m_Window(nullptr), m_data(), m_clearColor()
{
    Init(props);
}

SDL2Window::~SDL2Window()
{
    Shutdown();
}

void SDL2Window::OnUpdate()
{
    SDL_PumpEvents();
    m_Context->SwapBuffers();
}

bool SDL2Window::IsValid() const
{
    return ((m_Window != nullptr) && m_Context);
}

void SDL2Window::SetVSync(bool bEnable)
{
    if (bEnable)
        SDL_GL_SetSwapInterval(1);
    else
        SDL_GL_SetSwapInterval(0);

    m_data.vsync = bEnable;
}

bool SDL2Window::IsVSync() const
{
    return m_data.vsync;
}

void SDL2Window::SetClearColor(const Color& color)
{
    m_clearColor = color;

    FColor clearColor = color;
    glClearColor(clearColor.red, clearColor.green, clearColor.blue, 1.0f);
}

void SDL2Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void SDL2Window::Init(const WindowProps& props)
{
    PD_CORE_ASSERT(! SDL_WasInit(SDL_INIT_VIDEO), "SDL2 already initialized");
    PD_CORE_ASSERT(! m_Window, "Window already created");

    const char* cpExcept = "SDL2 could not be initialized";
    constexpr PDuint uiSDLFlags = SDL_INIT_VIDEO;

    m_data = props;

    // initialize SDL2 video
    if (SDL_Init(uiSDLFlags) != 0)
    {
        PD_CORE_CRITICAL("Failed to initialize SDL: {0}", SDL_GetError());
        throw DewpsiError(cpExcept);
    }

    // set OpenGL flags prior to window creation
    for (int x = 0; x < int(OpenGLAttributes::Count); ++x)
    {
        int iAttrVal = 0;
        SDL_GLattr eAttr;
        OpenGLAttributes eMyAttr;

        GetWindowOpenGLAttribute(props, eMyAttr, iAttrVal, static_cast<unsigned int>(x));

        // no attribute given
        if (eMyAttr == OpenGLAttributes::Empty)
            continue;

        // convert Dewpsi::OpenGLAttributes to SDL_GLattr
        eAttr = Dewpsi2SDL_GL_Attrib(eMyAttr);
        if ((int) eAttr >= 0)
            SDL_GL_SetAttribute(eAttr, iAttrVal);
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
            /* If the Dewpsi window bit is set,
               the corresponding SDL window flag is set. */
            if (props.flags & itrWinFlag)
                uiFlags |= *itrSDLFlag;
            ++itrSDLFlag;
        }

        // create a window (SDL_WINDOWPOS_CENTERED = center)
        m_Window = SDL_CreateWindow(props.title.c_str(), props.x, props.y,
                                    props.width, props.height, uiFlags);
        if (! m_Window)
        {
            PD_CORE_CRITICAL("Failed to create window: {0}", SDL_GetError());
            throw DewpsiError(cpExcept);
        }
        m_data.windowID = SDL_GetWindowID(m_Window);

#if defined(PD_DEBUG) && defined(PD_PRINT_WINDOW)
        print_window_information(props, m_Window);
#endif
    }

    // mark vsync as enabled
    if (props.flags & RendererVSync)
        m_data.vsync = true;

    m_Context = RenderContext::Create(m_Window);

    // initialize context
    if (m_Context->Init() != PD_OKAY)
    {
        std::stringstream ss;
        ss << "Failed to initialize OpenGL context: " << Dewpsi::GetError();
        throw DewpsiError(ss.str());
    }

    // enable vsync
    if (m_data.vsync)
    {
        if (props.flags & WindowOpenGL)
            SDL_GL_SetSwapInterval(1);
        else if (props.flags & WindowVulkan)
            throw DewpsiError("Vulkan support is not currently implemented");
    }

    // get width and height of window
    {
        int w, h;
        SDL_GetWindowSize(m_Window, &w, &h);
        PD_CORE_ASSERT(w && h, "Failed to get window size: {0}", SDL_GetError());
        glViewport(0, 0, w, h);
        m_data.width = w;
        m_data.height = h;
    }

    // set initial background color
    SetClearColor(DefineColor(PD_COLOR_BLACK));

    // TODO: remove or edit this block
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glCullFace(GL_BACK);
    ///////////////////////////////////////////////////////////////

    // get the dimensions and format of the window
    {
        SDL_DisplayMode mode;
        int temp = SDL_GetWindowDisplayMode(m_Window, &mode);
        PD_CORE_ASSERT(temp >= 0, "Failed to derive window information: {0}", SDL_GetError());

        m_data.width  = mode.w;
        m_data.height = mode.h;
        m_data.format = mode.format;
    }

    // register event callback
    SDL_AddEventWatch(OnEvent, &m_data);
}

void SDL2Window::Shutdown()
{
    // delete the OpenGL context
    m_Context.reset();

    // delete window
    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

int SDL2Window::OnEvent(void* udata, SDL_Event* event)
{
    WindowData* const pWinData = reinterpret_cast<WindowData*>(udata);

    ImGui_ImplSDL2_ProcessEvent(event);

    switch (event->type)
    {
    case SDL_QUIT:
        {
            WindowCloseEvent e(pWinData->windowID);
            pWinData->callback(e);
            break;
        }

    case SDL_WINDOWEVENT:
        {
            SDL_WindowEvent& winEvent = event->window;
            if (winEvent.event == SDL_WINDOWEVENT_RESIZED)
            {
                WindowResizeEvent e(winEvent.windowID, winEvent.data1, winEvent.data2);
                pWinData->callback(e);
            }
            else if (winEvent.event == SDL_WINDOWEVENT_CLOSE)
            {
                WindowCloseEvent e(winEvent.windowID);
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
            int32_t iMultiplier = (wheel.direction == SDL_MOUSEWHEEL_NORMAL) ? 1 : -1;
            MouseScrolledEvent e(static_cast<float>(wheel.x * iMultiplier),
                                 static_cast<float>(wheel.y * iMultiplier));
            pWinData->callback(e);
            break;
        }

    case SDL_KEYDOWN:
        {
            SDL_KeyboardEvent& key = event->key;
            KeyPressedEvent e(SDL2DewpsiKeyCode(key.keysym.sym), int(key.repeat));
            pWinData->callback(e);
            break;
        }

    case SDL_KEYUP:
        {
            SDL_KeyboardEvent& key = event->key;
            KeyReleasedEvent e(SDL2DewpsiKeyCode(key.keysym.sym));
            pWinData->callback(e);
            break;
        }

    case SDL_MOUSEBUTTONUP:
        {
            SDL_MouseButtonEvent& button = event->button;
            MouseReleasedEvent e(SDL2DewpsiMouseCode(button.button));
            pWinData->callback(e);
            break;
        }

    case SDL_MOUSEBUTTONDOWN:
        {
            SDL_MouseButtonEvent& button = event->button;
            MousePressedEvent e(SDL2DewpsiMouseCode(button.button));
            pWinData->callback(e);
            break;
        }

    case SDL_TEXTINPUT:
        {
            SDL_TextInputEvent& text = event->text;
            KeyTypedEvent e(text.text);
            pWinData->callback(e);
            break;
        }

        default: break;
    }

    return 0;
}

int GetWindowInformation(Window* win, WindowModeInfo* info)
{
    const char cpInvalid[] = "undefined";

    PD_CONSTEXPR int iaAttrs1[] = {
        SDL_GL_RED_SIZE,
        SDL_GL_GREEN_SIZE,
        SDL_GL_BLUE_SIZE,
        SDL_GL_ALPHA_SIZE,
    };

    PD_CONSTEXPR StaticString aStrings[] = {
        "red bit size",
        "green bit size",
        "blue bit size",
        "alpha bit size"
    };

    auto retUString = [&cpInvalid](const PDuchar* str) -> const PDuchar* {
        if (str) return str;
        return (const PDuchar*) cpInvalid;
    };

    int iCode = 0;

    // null window pointer
    if (! win)
    {
        PD_BADPARAM("win");
        return -1;
    }

    // null output pointer
    if (! info)
    {
        PD_BADPARAM("info");
        return -1;
    }

    {
        int iVal;

        SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &iVal);

        // swap interval
        info->swap = SDL_GL_GetSwapInterval();
        // hardware acceleration
        info->accel = iVal;
        // window width and height
        info->width = win->GetWidth();
        info->height = win->GetHeight();
        // string fields
        info->vendor     = retUString(glGetString(GL_VENDOR));
        info->version    = retUString(glGetString(GL_VERSION));
        info->extensions = retUString(glGetString(GL_EXTENSIONS));
        // rgba format
        PDuint32 uiFormat = reinterpret_cast<SDL2Window*>(win)->GetFormat();
        SeparateFormat(uiFormat, *info);
    }

    return iCode;
}

} // end namespace Dewpsi

Dewpsi::MouseCode SDL2DewpsiMouseCode(int mc)
{
    Dewpsi::MouseCode ret;

    switch (mc)
    {
    case SDL_BUTTON_LEFT:
        ret = PD_MOUSEBUTTON_LEFT;
        break;

    case SDL_BUTTON_RIGHT:
        ret = PD_MOUSEBUTTON_RIGHT;
        break;

    case SDL_BUTTON_MIDDLE:
        ret = PD_MOUSEBUTTON_MIDDLE;
        break;

    default:
        ret = PD_MOUSEBUTTON_UNKNOWN;
        break;
    }

    return ret;
}

int Dewpsi2SDLMouseCode(Dewpsi::MouseCode mc)
{
    int ret;

    switch (mc)
    {
    case PD_MOUSEBUTTON_LEFT:
        ret = SDL_BUTTON_LEFT;

    case PD_MOUSEBUTTON_RIGHT:
        ret = SDL_BUTTON_RIGHT;

    case PD_MOUSEBUTTON_MIDDLE:
        ret = SDL_BUTTON_MIDDLE;
        break;

    default:
        ret = SDLK_UNKNOWN;
        break;
    }

    return ret;
}

Dewpsi::KeyCode SDL2DewpsiKeyCode(int kc)
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

uint32_t Dewpsi2SDLKeyCode(Dewpsi::KeyCode code)
{
    uint32_t uiKey;

    auto found = RevKeyCodeMap.find(code);
    if (found == RevKeyCodeMap.end())
    {
        PD_CORE_ERROR("No key for '{0}'", code);
        return static_cast<uint32_t>(PD_KEY_UNKNOWN);
    }

    return uiKey;
}

SDL_GLattr Dewpsi2SDL_GL_Attrib(Dewpsi::OpenGLAttributes attr)
{
    using Dewpsi::OpenGLAttributes;
    SDL_GLattr eAttr;

    switch (attr)
    {
    case OpenGLAttributes::Depth:
        eAttr = SDL_GL_DEPTH_SIZE;
        break;

    case OpenGLAttributes::DoubleBuffer:
        eAttr = SDL_GL_DOUBLEBUFFER;
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

    case OpenGLAttributes::BufferSize:
        eAttr = SDL_GL_BUFFER_SIZE;
        break;

    case OpenGLAttributes::StencilSize:
        eAttr = SDL_GL_STENCIL_SIZE;
        break;

    case OpenGLAttributes::ShareContext:
        eAttr = SDL_GL_SHARE_WITH_CURRENT_CONTEXT;
        break;

    default:
        eAttr = (SDL_GLattr) -1;
        break;
    }

    return eAttr;
}

void SeparateFormat(PDuint32 format, Dewpsi::WindowModeInfo& info)
{
    PDuint8 bits[] = {0, 0, 0, 0};
    PDuint16 uiOrder = SDL_PIXELORDER(format);

    /* How to use: the first two arguments are the size of the red color in bits and its order number, 1-4. If the order
    number were 1, red would be the first in every group of four color fields. The other three pairs correspond to the
    green, blue, and alpha fields respectively.

    So func(8, 1, 8, 2, 8, 3, 8, 4) would mean that it's red, green, blue, then alpha, in order, and each is 8 bits long. */
    auto func = [&bits](PDuint8 rs, PDuint8 ro, PDuint8 gs, PDuint8 go, PDuint8 bs, PDuint8 bo, PDuint8 as, PDuint8 ao) {
        bits[0] = PD_CREATEBYTE(rs, ro);
        bits[1] = PD_CREATEBYTE(gs, go);
        bits[2] = PD_CREATEBYTE(bs, bo);
        bits[3] = PD_CREATEBYTE(as, ao);
    };

    // for formats with alpha channels
    if (SDL_ISPIXELFORMAT_ALPHA(format))
    {
        switch (format)
        {
        // 16 bit formats
        case SDL_PIXELFORMAT_ARGB4444:
            func(4, 2, 4, 3, 4, 4, 4, 1);
            break;

        case SDL_PIXELFORMAT_RGBA4444:
            func(4, 1, 4, 2, 4, 3, 4, 4);
            break;

        case SDL_PIXELFORMAT_ABGR4444:
            func(4, 4, 4, 3, 4, 2, 4, 1);
            break;

        case SDL_PIXELFORMAT_BGRA4444:
            func(4, 3, 4, 2, 4, 1, 4, 4);
            break;

        case SDL_PIXELFORMAT_ARGB1555:
            func(5, 2, 5, 3, 5, 4, 1, 1);
            break;

        case SDL_PIXELFORMAT_RGBA5551:
            func(5, 1, 5, 2, 5, 3, 1, 4);
            break;

        case SDL_PIXELFORMAT_ABGR1555:
            func(5, 4, 5, 3, 5, 2, 1, 1);
            break;

        case SDL_PIXELFORMAT_BGRA5551:
            func(5, 3, 5, 2, 5, 1, 1, 4);
            break;

        // 32 bit formats
        case SDL_PIXELFORMAT_RGBA8888:
            func(8, 1, 8, 2, 8, 3, 8, 4);
            break;

        case SDL_PIXELFORMAT_ARGB8888:
            func(8, 2, 8, 3, 8, 4, 8, 1);
            break;

        case SDL_PIXELFORMAT_ABGR8888:
            func(8, 4, 8, 3, 8, 2, 8, 4);
            break;

        case SDL_PIXELFORMAT_BGRA8888:
            func(8, 3, 8, 2, 8, 1, 8, 4);
            break;

        case SDL_PIXELFORMAT_ARGB2101010:
            func(10, 2, 10, 3, 10, 4, 2, 1);
            break;

        default:
            break;
        }
    }
    // no alpha channel
    else
    {
        switch (format)
        {
        case SDL_PIXELFORMAT_RGB565:
            func(5, 1, 6, 2, 5, 3, 0, -1);
            break;

        case SDL_PIXELFORMAT_BGR565:
            func(5, 3, 6, 2, 5, 1, 0, -1);
            break;

        case SDL_PIXELFORMAT_RGBX8888:
        case SDL_PIXELFORMAT_RGB888:
            func(8, 1, 8, 2, 8, 3, 0, -1);
            break;

        case SDL_PIXELFORMAT_BGRX8888:
        case SDL_PIXELFORMAT_BGR888:
            func(8, 3, 8, 2, 8, 1, 0, -1);
            break;

        default:
            break;
        }
    }

    info.red   = bits[0];
    info.green = bits[1];
    info.blue  = bits[2];
    info.alpha = bits[3];
}

// == DEBUG == //

#ifdef PD_DEBUG

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

#ifdef PD_PRINT_OPENGL_ATTRIBUTES
void print_opengl_attributes(SDL_Window* win)
{
    PD_CORE_ASSERT( >= 0, "Failed to get window information: {}", SDL_GetError());
    PD_CORE_INFO("Window display width: {0}\nWindow display height: {1}\nWindow refresh rate: {2} Hz\nWindow BPP: {3}",
        winMode.w, winMode.h, winMode.refresh_rate, SDL_BITSPERPIXEL(winMode.format));

    int iCode = 0;

    {
        const int iaAttrs[] = {
            SDL_GL_RED_SIZE,  SDL_GL_GREEN_SIZE,
            SDL_GL_BLUE_SIZE, SDL_GL_ALPHA_SIZE
        };
        int iaValues[4];

        for (int x = )
    }

    iCode = SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &iAttr);

    PD_CORE_INFO("Red channel depth: {0}");
}
#endif /* PD_PRINT_OPENGL_ATTRIBUTES */

#endif /* PD_DEBUG */

#undef space1
#undef space2
#undef space3
#undef space
#undef newl
#undef newlstr
#undef spacestr
