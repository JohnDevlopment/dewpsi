#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <csignal>
#include <cstdint>
#include <Dewpsi_Log.h>
#include <Dewpsi_Types.h>


/**
*   @file       Dewpsi_Core.h
*   @brief      A header for the Dewpsi engine. Contains preprocessor macros.
*
*   @defgroup   core Core
*   Provisions of the core functionality of Dewpsi.
*
*   @{
*/

/**
*   @def        PD_CONSTEXPR
*   @brief      Prefix to the declaration of any function even before PD_CALL.
*
*   Indicates that the function returns a constant expression.
*/
#if defined(__cplusplus) && __cplusplus >= 201103L
    #define PD_CONSTEXPR    constexpr
#else
    #define PD_CONSTEXPR
#endif

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
    #define PD_ASSERT(x, ...)       if (! (x)) { ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__); PD_DEBUGBREAK(); }
    #define PD_CORE_ASSERT(x, ...)  if (! (x)) { ::Dewpsi::Log::GetCoreLogger()->error(__VA_ARGS__); PD_DEBUGBREAK(); }
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
#define PD_CREATEWORD(lo, hi)   (((uint16_t)(hi) << 16) | (uint16_t)(lo))

/// Dewpsi Namespace: contains all relevent Dewpsi functions, classes, and other types.
namespace Dewpsi {
    /// @addtogroup core
    /// @{
    
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
    
    /// @}
}

/// @}

#endif /* CORE_H */
