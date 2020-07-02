#ifndef CORE_H
#define CORE_H

#include <type_traits>

/**
*   @file       core.h
*   @brief      A header for the Dewpsi engine. Contains preprocessor macros.
*
*   @defgroup   core Core
*   Provisions of the core functionality of Dewpsi.
*
*   @{
*/

/**
*   @def        PD_CALL
*   @brief      Prefix to the declaration of a non-static non-member function.
*/
#ifdef PD_PLATFORM_LINUX
    #define PD_CALL         extern
#elif PD_PLATFORM_WINDOWS
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
*   @def        PD_DEBUGBREAK
*   @brief      Sends an interruption signal to the application.
*               This causes GDB and othe debuggers to set a breakpoint.
*/
// debug breaking
#ifdef PD_DEBUG
    #ifdef PD_PLATFORM_LINUX
        #define PD_DEBUGBREAK() std::raise(3);
    #elif PD_PLATFORM_WINDOWS
        #define PD_DEBUGBREAK() __debugbreak();
    #endif
#else
    #define PD_DEBUGBREAK(m)
#endif

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
    #define PD_ASSERT(x, ...)       if (! (x)) { PD_ERROR("Assertion failed: {0}", __VA_ARGS__); PD_DEBUGBREAK(); }
    #define PD_CORE_ASSERT(x, ...)  if (! (x)) { PD_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); PD_DEBUGBREAK(); }
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

/// Creates a 32 bit value from two words.
#define PD_CREATEWORD(lo, hi)   (((uint16_t) hi << 16) | (uint16_t)(lo))

/// Namespace Dewpsi
namespace Dewpsi {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    
    /// Create a scope of type @c T and pass @a args to its constructor.
    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    /// Create a "reference" of type @c T and pass @a args to its constructor.
    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    /// Returns 1 or 0 depending on whether @a val is non-zero.
    template<typename T>
    inline typename std::enable_if<std::is_arithmetic<T>::value, int>::type
    IsNonzero(T val)
    {
        return (val) ? 1 : 0;
    }
}

/// @}

#endif /* CORE_H */
