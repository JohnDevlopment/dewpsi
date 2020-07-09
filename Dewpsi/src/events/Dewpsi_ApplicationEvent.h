#ifndef DEWPSI_APPLICATIONEVENT_H
#define DEWPSI_APPLICATIONEVENT_H

/**
*   @file       Dewpsi_Event.h
*   @brief      A header for the Dewpsi engine.
*   Contains application window events, such as resize and close events.
*
*   @defgroup   windowevents    Window events
*   Window resize and close events.
*   @ingroup    events
*/

#include "Dewpsi_Event.h"

/// @ingroup events
namespace Dewpsi {
    /**
    *   @addtogroup windowevents
    *   @{
    */
    
    /// Window resize event.
    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent() = default;
        
        EVENT_CLASS_TYPE(ET_WindowResize)
        EVENT_CLASS_CATEGORY(EC_Application)
    };
    
    /// Close-window event.
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;
        
        EVENT_CLASS_TYPE(ET_WindowClose)
        EVENT_CLASS_CATEGORY(EC_Application)
    };
    
    /// @}
}

#endif /* DEWPSI_APPLICATIONEVENT_H */
