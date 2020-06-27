#ifndef CORE_H
#define CORE_H

/**
*   @file       core.h
*   @brief      A header for the Dewpsi engine. Contains preprocessor macros.
*   @defgroup   core
*/

/**
*   @macro      PD_CALL
*   @brief      Prefix to the declaration of a non-static non-member function.
*   @ingroup    core
*
*   @addtogroup core {
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

/// Creates a value where only bit @a x is set.
#define BIT(x)                  (1 << x)

/// Function binding.
/// @todo Add description.
#define PD_BIND_EVENT_FN(fn)    std::bind(&fn, this, std::placeholders::_1)

/// @}

namespace Dewpsi {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    
    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif /* CORE_H */

