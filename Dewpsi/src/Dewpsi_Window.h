#ifndef DEWPSI_WINDOW_H
#define DEWPSI_WINDOW_H

/**
*   @file       Dewpsi_Window.h
*   @brief      A header for the Dewpsi engine.
*
*   @defgroup   windows Windows
*   A series of functions that create, delete, and manipulate windows using a
*   platform-indepedent interface.
*   @ingroup    core
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Event.h>

/**
*   @def    PD_WINDOWPOS_CENTERED
*   @brief  Indicates that a coordinate should be centered.
*
*   @def    PD_WINDOWPOS_CENTERED_DISPLAY(x)
*   @brief  Indicates that a coordinate should be centered on a given display.
*
*   @def    PD_WINDOWPOS_CENTERED_MASK
*   @brief  Mask used in PD_WINDOWPOS_CENTERED_DISPLAY macro.
*
*   @def    PD_WINDOWPOS_ISCENTERED(x)
*   @brief  Returns non-zero if the coordinate is centered.
*/
#define PD_WINDOWPOS_CENTERED_MASK          0x2FFF0000u
#define PD_WINDOWPOS_CENTERED_DISPLAY(x)    (PD_WINDOWPOS_CENTERED_MASK|(x))
#define PD_WINDOWPOS_CENTERED               PD_WINDOWPOS_CENTERED_DISPLAY(0)
#define PD_WINDOWPOS_ISCENTERED(x)          (((x)&0xFFFF0000) == PD_WINDOWPOS_CENTERED_MASK)

/// @ingroup windows
namespace Dewpsi {
    /// Window/renderer creation flags.
    /// @ingroup windows
    enum WindowFlags : uint32_t {
        WindowFullscreen        = BIT(0),   ///< Generated window is in fullscreen
        WindowFullDesktop       = BIT(1),   ///< Create a fullscreen window at desktop resolution
        WindowOpenGL            = BIT(2),   ///< Window is usable with an OpenGL context
        WindowVulkan            = BIT(3),   ///< Window is usable with a Vulkan instance
        WindowShown             = BIT(4),   ///< Window is visible
        WindowHidden            = BIT(5),   ///< Window is not visible
        WindowBorderless        = BIT(6),   ///< Window is without a border
        WindowResizable         = BIT(7),   ///< Window can be resized
        WindowMinimized         = BIT(8),   ///< Window is minimized
        WindowMaximized         = BIT(9),   ///< Window is maximized
        WindowInputGrabbed      = BIT(10),  ///< Window has grabbed input focus
        WindowInputFocus        = BIT(11),  ///< Window has input focus
        WindowMouseFocus        = BIT(12),  ///< Window has mouse focus
        WindowForeign           = BIT(13),  ///< Window is not created with SDL
        WindowAllowHighDPI      = BIT(14),  ///< Window should be created in high-DPI mode if supported
        WindowMouseCapture      = BIT(15),  ///< Window has mouse captured (unreleated to WindowInputGrabbed)
        WindowAlwaysOnTop       = BIT(16),  ///< Window is always above others
        WindowSkipTaskbar       = BIT(17),  ///< Window should not be added to the taskbar
        WindowUtility           = BIT(18),  ///< Window should be treated as a utility window
        WindowTooltip           = BIT(19),  ///< Window should be treated as a tooltop
        WindowPopupMenu         = BIT(20),  ///< Window should be treated as a popup window
        
        RendererSoftware        = BIT(21),  ///< The renderer is a software fallback
        RendererAccelerated     = BIT(22),  ///< The renderer uses hardware acceleration
        RendererVSync           = BIT(23),  ///< Renders are synchronized with the refresh rate
        RendererTargetTexture   = BIT(24)   ///< The renderer supports rendering to texture
    };
    
    /** A structure that contains the properties
    *   of the window to be created.
    *
    *   @ingroup windows
    */
    struct WindowProps {
        std::string title; ///< The title of the window
        uint32_t x;        ///< The X position of the window
        uint32_t y;        ///< The Y position of the window
        uint32_t width;    ///< The width in pixels
        uint32_t height;   ///< The height in pixels
        uint32_t flags;    ///< Window/renderer flags
        int32_t  index;    ///< Rendering device index (or -1 for first available device that supports all options)
        
        /// Constructs an empty structure
        WindowProps() : title(), width(0), height(0), flags(0), index(0)
        {  }
        
        /// Constructs a copy of the other structure
        WindowProps(const WindowProps& src)
            : title(src.title), width(src.width), height(src.height),
              flags(src.flags), index(src.index)
        {  }
    };
    
    /** A class that abstractly represents the application window.
    *   @ingroup windows
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

#endif /* DEWPSI_WINDOW_H */
