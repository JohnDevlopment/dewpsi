#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <core.h>
#include <event.h>
#include <SDL.h>
#include <window.h>

namespace Dewpsi {
    class SDL2Window : public Window {
    public:
        SDL2Window(const WindowProps& props);
        virtual ~SDL2Window();
        
        // update the SDL2 window
        virtual void Update() override;
        
        // get the width
        virtual uint32_t GetWidth() const override
        {
            return m_data.width;
        }
        
        // get the height
        virtual uint32_t GetHeight() const override
        {
            return m_data.height;
        }
        
        // set the event callback for the window
        virtual void SetEventCallback(const EventCallback& fn) override
        {
            m_data.callback = fn;
        }
        
        // sets the vsync flag
        virtual void SetVSync(bool bEnable) override;
        
        // returns true if vsync is enabled
        virtual bool IsVSync() const override;
        
        // returns a pointer to the actual window
        virtual void* GetNativeWindow() const override
        {
            return m_window;
        }
        
     private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
        
        static int OnEvent(void* udata, SDL_Event* event);
        
        struct WindowData {
            std::string title;
            unsigned int width, height;
            bool vsync;
            EventCallback callback;
            
            WindowData() = default;
            ~WindowData() = default;
            
            WindowData& operator=(const WindowProps& src)
            {
                title = src.title;
                height = src.height;
                width = src.width;
                vsync = false;
                
                return *this;
            }
        };
        
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        WindowData m_data;
    };
}

#endif /* SDLWINDOW_H */

