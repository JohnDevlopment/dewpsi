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
#include <Dewpsi_Color.h>
#include <Dewpsi_Memory.h>

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

//extern void (PD_APIENTRY * SetViewport)(int, int, int, int);

namespace Dewpsi {
    /**
    *   @addtogroup windows
    *   @{
    */

    /** OpenGL-specific context flags.
    *   @note       Check (https://wiki.libsdl.org/SDL_GLattr?highlight=%28%5CbCategoryEnum%5Cb%29%7C%28CategoryVideo%29)
    *               for a full list of SDL OpenGL attributes.
    */
    enum OpenGLAttributes {
        Empty                   = -1,   ///< Set no attribute
        Depth                   = 0,    ///< Set the bit depth
        DoubleBuffer            = 1,    ///< Enable/disable a double buffer
        RedSize                 = 2,    ///< Bit size of the red channel
        GreenSize               = 3,    ///< Bit size of the green channel
        BlueSize                = 4,    ///< Bit size of the blue channel
        AlphaSize               = 5,    ///< Bit size of the alpha channel
        AccelerationRequired    = 6,    ///< Set to enable/require hardware acceleration
        MajorVersion            = 7,    ///< Set the major version of the context
        MinorVersion            = 8,    ///< Set the minor version of the context
        ContextFlags            = 9,    ///< Set the context flags (https://wiki.libsdl.org/SDL_GLcontextFlag)
        BufferSize              = 10,   ///< Minimum bit size of the frame buffer
        StencilSize             = 11,   ///< Minimum bit size of the stencil buffer
        ShareContext            = 12,   ///< OpenGL context sharing (default is no)

        Count                   = 13    ///< Number of meaningful values in this enumeration (12)
    };

    /** Window/renderer creation flags.
    *   These flags are passed saved to @c %WindowProps. The
    *   structure is passed to SetWindowProps() prior to calling
    *   NewApplication().
    */
    enum WindowFlags : uint32_t {
        WindowFullscreen        = BIT(0),   ///< Generated window is in fullscreen
        WindowFullDesktop       = BIT(1),   ///< Create a fullscreen window at desktop resolution
        WindowOpenGL            = BIT(2),   ///< %Window is usable with an OpenGL context
        WindowVulkan            = BIT(3),   ///< %Window is usable with a Vulkan instance
        WindowShown             = BIT(4),   ///< %Window is visible
        WindowHidden            = BIT(5),   ///< %Window is not visible
        WindowBorderless        = BIT(6),   ///< %Window is without a border
        WindowResizable         = BIT(7),   ///< %Window can be resized
        WindowMinimized         = BIT(8),   ///< %Window is minimized
        WindowMaximized         = BIT(9),   ///< %Window is maximized
        WindowInputGrabbed      = BIT(10),  ///< %Window has grabbed input focus
        WindowInputFocus        = BIT(11),  ///< %Window has input focus
        WindowMouseFocus        = BIT(12),  ///< %Window has mouse focus
        WindowForeign           = BIT(13),  ///< %Window is not created with the native API
        WindowAllowHighDPI      = BIT(14),  ///< %Window should be created in high-DPI mode if supported
        WindowMouseCapture      = BIT(15),  ///< %Window has mouse captured (unreleated to WindowInputGrabbed)
        WindowAlwaysOnTop       = BIT(16),  ///< %Window is always above others
        WindowSkipTaskbar       = BIT(17),  ///< %Window should not be added to the taskbar
        WindowUtility           = BIT(18),  ///< %Window should be treated as a utility window
        WindowTooltip           = BIT(19),  ///< %Window should be treated as a tooltop
        WindowPopupMenu         = BIT(20),  ///< %Window should be treated as a popup window

        RendererSoftware        = BIT(21),  ///< The renderer is a software fallback
        RendererAccelerated     = BIT(22),  ///< The renderer uses hardware acceleration
        RendererVSync           = BIT(23),  ///< Renders are synchronized with the refresh rate
        RendererTargetTexture   = BIT(24)   ///< The renderer supports rendering to texture
    };

    /** A structure that contains the properties
    *   of the window to be created.
    */
    struct WindowProps {
        std::string title;      ///< The title of the window
        PDuint32 x;             ///< The X position of the window
        PDuint32 y;             ///< The Y position of the window
        PDuint32 width;         ///< The width in pixels
        PDuint32 height;        ///< The height in pixels
        PDuint32 flags;         ///< Window/renderer flags
        PDint32  index;         ///< Rendering device index (or -1 for first available device that supports all options)
        PDint32 openglattr[13]; ///< An array of OpenGL attributes (0 indicates no attribute)

        /// Constructs an empty structure
        WindowProps();

        /// Constructs a copy of the other structure
        WindowProps(const WindowProps& src);

        /// Moves @a src into another structure
        WindowProps(WindowProps&& src);

        /// Copy assignment
        WindowProps& operator=(const WindowProps& rhs);

        /// Move assignment
        WindowProps& operator=(WindowProps&& rhs);
    };

    /** A structure with information about a window.
    *   This structure with filled by a call to GetWindowInformation().
    *   @note   A note on the red, green, blue, and alpha fields: those values were constructed with
    *   PD_CREATEBYTE(). To get the size of these, use PD_LONYBBLE(); to get the order, use PD_HINYBBLE().
    *   @code{.cpp}
        WindowModeInfo info;
        // Window* win;
        GetWindowInformation(win, info);
        PDuint8 uiRedSize = PD_LONYBBLE(info.red);
        // uiRedSize = amount of bits for red
    *   @endcode
    */
    struct WindowModeInfo {
        PDint32         swap;       ///< The swap interval (i.e., enable for vsync, 1 for enable, 0 for disable, -1 for adaptive)
        PDint32         accel;      ///< Hardware acceleration enabled (1 for yes, 0 for no)
        PDuint32        width;      ///< The display width
        PDuint32        height;     ///< The display height
        const PDuchar*  vendor;     ///< A string with the underlying rendering context's vendor
        const PDuchar*  renderer;   ///< A string with the name of the underlying rendering context
        const PDuchar*  version;    ///< A string with the version of the underlying rendering context
        const PDuchar*  extensions; ///< A string with the supported extensions for the underlying rendering context
        PDuint8         red;        ///< The bit size of the red component
        PDuint8         green;      ///< The bit size of the green component
        PDuint8         blue;       ///< The bit size of the blue component
        PDuint8         alpha;      ///< The bit size of the alpha component
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
        virtual void OnUpdate() = 0;

        /// Returns true if the window is valid, false otherwise.
        virtual bool IsValid() const = 0;

        /// Obtain the window width.
        virtual PDuint32 GetWidth() const = 0;

        /// Obtain the window height.
        virtual PDuint32 GetHeight() const = 0;

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

        /** Sets the window clear color.
        *   @param color What color to clear the window with
        */
        virtual void SetClearColor(const Color& color) = 0; // TODO: move to a renderer class

        /// Clears the window.
        virtual void Clear() = 0; // TODO: move to a renderer class

        /// Creates an instance of the window.
        static Scope<Window> Create(const WindowProps& props);

    protected:
        static WindowProps* s_pWindowState;
    };

    /** Retrieves information about a window.
    *
    *   @param  win     A pointer to a window
    *   @param  info    A pointer to WindowModeInfo, filled with information on @a win
    *   @return         0 on success and -1 on failure; call GetError() for the error message
    */
    PD_CALL int GetWindowInformation(Window* win, WindowModeInfo* info);

    /** Sets a window OpenGL attribute.
    *   @param  props   A WindowProps structure that is filled with OpenGL attributes
    *   @param  attr    An OpenGL attribute of the enumeration OpenGLAttributes
    *   @param  val     A value to set the given attribute to
    */
    PD_CALL void SetWindowOpenGLAttribute(WindowProps& props, OpenGLAttributes attr, int val);

    /** Retrieves a window OpenGL attribute.
    *   @param  props   A WindowProps structure that was previously filled with information from SetWindowOpenGLAttribute()
    *   @param  pAttr   Filled with the attribute set at @a index position within the array
    *   @param  pVal    Filled with the value of the attribute
    *   @param  index   The index of the attribute to set, must be less than OpenGLAttributes::Count
    */
    PD_CALL void GetWindowOpenGLAttribute(const WindowProps& props, OpenGLAttributes& pAttr, int& pVal, unsigned int index);

    /// @}
}

/// @}

#endif /* DEWPSI_WINDOW_H */
