#ifndef DEWPSI_SDLINPUT_H
#define DEWPSI_SDLINPUT_H

#include <Dewpsi_Input.h>

namespace Dewpsi {
    class SDLInput : public Input {
    protected:
        virtual bool IsKeyPressedImpl(KeyCode key) override;
        virtual KeyMod GetModStateImpl() override;
        virtual bool IsMouseButtonPressedImpl(MouseCode button);
        virtual PDuint32 GetMouseStateImpl() override;
        virtual Pair<float, float> GetMousePositionImpl() override;
        virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
    };
}

#endif /* DEWPSI_SDLINPUT_H */
