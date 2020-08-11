#ifndef DEWPSI_SDLINPUT_H
#define DEWPSI_SDLINPUT_H

/**
*   @file   Dewpsi_SDLInput.h
*   @brief  @doxfb
*   Contains SDL-specific input polling functions.
*
*   @ingroup sdl
*/

#include <Dewpsi_Input.h>

namespace Dewpsi {
    /** An SDL-specific window.
    *   @ingroup sdl
    */
    class SDLInput : public Input {
    protected:
        virtual bool IsKeyPressedImpl(KeyCode key) override;
        virtual bool IsMouseButtonPressedImpl(MouseCode button);
        virtual PDuint32 GetMouseStateImpl() override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
        virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
    };
}

#endif /* DEWPSI_SDLINPUT_H */
