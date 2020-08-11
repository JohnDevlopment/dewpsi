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
        /// Initialize a @c %WindowResizeEvent with a width and height.
        WindowResizeEvent(PDint32 w, PDint32 h)
            : m_uiWidth(w), m_uiHeight(h)
        {  }

        EVENT_CLASS_TYPE(ET_WindowResize)
        EVENT_CLASS_CATEGORY(EC_Application)

        /// Returns the new display width.
        PDint32 GetWidth() const
        {
            return m_uiWidth;
        }

        /// Returns the new display height.
        PDint32 GetHeight() const
        {
            return m_uiHeight;
        }

    private:
        PDint32 m_uiWidth;
        PDint32 m_uiHeight;
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
