#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Application.h"



const PDuint8* _KeyState = nullptr;

Dewpsi::KeyCode SDL2DewpsiKeyCode(int kc);
uint32_t Dewpsi2SDLKeyCode(Dewpsi::KeyCode code);

Dewpsi::MouseCode SDL2DewpsiMouseCode(int mc);
int Dewpsi2SDLMouseCode(Dewpsi::MouseCode mc);

namespace Dewpsi {

bool SDLInput::IsKeyPressedImpl(KeyCode key)
{
    if (! _KeyState)
    {
        PD_CORE_ASSERT(Application::Get().GetWindow().IsValid(), "Window not created");
        _KeyState = SDL_GetKeyboardState(nullptr);
    }

    uint32_t uiKey = Dewpsi2SDLKeyCode(key);

    return static_cast<bool>(_KeyState[(int)SDL_GetScancodeFromKey(uiKey)]);
}

KeyMod SDLInput::GetModStateImpl()
{
    SDL_Keymod mod = SDL_GetModState();
    PDuint32 uiRetMod = 0;

    if (mod & KMOD_SHIFT)
    {
        if (mod & KMOD_LSHIFT)
            uiRetMod |= (PDuint32) PD_MOD_LEFTSHIFT;
        if (mod & KMOD_RSHIFT)
            uiRetMod |= (PDuint32) PD_MOD_RIGHTSHIFT;
    }

    if (mod & KMOD_CTRL)
    {
        if (mod & KMOD_LCTRL)
            uiRetMod |= (PDuint32) PD_MOD_LEFTCONTROL;
        if (mod & KMOD_RCTRL)
            uiRetMod |= (PDuint32) PD_MOD_RIGHTCONTROL;
    }

    if (mod & KMOD_ALT)
    {
        if (mod & KMOD_LALT)
            uiRetMod |= (PDuint32) PD_MOD_LEFTALT;
        if (mod & KMOD_RALT)
            uiRetMod |= (PDuint32) PD_MOD_RIGHTALT;
    }

#ifndef _WIN32
    if (mod & KMOD_GUI)
    {
        if (mod & KMOD_LGUI)
            uiRetMod |= (PDuint32) PD_MOD_LEFTGUI;
        if (mod & KMOD_RGUI)
            uiRetMod |= (PDuint32) PD_MOD_RIGHTGUI;
    }
#endif

    if (mod & KMOD_NUM)
        uiRetMod |= (PDuint32) PD_MOD_NUMLOCK;

    if (mod & KMOD_CAPS)
        uiRetMod |= (PDuint32) PD_MOD_CAPSLOCK;

    return static_cast<KeyMod>(uiRetMod);
}

/*typedef enum
{
    KMOD_NONE = 0x0000,
    KMOD_LSHIFT = 0x0001,
    KMOD_RSHIFT = 0x0002,
    KMOD_LCTRL = 0x0040,
    KMOD_RCTRL = 0x0080,
    KMOD_LALT = 0x0100,
    KMOD_RALT = 0x0200,
    KMOD_LGUI = 0x0400,
    KMOD_RGUI = 0x0800,
    KMOD_NUM = 0x1000,
    KMOD_CAPS = 0x2000,
    KMOD_MODE = 0x4000,
    KMOD_RESERVED = 0x8000
} SDL_Keymod;

#define KMOD_CTRL   (KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT  (KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT    (KMOD_LALT|KMOD_RALT)
#define KMOD_GUI    (KMOD_LGUI|KMOD_RGUI)*/

bool SDLInput::IsMouseButtonPressedImpl(MouseCode button)
{
    PDuint32 uiState = SDL_GetMouseState(nullptr, nullptr);
    PDuint32 uiMouse = Dewpsi2SDLMouseCode(button);

    //static constexpr PDuint32

    return static_cast<bool>(SDL_BUTTON(uiMouse));
}

PDuint32 SDLInput::GetMouseStateImpl()
{
    PDuint32 uiState, uiRet = 0;
    uiState = SDL_GetMouseState(nullptr, nullptr);

    if (uiState & SDL_BUTTON(SDL_BUTTON_LEFT))
        uiRet |= (PDuint32) PD_MOUSEBUTTON_LEFT;

    if (uiState & SDL_BUTTON(SDL_BUTTON_RIGHT))
        uiRet |= (PDuint32) PD_MOUSEBUTTON_RIGHT;

    if (uiState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        uiRet |= (PDuint32) PD_MOUSEBUTTON_MIDDLE;

    return uiRet;
}

Pair<float, float> SDLInput::GetMousePositionImpl()
{
    Pair<float, float> pos;
    int iMx, iMy;
    SDL_GetMouseState(&iMx, &iMy);

    pos.first = (float) iMx;
    pos.second = (float) iMy;

    return pos;
}

float SDLInput::GetMouseXImpl()
{
    return GetMousePositionImpl().first;
}

float SDLInput::GetMouseYImpl()
{
    return GetMousePositionImpl().second;
}

}
