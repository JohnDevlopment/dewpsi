#ifndef DEWPSI_INPUT_H
#define DEWPSI_INPUT_H

/**
*   @file   Dewpsi_Input.h
*   @brief  @doxfb
*   Contains the @c %Input class.
*
*   @defgroup input Input Polling
*   The Input class provides an interface to input polling functions.
*   @ingroup core
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_KeyCode.h>
#include <Dewpsi_MouseCode.h>

namespace Dewpsi {
    /** Global input class.
    *   This merely provides an interface to a platform-specific implementation
    *   of input-polling functions.
    *   @ingroup input
    */
    class Input {
    protected:
        Input() = default;
    public:
        virtual ~Input() = default;

        Input(const Input& src) = delete;
        Input& operator=(const Input& rhs) = delete;

        /// Create a static instance of @c %Input.
        static Scope<Input> Create();

        /** Returns true if the keyboard key @a key is pressed.
        *   @param  key     A KeyCode
        *   @return         True if the key is pressed down, false otherwise
        */
        static bool IsKeyPressed(KeyCode key)
        {
            return s_Instance->IsKeyPressedImpl(key);
        }

        static KeyMod GetModState()
        {
            return s_Instance->GetModStateImpl();
        }

        /** Returns true if the mouse button @a button is pressed.
        *   @param  button  A MouseCode
        *   @return         True if the button is pressed down, false otherwise
        */
        static bool IsMouseButtonPressed(MouseCode button)
        {
            return s_Instance->IsMouseButtonPressedImpl(button);
        }

        /** Get the mouse state.
        *   The return value is a field of bits representing what mouse
        *   buttons have been pressed.
        */
        static PDuint32 GetMouseState()
        {
            return s_Instance->GetMouseStateImpl();
        }

        /** Returns the current X and Y position of the mouse.
        *   The return value is a pair. The @a first member is
        *   the X position of the mouse and @a second is the Y
        *   position.
        */
        static std::pair<float, float> GetMousePosition()
        {
            return s_Instance->GetMousePositionImpl();
        }

        /// Return the X position of the mouse.
        static float GetMouseX() {
            return s_Instance->GetMouseXImpl();
        }

        /// Return the Y position of the mouse.
        static float GetMouseY() {
            return s_Instance->GetMouseYImpl();
        }

    protected:
        virtual bool IsKeyPressedImpl(KeyCode key) = 0;
        virtual KeyMod GetModStateImpl() = 0;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
        virtual PDuint32 GetMouseStateImpl() = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

    private:
        static Scope<Input> s_Instance;
    };
}

#endif /* DEWPSI_INPUT_H */
