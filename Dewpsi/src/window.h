#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"
#include "event.h"

namespace Dewpsi {
    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;
        
        WindowProps(const std::string& sTitle = "Dewpsi Engine",
                    uint32_t uiWidth, uint32_t uiHeight)
            : title(sTitle), width(uiWidth), height(uiHeight)
        {  }
    };
    
    class Window {
    public:
        using EventCallback = std::function<void(Event&)>;
        
        Window() = default;
        virtual ~Window() = default;
        
        virtual void Update() = 0;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        
        virtual void SetEventCallback(const EventCallback& fn) = 0;
        virtual void SetVSync(bool bEnable) = 0;
        virtual bool IsVSync() const = 0;
        
        virtual void* GetNativeWindow() const = 0;
        
        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };
}

#endif /* WINDOW_H */
