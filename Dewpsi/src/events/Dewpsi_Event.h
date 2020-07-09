#ifndef DEWPSI_EVENT_H
#define DEWPSI_EVENT_H

/**
*   @defgroup events Events
*   A list of all events supported by Dewpsi.
*   @ingroup core
*/

/**
*   @file       Dewpsi_Event.h
*   @brief      A header for the Dewpsi engine.
*   Contains core event enumerations and classes.
*
*   @ingroup    events
*/

#include <Dewpsi_Core.h>

/// Define the type of an event.
#define EVENT_CLASS_TYPE(type)          static EventType GetStaticType() { return EventType::type; } \
                                        virtual EventType GetEventType() const override { GetStaticType(); } \
                                        virtual const char* GetName() const override { return #type; }

/// Assign one or more categories to an event, combined with '|'.
#define EVENT_CLASS_CATEGORY(category)  virtual int GetCategoryFlags() const override { return category; }

namespace Dewpsi {
    /// @addtogroup events
    /// @{
    
    /// Type of the event being processed.
    enum EventType {
        ET_None = 0,
        
        ET_WindowClose,         ///< Window closed
        ET_WindowResize,        ///< Window resized
        ET_WindowFocus,         ///< Window gains focus
        ET_WindowLostFocus,     ///< Window loses focus
        ET_WindowMoved,         ///< Window moved
        
        ET_AppTick,             ///< App tick event
        ET_AppUpdate,           ///< App update event
        ET_AppRender,           ///< App render event
        
        ET_KeyPressed,          ///< Key pressed
        ET_KeyReleased,         ///< Key released
        ET_KeyTyped,            ///< Key typed
        
        ET_MouseButtonPressed,  ///< Mouse button pressed
        ET_MouseButtonReleased, ///< Mouse button released
        ET_MouseMoved,          ///< Mouse motion
        ET_MouseScrolled        ///< Mouse scrolled
    };
    
    /// Event category.
    /// Bitmask to see what category event is in.
    enum EventCategory {
        EC_None = 0,
        EC_Application  = BIT(0),   ///< Application events
        EC_Input        = BIT(1),   ///< Input devices
        EC_Keyboard     = BIT(2),   ///< Keyboard events
        EC_Mouse        = BIT(3),   ///< Mouse events
        EC_MouseButton  = BIT(4)    ///< Mouse button events
    };
    
    /// Base event class -- defines every event
    class Event {
    public:
        Event() : m_handled(false) {  }
        virtual ~Event() = default;
        
        /// Indicates whether the event was handled by the application.
        bool m_handled;
        
        /// Returns the type of event.
        virtual EventType GetEventType() const = 0;
        
        /// Returns the name of the event as a string.
        virtual const char* GetName() const = 0;
        
        /// Returns the event categories as a bit field.
        virtual int GetCategoryFlags() const = 0;
        
        /// Returns a string describing the event.
        virtual std::string ToString() const
        {
            return GetName();
        }
    };
    
    /// Event dispatcher
    class EventDispatcher {
    public:
        /// Sets the event to be dispatched.
        EventDispatcher(Event& e) : m_event(e)
        {  }
        ~EventDispatcher() = default;
        
        /// Dispatches the event with function of type @c T.
        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_event.GetEventType() == T::GetStaticType())
            {
                m_event.m_handled = func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }
        
    private:
        Event& m_event;
    };
    
    /// Prints event name to stdout.
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        os << e.ToString();
    }
    
    /// @}
}

#endif /* DEWPSI_EVENT_H */
