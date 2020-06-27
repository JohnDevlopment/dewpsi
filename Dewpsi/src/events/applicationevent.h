#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H

#include "event.h"

namespace Dewpsi {
    class WindowResizeEvent : public Event {
        WindowResizeEvent() = default;
        
        EVENT_CLASS_TYPE(ET_WindowResize)
        EVENT_CLASS_CATEGORY(EC_Application)
    };
    
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;
        
        EVENT_CLASS_TYPE(ET_WindowClose)
        EVENT_CLASS_CATEGORY(EC_Application)
    };
}

#endif /* APPLICATIONEVENT_H */

