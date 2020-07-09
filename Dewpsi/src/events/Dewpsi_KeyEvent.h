#ifndef DEWPSI_KEYEVENT_H
#define DEWPSI_KEYEVENT_H

/**
*   @file       Dewpsi_KeyEvent.h
*   @brief      A header for the Dewpsi engine.
*
*   @defgroup   keyevents Keyboard events
*   Keyboard press and release events.
*
*   @ingroup    events
*/

#include <Dewpsi_Event.h>
#include <Dewpsi_Input.h>

namespace Dewpsi {
    /// @addtogroup keyevents
    /// @{
    
    /// Key event.
    class KeyEvent : public Event {
    public:
        /// Returns the key code
        KeyCode GetKeyCode() const
        { return m_keycode; }
        
    protected:
        /// Constructs a key event with a key code.
        KeyEvent(KeyCode key) : m_keycode(key)
        {  }
        
        EVENT_CLASS_CATEGORY(EC_Application|EC_Keyboard)
        
        KeyCode m_keycode;
    };
    
    /// Key pressed event.
    class KeyPressedEvent : public KeyEvent {
    public:
        /// Constructs a key-pressed event with a keycode and repeat count.
        KeyPressedEvent(KeyCode keycode, int repeatCount)
            : KeyEvent(keycode), m_iRepeatCount(repeatCount)
        {  }
        
        EVENT_CLASS_TYPE(ET_KeyPressed)
        
        /// Returns the repeat count.
        int GetRepeatCount() const
        { return m_iRepeatCount; }
        
        /// Returns a string denoting the key being pressed.
        virtual std::string ToString() const
        {
            std::stringstream ss;
            ss << "Key pressed: " << m_keycode << " (" << m_iRepeatCount << " repeats)";
            return ss.str();
        }
        
    private:
        int m_iRepeatCount;
    };
    
    /// Key released event.
    class KeyReleasedEvent : public KeyEvent {
    public:
        /// Constructs a key-released event with a keycode and repeat count.
        KeyReleasedEvent(KeyCode keycode)
            : KeyEvent(keycode)
        {  }
        
        EVENT_CLASS_TYPE(ET_KeyReleased)
        
        /// Returns a string denoting the key being pressed.
        virtual std::string ToString() const
        {
            std::stringstream ss;
            ss << "Key released: " << m_keycode;
            return ss.str();
        }
    };
    
    /// Key typed event.
    class KeyTypedEvent : public KeyEvent {
    public:
        /// Constructs a key-typed event with a keycode and repeat count.
        KeyTypedEvent(KeyCode keycode)
            : KeyEvent(keycode)
        {  }
        
        EVENT_CLASS_TYPE(ET_KeyTyped)
        
        /// Returns a string denoting the key being pressed.
        virtual std::string ToString() const
        {
            std::stringstream ss;
            ss << "Key typed: " << m_keycode;
            return ss.str();
        }
    };
    
    /// @}
}

#endif /* DEWPSI_KEYEVENT_H */
