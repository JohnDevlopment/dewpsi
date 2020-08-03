#ifndef SDLWINDOW_H
#define SDLWINDOW_H

/**
*   @file   Dewpsi_SDLWindow.h
*   @brief  A header for the Dewpsi engine.
*   Contains the SDL2Window definition. This file handles the SDL-specific
*   deriviation of the Window abstract class.
*
*   @ingroup sdl
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Event.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_Color.h>
#include <SDL.h>

/// Retrieves the @a window member from a Window.
#define PD_GET_SDL_WINDOW(winp)  reinterpret_cast<::Dewpsi::SDLNativeWindow*>((winp)->GetNativeWindow())->window

namespace Dewpsi {
    /// @addtogroup sdl
    /// @{
    
    /// Type returned by SDL2Window::GetNativeWindow().
    struct SDLNativeWindow {
        SDL_Window* window;     ///< SDL window
        SDL_Renderer* renderer; ///< SDL renderer
        SDL_GLContext context;  ///< OpenGL context
    };
    
    /// SDL2 window abstraction
    class SDL2Window : public Window {
    public:
        /// Constructs a window with the properties given.
        SDL2Window(const WindowProps& props);
        
        /// Destroys the window.
        virtual ~SDL2Window();
        
        /// Update the SDL2 window.
        virtual void OnUpdate() override;
        
        /// Get the width of the window.
        virtual uint32_t GetWidth() const override
        {
            return m_data.width;
        }
        
        /// Get the height of the window.
        virtual uint32_t GetHeight() const override
        {
            return m_data.height;
        }
        
        /// Set the event callback for the window.
        virtual void SetEventCallback(const EventCallback& fn) override
        {
            m_data.callback = fn;
        }
        
        /** Enable/disable vsync for the window.
        *   If using SDL's builtin functionality, this has no effect.
        *   But if using OpenGL, SDL_GL_SwapInterval() is called with
        *   the integer equivelent of @a bEnable.
        *
        *   @param  bEnable True to enable, false to disable.
        */
        virtual void SetVSync(bool bEnable) override;
        
        /// Returns true if vsync is enabled.
        virtual bool IsVSync() const override;
        
        /// Returns a pointer to the actual window, type @c SDLNativeWindow.
        virtual void* GetNativeWindow() const override
        {
            return (void*) &m_native;
        }
        
        /// Set the clear color.
        /// @todo move to a renderer class
        virtual void SetClearColor(const Color& color) override; // TODO: move to a renderer class
        
        /// Clears the window.
        /// @todo move to a renderer class
        virtual void Clear() override; // TODO: move to a renderer class
        
        /// Get the window display format of type @c SDL_PixelFormatEnum.
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
        Color m_clearColor;
    };
    
    /// @}
}

#endif /* SDLWINDOW_H */

