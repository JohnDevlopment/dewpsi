#ifndef CORE_H
#define CORE_H

/**
*   @file       Dewpsi_Core.h
*   @brief      @doxfb
*   Contains preprocessor macros.
*
*   @defgroup   core Core
*   Provisions of the core functionality of Dewpsi.
*
*   @defgroup platforms Platform-Specific
*   Headers, functions, types, and other declarations specific to each platform
*
*   @addtogroup core
*   @{
*/

#include <Dewpsi_cxx11.h>
#include <Dewpsi_Types.h>

#include <iostream>
#include <new>
#include <utility>
#include <type_traits>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

#include <csignal>
#include <cstdint>

/// Convert an lvalue to an rvalue.
#define PD_MOVE(e)  std::move(e)

/**
*   @def    PD_INLINE
*   @brief  Marks a function as inline.
*   @code
    PD_INLINE int add(int a, int b)
    {
        return a + b;
    }
*   @endcode
*/
#ifndef PD_INLINE
    #if defined(__GNUC__)
        #define PD_INLINE inline
        #ifndef __inline__
            #define __inline__ inline
        #endif
    #elif defined(_MSC_VER) || defined(__BORLANDC__) || \
          defined(__DMC__) || defined(__SC__) || \
          defined(__WATCOMC__) || defined(__LCC__) || \
          defined(__DECC) || defined(__CC_ARM)
        #define PD_INLINE __inline
        #ifndef __inline__
            #define __inline__ __inline
        #endif
    #else
        #define PD_INLINE
    #endif
#endif

/**
*   @def    PD_FORCE_INLINE
*   @brief  Forces a function to be inline.
*   @code
    PD_FORCE_INLINE int add(int a, int b)
    {
        return a + b;
    }
*   @endcode
*/
#ifndef PD_FORCE_INLINE
    #if ((defined(__GNUC__) && (__GNUC__ >= 4))) || defined(__clang__)
        #define PD_FORCE_INLINE __attribute__((always_inline)) static __inline__
    #elif defined(_MSC_VER)
        #define PD_FORCE_INLINE __forceinline
    #else
        #define PD_FORCE_INLINE static PD_INLINE
    #endif
#endif

/**
*   @def    PD_COLD
*   @brief  Mark a function as hot.
*   @code
    int add(int a, int b) PD_COLD;
*   @endcode
*/
#ifndef PD_COLD
    #ifdef __GNUC__
        #define PD_COLD  __attribute__ ((hot))
    #else
        #define PD_COLD
    #endif
#endif

/**
*   @def    PD_HOT
*   @brief  Mark a function as hot.
*   @code
    int add(int a, int b) PD_HOT;
*   @endcode
*/
#ifndef PD_HOT
    #ifdef __GNUC__
        #define PD_HOT  __attribute__ ((hot))
    #else
        #define PD_HOT
    #endif
#endif

/** Get the byte offset of a member of a structure or class.
*   @param  type    A type that is a structure or class
*   @param  member  A member of @a type
*   @return         The byte offset of @a member inside of @a type
*
*   @par Example
*   @snippet core.cpp Offsetof
*/
#define PD_OFFSETOF(type, member)   offsetof(type, member)

/** Get the size of a static array.
*   @param  a   An array with static storage
*   @return     The number of elements in the array
*/
#define PD_ARRAYSIZE(a)             static_cast<int>(sizeof(a) / sizeof(* (a)))

/** Prefix to the declaration of any function or variable.
*   Indicates that a function or variable is @c constexpr.
*/
#define PD_CONSTEXPR constexpr

/**
*   @def        PD_CALL
*   @brief      Prefix to the declaration of a non-static non-member function.
*/
#if (defined(PD_PLATFORM_LINUX) || defined(__linux__) || defined(__gnu_linux__)) \
         && (defined(__GNU__) || defined(__GNUG__))
    #define PD_CALL         extern
#elif defined(PD_PLATFORM_WINDOWS) || defined(_WIN32)
    #ifdef PD_BUILD_DLL
        #define PD_CALL     __declspec(dllexport)
    #else
        #define PD_CALL     __declspec(dllimport)
    #endif
#else
    #define PD_CALL
    #error This platform is not supported!
#endif

/**
*   @def    PD_APIENTRY
*   @brief  As far as I can tell, it's used for function pointers.
*   @todo   Research more about this.
*   @par    Example
*   Define a function pointer in a header
*   @code
    int (PD_APIENTRY * func)(int);
*   @endcode
*/
#define PD_APIENTRY

/**
*   @def        PD_DEBUGBREAK
*   @brief      Sends an interruption signal to the application.
*               This causes GDB and othe debuggers to set a breakpoint.
*/
// debug breaking
#ifdef PD_DEBUG
    #if defined(PD_PLATFORM_LINUX) || defined(__linux)
        #define PD_DEBUGBREAK() std::raise(SIGINT);
    #elif defined(PD_PLATFORM_WINDOWS) || defined(_WIN32)
        #define PD_DEBUGBREAK() __debugbreak();
    #else
        #warning PD_DEBUGBREAK is undefined because the operating system is not recognized.
        #define PD_DEBUGBREAK()
    #endif
#else
    #define PD_DEBUGBREAK(m)
#endif

/// Aborts execution of the program.
#define PD_ABORT()  std::abort()

/**
*   @def        PD_ASSERT
*   @brief      Summarily stops execution of the application if @a x evaluates as @c false.
*   @param  x   The expression to evaluate.
*   @param  ... Forwarded to PD_ERROR().
*   @note       Only compiles if @a PD_ENABLE_ASSERTS is defined.
*
*   @def        PD_CORE_ASSERT
*   @brief      Summarily stops execution of the application if @a x evaluates as @c false.
*   @param  x   The expression to evaluate.
*   @param  ... Forwarded to PD_CORE_ERROR().
*   @note       Only compiles if @a PD_ENABLE_ASSERTS is defined.
*/
#ifdef PD_ENABLE_ASSERTS
    #define PD_ASSERT(x, ...)       if (! (x)) { \
                                        ::Dewpsi::Log::GetClientLogger()->error("Assertion '" #x "' failed"); \
                                        ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__); \
                                        PD_DEBUGBREAK(); \
                                    }
    #define PD_CORE_ASSERT(x, ...)  if (! (x)) { \
                                        ::Dewpsi::Log::GetCoreLogger()->error("Assertion '" #x "' failed"); \
                                        ::Dewpsi::Log::GetCoreLogger()->error(__VA_ARGS__); \
                                        PD_DEBUGBREAK(); \
                                    }
#else
    #define PD_ASSERT(x, ...)
    #define PD_CORE_ASSERT(x, ...)
#endif

/// Creates a value where only bit @a x is set.
#define BIT(x)                  (1 << x)

/// Function binding.
/// @todo Add description.
#define PD_BIND_EVENT_FN(fn)    std::bind(&fn, this, std::placeholders::_1)

/// Retrieves the low word of a value.
#define PD_LOWORD(x)            ((x) & 0xffff)

/// Retrieves the high word of a value.
#define PD_HIWORD(x)            ((x) >> 16)

/** Defines a 32-bit integer from two 16-bit values.
*   @param  lo  The low word
*   @param  hi  The high word.
*   @return     A 32 bit value constructed from two words
*
*   @par Example
*   An example of how PD_CREATEDWORD() can be used:
*   @code{.cpp}
    PDint32 iVal = PD_CREATEDWORD(0x0f, 0x05); // results in 0x010f
    std::cout << std::hex << "0x" << iVal << std::endl;
*   @endcode
*/
#define PD_CREATEDWORD(lo, hi)  (((PDuint16)(hi) << 16) | (PDuint16)(lo))

/// Retrieves the low nybble of an 8-bit value.
#define PD_LONYBBLE(x)          ((PDint8)(x) & 0x0f)

/// Retrieves the high nybble of an 8-bit value.
#define PD_HINYBBLE(x)          ((PDuint8)(x) >> 4)

/** Defines an 8 bit value from two integers.
*   The value is created by combining two integers, one for the low nybble
*   (a nybble is 4 bits), and one for the high nybble.
*   @param  lo  The low nybble
*   @param  hi  The high nybble
*   @return     An 8-bit integer with the two values combined
*
*   @par Example
*   To show how this macro can be used, let's create an 8 bit integer from two
*   smaller values.
*
*   @code{.cpp}
    PDint8 uiVal = PD_CREATEBYTE(0x0f, 0x01); // results in 0x1f
    std::cout << std::hex << "0x" << uiVal << std::endl;
*   @endcode
*/
#define PD_CREATEBYTE(lo, hi)   (((PDuint8)(hi) << 4) | (PDuint8)(lo))

/// Dewpsi Namespace: contains all relevent Dewpsi functions, classes, and other types.
namespace Dewpsi {
    /// @addtogroup core
    /// @{

    /// Returns 1 or 0 depending on whether @a val is non-zero.
    template<typename T>
    inline typename std::enable_if<std::is_arithmetic<T>::value, int>::type
    IsNonzero(T val)
    {
        return (val) ? 1 : 0;
    }

    /** Exchanges the values of @a a and @a b.
    *   The parameters are of course @a a and @a b, whose values are swapped.
    *   The type @c T must be move constructible and move assignable.
    */
    template<typename T>
    void Swap (T& a, T& b)
    noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value)
    {
        T temp(std::move(a));
        a = std::move(b);
        b = std::move(temp);
    }

    /// @}
}

/// @}

#ifdef PD_DONT_DEFINE
/**
*   @defgroup imgui ImGui
*   ImGui types and maybe functions.
*   @ingroup core
*/

/** A draw command.
*   Typically, 1 command is 1 CPU draw call (unless command is a callback).
*   Members @a VtxOffset and @a IdxOffset are relevent when <tt>io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset</tt>
*   is enabled. Those fields allow us to render meshes larger than 64K vertices while keeping 16-bit indices.
*   Pre-1.71 back-ends will typically ignore the @a VtxOffset/IdxOffset fields.
*   The @a ClipRect/TextureId/VtxOffset fields must be contiguous as we memcmp() them together (this is asserted for).
*
*   @ingroup imgui
*/
struct ImDrawCmd
{
    ImVec4          ClipRect;         ///< Clipping rectangle (x1, y1, x2, y2); subtract ImDrawData->DisplayPos to get clipping rectangle in "viewport" coordinates
    ImTextureID     TextureId;        ///< User-provided texture ID, set by user in ImfontAtlas::SetTexID() for fonts or passed to Image*() functions; ignore if never using images or multiple fonts atlas
    unsigned int    VtxOffset;        ///< Starting offset in vertex buffer; ImGuiBackendFlags_RendererHasVtxOffset: always 0, otherwise may be >0 to support large meshes
    unsigned int    IdxOffset;        ///< Starting offset in index buffer; always equal to sum of @a ElemCount drawn so far
    unsigned int    ElemCount;        ///< Number of indices (multiple of 3) to be rendered as triangles; vertices are stored in the callee @c ImDrawList's @a vtx_buffer array, indices in @a idx_buffer[]
    ImDrawCallback  UserCallback;     ///< If not null, call the function instead of rendering the vertices; clip_rect and texture_id will be set normally
    void*           UserCallbackData; ///< Accessed by the callback function

    /// Initializes @c ImDrawCmd's data to zero.
    ImDrawCmd() { memset(this, 0, sizeof(*this)); }
};

/** Draw command list.
*   This is the low-level list of polygons that ImGui functions are filling. At the end of the frame,
*   all command lists are passed to your ImGuiIO::RenderDrawListFn function for rendering.
*   Each Dear ImGui window contains its own ImDrawList. You can use ImGui::GetWindowDrawList() to
*   access the current window draw list and draw custom primitives.
*   You can interleave normal ImGui calls and add primitives to the current draw list.
*
*   @par Pixel
*   All positions are generally in pixel coordinates (top-left corner at (0, 0), bottom-right corner
*   at io.DisplaySize). However, transformation matrices can be applied to the data, and they can
*   be any type of transformation. A point of advice: if such a transformation is applied, it should be
*   applied to ClipRect as well.
*
*   @note
*   Primitives are always added to the list not culled (culling is done at a higher level by ImGui functions).
*   Culling of drawn objects should be considered if this API is used a lot.
*
*   @ingroup imgui
*/
struct ImDrawList {
    ImVector<ImDrawCmd>     CmdBuffer; ///< Draw commands
    ImVector<ImDrawIdx>     IdxBuffer; ///< Index buffer; each command consume ImDrawCmd::ElemCount of those
    ImVector<ImDrawVert>    VtxBuffer; ///< Vertex buffer
    ImDrawListFlags         Flags;     ///< Flags, you may poke into these to adjust anti-aliasing settings per-primitive
};

/** All draw data to render a Dear ImGui frame.
*   @ingroup imgui
*/
struct ImDrawData {
    bool         Valid;             ///< Only valid after Render() is called and before the next NewFrame() is called
    ImDrawList** CmdLists;          ///< Array of ImDrawList* to render, the ImDrawList are owned by ImGuiContext and only pointed to from here
    int          CmdListsCount;     ///< Number of ImDrawList* to render
    int          TotalIdxCount;     ///< For convenience, sum of all ImDrawList's IdxBuffer.Size
    int          TotalVtxCount;     ///< For convenience, sum of all ImDrawList's VtxBuffer.Size
    ImVec2       DisplayPos;        ///< Upper-left position of the viewport to render (== upper-left of the orthogonal projection matrix to use)
    ImVec2       DisplaySize;       ///< Size of the viewport to render (== io.DisplaySize for the main viewport) (DisplayPos + DisplaySize == lower-right of the orthogonal projection matrix to use)
    ImVec2       FramebufferScale;  ///< Amount of pixels for each unit of DisplaySize. Based on io.DisplayFramebufferScale. Generally (1,1) on normal display, (2,2) on OSX with Retina display

    /// Default constructor
    ImDrawData()
    {
        Clear();
    }

    /// Destructor
    ~ImDrawData()
    {
        Clear();
    }

    /// Clear and initialize data.
    void Clear()
    {
        Valid = false;
        CmdLists = nullptr;
        CmdListsCount = TotalVtxCount = TotalIdxCount = 0;
        DisplayPos = DisplaySize = FramebufferScale = ImVec2(0.f, 0.f);
    }

    /** Helper to convert all buffers from indexed to non-indexed, in case you cannot render indexed.
    *   @note
    *   This is a slow and most likely a waste of resources. Always prefer indexed rendering!
    */
    IMGUI_API void DeIndexAllBuffers();

    /** Helper to scale the ClipRect field of each ImDrawCmd.
    *   Use if the final output buffer is at a different scale than
    *   Dear ImGui expects, or if there is a difference between
    *   your window resolution and framebuffer resolution.
    */
    IMGUI_API void ScaleClipRects(const ImVec& scale);
};
#endif /* PD_DONT_INCLUDE */

#endif /* CORE_H */
