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

bool SDLInput::IsMouseButtonPressedImpl(MouseCode button)
{
    PDuint32 uiState = SDL_GetMouseState(nullptr, nullptr);
    PDuint32 uiMouse = Dewpsi2SDLMouseCode(button);

    return static_cast<bool>(SDL_BUTTON(uiMouse));
}

PDuint32 SDLInput::GetMouseStateImpl()
{
    PDuint32 uiState, uiRet = 0;
    uiState = SDL_GetMouseState(nullptr, nullptr);
}

std::pair<float, float> SDLInput::GetMousePositionImpl()
{
    std::pair<float, float> pos;
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
