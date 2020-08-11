#ifndef DEWPSI_MOUSEEVENT_H
#define DEWPSI_MOUSEEVENT_H

/**
*   @file       Dewpsi_MouseEvent.h
*   @brief      A header for the Dewpsi engine.
*   Contains mouse events.
*
*   @defgroup   mouse   Mouse events
*   Mouse moved events.
*   @ingroup    events
*/

#include <Dewpsi_Event.h>
#include <Dewpsi_Input.h>

namespace Dewpsi {
    /// @addtogroup mouse
    /// @{
    
    /// Mouse moved event.
    /// This event signifies that the mouse has moved to a new location.
    class MouseMovedEvent : public Event {
    public:
        /// Creates an event with the specified new location.
        MouseMovedEvent(float x, float y)
            : m_fX(x), m_fY(y)
        {  }
        
        /// Return the X offset
        float GetX() const
        { return m_fX; }
        
        /// Return the y offset
        float GetY() const
        { return m_fY; }
        
        // Returns a string with the new location of the mouse pointer.
        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "mouse moved to (" << m_fX << ',' << m_fY << "), " << GetName();
            return ss.str();
        }
        
        EVENT_CLASS_TYPE(ET_MouseMoved)
        EVENT_CLASS_CATEGORY(EC_Mouse | EC_Input)
        
    private:
        float m_fX;
        float m_fY;
    };
    
    /// Mouse scrolled event.
    /// This event signifies that the mouse has been scrolled.
    class MouseScrolledEvent : public Event {
    public:
        /// Creates an event with the specified new location.
        MouseScrolledEvent(float xoffset, float yoffset)
            : m_fXOffset(xoffset), m_fYOffset(yoffset)
        {  }
        
        /// Return the X offset
        float GetXOffset() const
        { return m_fXOffset; }
        
        /// Return the y offset
        float GetYOffset() const
        { return m_fYOffset; }
        
        // Returns a string with the new location of the mouse pointer.
        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "mouse scrolled by (" << m_fXOffset << ',' << m_fYOffset << "), " << GetName();
            return ss.str();
        }
        
        EVENT_CLASS_TYPE(ET_MouseScrolled)
        EVENT_CLASS_CATEGORY(EC_Mouse | EC_Input)
        
    private:
        float m_fXOffset;
        float m_fYOffset;
    };
    
    /** Mouse button event.
    *   Base class of mouse pressed and released events.
    */
    class MouseButtonEvent : public Event {
    public:
        EVENT_CLASS_CATEGORY(EC_Application|EC_Mouse)
        
        /// Returns the mouse button that was invoked.
        MouseCode GetMouseCode() const
        {
            return m_eCode;
        }
        
    protected:
        MouseButtonEvent(MouseCode code) : m_eCode(code)
        {  }
        
        MouseCode m_eCode;
    };
    
    /** Mouse pressed event.
    *   Dispatched whenever a mouse button is pressed down.
    */
    class MousePressedEvent : public MouseButtonEvent {
    public:
        /// Constructs a mouse button event mouse button @a code.
        MousePressedEvent(MouseCode code) : MouseButtonEvent(code)
        {  }
        
        /// Returns a string representation of the mouse code.
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MousePressedEvent: " << m_eCode;
            return ss.str();
        }
        
        EVENT_CLASS_TYPE(ET_MouseButtonPressed)
    };
    
    /** Mouse released event.
    *   Dispatched whenever a mouse button is released.
    */
    class MouseReleasedEvent : public MouseButtonEvent {
    public:
        /// Constructs a mouse button event mouse button @a code.
        MouseReleasedEvent(MouseCode code) : MouseButtonEvent(code)
        {  }
        
        /// Returns a string representation of the mouse code.
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseReleasedEvent: " << m_eCode;
            return ss.str();
        }
        
        EVENT_CLASS_TYPE(ET_MouseButtonReleased)
    };
    
    /// @}
}

#endif /* DEWPSI_MOUSEEVENT_H */
