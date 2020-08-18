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

    /// A window event.
    class WindowEvent : public Event {
    public:
        /// Initialize with a window ID
        WindowEvent(PDuint32 id) : m_WindowID(id)
        {  }

        EVENT_CLASS_CATEGORY(EC_Application)

        /// Return the ID of the window in which the event was pushed.
        PDuint32 GetWindowID() const
        {
            return m_WindowID;
        }

    protected:
        PDuint32 m_WindowID;
    };

    /// Window resize event.
    class WindowResizeEvent : public WindowEvent {
    public:
        /// Initialize a @c %WindowResizeEvent with a width and height.
        WindowResizeEvent(PDuint32 id, PDint32 w, PDint32 h)
            : WindowEvent(id), m_uiWidth(w), m_uiHeight(h)
        {  }

        EVENT_CLASS_TYPE(ET_WindowResize)

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
    class WindowCloseEvent : public WindowEvent {
    public:
        /// Initialize with the ID of a window.
        WindowCloseEvent(PDuint32 id) : WindowEvent(id)
        {  }

        EVENT_CLASS_TYPE(ET_WindowClose)
    };

    /// @}
}

#endif /* DEWPSI_APPLICATIONEVENT_H */
