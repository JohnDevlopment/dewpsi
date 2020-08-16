// dear imgui: Platform Binding for SDL2
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)
// (Info: SDL2 is a cross-platform general purpose library for handling windows, inputs, graphics context creation, etc.)
// (Requires: SDL 2.0. Prefer SDL 2.0.4+ for full feature support.)

#define _PD_DEBUG_BREAKS
#include "Dewpsi_Debug.h"

#include "Dewpsi_Core.h"
#include "Dewpsi_WhichOS.h"
#include "imgui.h"
#include "Dewpsi_ImGui_SDL.h"
#include "Dewpsi_String.h"

// SDL
#include <SDL.h>
#include <SDL_syswm.h>
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#define SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE    SDL_VERSION_ATLEAST(2,0,4)
#define SDL_HAS_VULKAN                      SDL_VERSION_ATLEAST(2,0,6)

void (* ImGui_UpdateIntState)(SDL_Window*) = nullptr;

// Data
static SDL_Window*  g_Window = NULL;
static Uint64       g_uiTime = 0;
static bool         g_baMousePressed[3] = { false, false, false };
static SDL_Cursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = {};
static char*        g_ClipboardTextData = NULL;
static bool         g_bMouseCanUseGlobalState = true;

static const char* ImGui_ImplSDL2_GetClipboardText(void*)
{
    if (g_ClipboardTextData)
        SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = SDL_GetClipboardText();
    return g_ClipboardTextData;
}

static void ImGui_ImplSDL2_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}

static void ImGui_ImplSDL2_SetViewport(int x, int y, int w, int h)
{
    glViewport(x, y, (GLsizei) w, (GLsizei) h);
}

static void ImGui_ImplSDL2_UpdateIntState(SDL_Window* window)
{
    int w, h, dw, dh;
    ImGuiIO& io = ImGui::GetIO();

    SDL_GetWindowSize(window, &w, &h);
    io.DisplaySize = ImVec2((float)w, (float)h);

    SDL_GL_GetDrawableSize(window, &dw, &dh);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float) dw / w, (float) dh / h);
}

static bool ImGui_ImplSDL2_Init(SDL_Window* window)
{
    g_Window = window;

    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl_sdl";

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable keyboard controls

    ImGui_UpdateIntState = ImGui_ImplSDL2_UpdateIntState;

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab]         = (int) PD_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]   = (int) PD_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow]  = (int) PD_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]     = (int) PD_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow]   = (int) PD_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp]      = (int) PD_KEY_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown]    = (int) PD_KEY_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home]        = (int) PD_KEY_HOME;
    io.KeyMap[ImGuiKey_End]         = (int) PD_KEY_END;
    io.KeyMap[ImGuiKey_Insert]      = (int) PD_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete]      = (int) PD_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace]   = (int) PD_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]       = (int) PD_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter]       = (int) PD_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape]      = (int) PD_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = (int) PD_KEY_KPENTER;
    io.KeyMap[ImGuiKey_A]           = (int) PD_KEY_A;
    io.KeyMap[ImGuiKey_C]           = (int) PD_KEY_C;
    io.KeyMap[ImGuiKey_V]           = (int) PD_KEY_V;
    io.KeyMap[ImGuiKey_X]           = (int) PD_KEY_X;
    io.KeyMap[ImGuiKey_Y]           = (int) PD_KEY_Y;
    io.KeyMap[ImGuiKey_Z]           = (int) PD_KEY_Z;

    io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
    io.ClipboardUserData = NULL;

    // Load mouse cursors
    g_MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    g_MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    g_MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    g_MouseCursors[ImGuiMouseCursor_NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

    // Check and store if we are on Wayland
    g_bMouseCanUseGlobalState = strncmp(SDL_GetCurrentVideoDriver(), "wayland", 7) != 0;

#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    io.ImeWindowHandle = wmInfo.info.win.window;
#else
    (void)window;
#endif

    // this function is called whenever the window's resized
    ImGui_ImplSDL2_UpdateIntState(window);

    return true;
}

bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window, void* sdl_gl_context)
{
    (void)sdl_gl_context; // Viewport branch will need this.
    return ImGui_ImplSDL2_Init(window);
}

bool ImGui_ImplSDL2_InitForVulkan(SDL_Window* window)
{
#if !SDL_HAS_VULKAN
    IM_ASSERT(0 && "Unsupported");
#endif
    return ImGui_ImplSDL2_Init(window);
}

bool ImGui_ImplSDL2_InitForD3D(SDL_Window* window)
{
#if !defined(_WIN32)
    IM_ASSERT(0 && "Unsupported");
#endif
    return ImGui_ImplSDL2_Init(window);
}

bool ImGui_ImplSDL2_InitForMetal(SDL_Window* window)
{
    return ImGui_ImplSDL2_Init(window);
}

void ImGui_ImplSDL2_Shutdown()
{
    g_Window = NULL;

    // Destroy last known clipboard data
    if (g_ClipboardTextData)
        SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = NULL;

    // Destroy SDL mouse cursors
    for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
        SDL_FreeCursor(g_MouseCursors[cursor_n]);
    Dewpsi::String::MemSet(g_MouseCursors, 0, sizeof(g_MouseCursors));
}

static void ImGui_ImplSDL2_UpdateMousePosAndButtons()
{
    ImGuiIO& io = ImGui::GetIO();

    g_baMousePressed[0] = g_baMousePressed[1] = g_baMousePressed[2] = false;

    int iMx, iMy;
    PDuint32 uiMouseButtons = SDL_GetMouseState(&iMx, &iMy);

#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE && !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)
    SDL_Window* pFocusedWindow = SDL_GetKeyboardFocus();
    if (g_Window == pFocusedWindow)
    {
        if (g_bMouseCanUseGlobalState)
        {
            int iWx, iWy;
            SDL_GetWindowPosition(pFocusedWindow, &iWx, &iWy);
            SDL_GetGlobalMouseState(&iMx, &iMy);
            iMx -= iWx;
            iMy -= iWy;
        }
        io.MousePos = ImVec2((float) iMx, (float) iMy);
    }
    bool bAnyMouseDown = ImGui::IsAnyMouseDown();
    SDL_CaptureMouse(bAnyMouseDown ? SDL_TRUE : SDL_FALSE);
#else
    if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS)
        io.MousePos = ImVec2((float) iMx, (float) iMy);
#endif
}

static void ImGui_ImplSDL2_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        SDL_ShowCursor(SDL_FALSE);
    }
    else
    {
        // Show OS mouse cursor
        SDL_SetCursor(g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
        SDL_ShowCursor(SDL_TRUE);
    }
}

static void ImGui_ImplSDL2_UpdateGamepads()
{
    ImGuiIO& io = ImGui::GetIO();
    Dewpsi::String::MemSet(io.NavInputs, 0, sizeof(io.NavInputs));
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
        return;

    // Get gamepad
    SDL_GameController* game_controller = SDL_GameControllerOpen(0);
    if (!game_controller)
    {
        io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
        return;
    }

    // Update gamepad inputs
    #define MAP_BUTTON(NAV_NO, BUTTON_NO)       { io.NavInputs[NAV_NO] = (SDL_GameControllerGetButton(game_controller, BUTTON_NO) != 0) ? 1.0f : 0.0f; }
    #define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
    const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
    MAP_BUTTON(ImGuiNavInput_Activate,      SDL_CONTROLLER_BUTTON_A);               // Cross / A
    MAP_BUTTON(ImGuiNavInput_Cancel,        SDL_CONTROLLER_BUTTON_B);               // Circle / B
    MAP_BUTTON(ImGuiNavInput_Menu,          SDL_CONTROLLER_BUTTON_X);               // Square / X
    MAP_BUTTON(ImGuiNavInput_Input,         SDL_CONTROLLER_BUTTON_Y);               // Triangle / Y
    MAP_BUTTON(ImGuiNavInput_DpadLeft,      SDL_CONTROLLER_BUTTON_DPAD_LEFT);       // D-Pad Left
    MAP_BUTTON(ImGuiNavInput_DpadRight,     SDL_CONTROLLER_BUTTON_DPAD_RIGHT);      // D-Pad Right
    MAP_BUTTON(ImGuiNavInput_DpadUp,        SDL_CONTROLLER_BUTTON_DPAD_UP);         // D-Pad Up
    MAP_BUTTON(ImGuiNavInput_DpadDown,      SDL_CONTROLLER_BUTTON_DPAD_DOWN);       // D-Pad Down
    MAP_BUTTON(ImGuiNavInput_FocusPrev,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
    MAP_BUTTON(ImGuiNavInput_FocusNext,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
    MAP_BUTTON(ImGuiNavInput_TweakSlow,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
    MAP_BUTTON(ImGuiNavInput_TweakFast,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
    MAP_ANALOG(ImGuiNavInput_LStickLeft,    SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiNavInput_LStickRight,   SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiNavInput_LStickUp,      SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
    MAP_ANALOG(ImGuiNavInput_LStickDown,    SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);

    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    #undef MAP_BUTTON
    #undef MAP_ANALOG
}

void ImGui_ImplSDL2_NewFrame(SDL_Window* window)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
    static Uint64 s_uiFrequency = SDL_GetPerformanceFrequency();

    Uint64 uiCurrentTime = SDL_GetPerformanceCounter();

    io.DeltaTime = g_uiTime > 0 ? (float)((double)(uiCurrentTime - g_uiTime) / s_uiFrequency) : (float)(1.0f / 60.0f);
    g_uiTime = uiCurrentTime;

    ImGui_ImplSDL2_UpdateMousePosAndButtons();
    ImGui_ImplSDL2_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    ImGui_ImplSDL2_UpdateGamepads();
}
