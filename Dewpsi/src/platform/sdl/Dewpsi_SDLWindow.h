#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <Dewpsi_Core.h>
#include <Dewpsi_Event.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_Color.h>
#include <SDL.h>

namespace Dewpsi {
    struct SDLNativeWindow {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_GLContext context;
    };
    
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
            return (void*) &m_native;
        }
        
        // set clear color
        virtual void SetClearColor(const Color& color) override;
        
        // get window data
        uint32_t GetFormat() const
        { return m_data.format; }
        
     private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
        
        static int OnEvent(void* udata, SDL_Event* event);
        
        struct WindowData {
            std::string title;
            uint32_t width;
            uint32_t height;
            uint32_t format;
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
        SDL_GLContext m_context;
        WindowData m_data;
        SDLNativeWindow m_native;
    };
}

#endif /* SDLWINDOW_H */

