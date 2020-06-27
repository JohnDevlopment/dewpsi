#ifndef EVENT_H
#define EVENT_H

/** ***************************************************************************************************************
*
*/

#include "core.h"

#define EVENT_CLASS_TYPE(type)          static EventType GetStaticType() { return EventType::type; } \
                                        virtual EventType GetEventType() const override { GetStaticType(); } \
                                        virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)  virtual int GetCategoryFlags() const override { return category; }

namespace Dewpsi {
    /// Type of the event being processed.
    enum EventType {
        ET_None = 0,
        
        ET_WindowClose,
        ET_WindowResize,
        ET_WindowFocus,
        ET_WindowLostFocus,
        ET_WindowMoved,
        
        ET_AppTick,
        ET_AppUpdate,
        ET_AppRender,
        
        ET_KeyPressed,
        ET_KeyReleased,
        ET_KeyTyped,
        
        ET_MouseButtonPressed,
        ET_MouseButtonReleased,
        ET_MouseMoved,
        ET_MouseScrolled
    };
    
    /// Event category. Bitmask EventType to see what category it is in.
    enum EventCategory {
        EC_None = 0,
        EC_Application  = BIT(0),
        EC_Input        = BIT(1),
        EC_Keyboard     = BIT(2),
        EC_Mouse        = BIT(3),
        EC_MouseButton  = BIT(4)
    };
    
    /// Base event class -- defines every event.
    class Event {
    public:
        Event() = default;
        virtual ~Event() = default;
        
        /// Indicates whether the event was handled by the application.
        bool m_handled;
        
        /// Returns the type of event.
        virtual EventType GetEventType() const = 0;
        
        /// Returns the name of the event as a string.
        virtual const char* GetName() const = 0;
        
        /// Returns the event categories as a bit field.
        virtual int GetCategoryFlags() const = 0;
        
        /// Returns 
        virtual std::string ToString() const
        {
            return GetName();
        }
    };
    
    /// Dispatches events.
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
}

#endif /* EVENT_H */
