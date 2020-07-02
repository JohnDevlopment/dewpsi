#ifndef WINDOW_H
#define WINDOW_H

/**
*   @file       window.h
*   @brief      A header for the Dewpsi engine.
*
*   @defgroup   windows Windows
*   A series of functions that create, delete, and manipulate windows using a
*   platform-indepedent interface.
*   @ingroup    core
*
*   @{
*/

#include <core.h>
#include <event.h>

namespace Dewpsi {
    /** A structure that contains the properties
    *   of the window to be created.
    *
    *   @note   The members @a flags1 and @a flags2 are used
    *           differently based on the platform.
    *
    *   @par    Linux Platforms
    *           The @a flags array is four elements long,
    *           each a 32-bit integer. Each element is
    *           used as follows:
    *
    *           - flags[0] = 6th parameter of SDL_CreateWindow(),
    *                        window creation flags
    *           - flags[1] = 3rd parameter of SDL_CreateRenderer(),
    *                        renderer creation flags
    *           - flags[2] = second parameter of SDL_CreateRenderer(),
    *                        index of the renderer to initialize
    */
    struct WindowProps {
        std::string title; ///< The title of the window
        uint32_t width;    ///< The width in pixels
        uint32_t height;   ///< The height in pixels
        uint32_t flags[4]; ///< Array of 32 bit integers used as flags
        
        /// Constructs an empty structure
        WindowProps() : title(), width(0), height(0), flags{0}
        {  }
        
        /// Constructs a copy of the other structure
        WindowProps(const WindowProps& src)
            : title(src.title), width(src.width), height(src.height),
              flags {src.flags[0], src.flags[1], src.flags[2], src.flags[3]}
        {  }
    };
    
    /** A class that abstractly represents the application window.
    */
    class Window {
    public:
        using EventCallback = std::function<void(Event&)>;
        
        /// Constructs an empty window.
        Window() = default;
        
        // Destroys the window.
        virtual ~Window() = default;
        
        /// Update the window.
        virtual void Update() = 0;
        
        /// Obtain the window width.
        virtual uint32_t GetWidth() const = 0;
        
        /// Obtain the window height.
        virtual uint32_t GetHeight() const = 0;
        
        /** Sets the event callback.
        *   @param  fn  The event function to set.
        */
        virtual void SetEventCallback(const EventCallback& fn) = 0;
        
        /** Enable/disable vsync.
        *   @param  bEnable Whether to enable vsync.
        */
        virtual void SetVSync(bool bEnable) = 0;
        
        /// Returns @c true or @c false depending on whether vsync is enabled.
        virtual bool IsVSync() const = 0;
        
        /// Returns a pointer to the platform-specific window.
        virtual void* GetNativeWindow() const = 0;
        
        /// Creates an instance of the window.
        static Scope<Window> Create(const WindowProps& props);
    };
}

/// @}

#endif /* WINDOW_H */
