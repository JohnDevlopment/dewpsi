#ifndef DEWPSI_BITS_ATTRIBUTES_H
#define DEWPSI_BITS_ATTRIBUTES_H

#include <Dewpsi_Types.h>

/** @file Dewpsi_Bits_Attributes.h
*   @ref func_attr
*   @defgroup func_attr Function Attributes
*   @ingroup core
*   @{
*/

#ifndef PD_ATTR
    #if defined(__GNUC__)
        /** Define a list of attributes for a function or variable.
        *   @param ... One or more attributes to apply
        *	@note For a list of attributes, see your compiler's documentation.
        */
        #define PD_ATTR(...) __attribute__((__VA_ARGS__))
    #elif defined(_MSC_VER) || defined(__BORLANDC__)
        #define PD_ATTR(...) __declspec(__VA_ARGS__)
    #else
        #error "This compiler is not supported"
    #endif
#endif

/**
*   @def    PD_COLD
*   @brief  Mark a function as cold.
*   @code
    int add(int a, int b) PD_ATTR(PD_COLD);
*   @endcode
*/
#ifndef PD_COLD
    #define PD_COLD cold
#endif

/**
*   @def    PD_HOT
*   @brief  Mark a function as hot.
*   @code
    int add(int a, int b) PD_ATTR(PD_HOT);
*   @endcode
*/
#ifndef PD_HOT
    #define PD_HOT hot
#endif

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
*   @def        PD_CALL
*   @brief      Prefix to the declaration of a non-static non-member function.
*	It expands to different things depending on the platform and how the library
*	is being built. (Currently it does not mean anything.)
*/
#if (defined(PD_PLATFORM_LINUX) || defined(__linux__) || defined(__gnu_linux__)) \
         && (defined(__GNU__) || defined(__GNUG__))
    #define PD_CALL extern
#elif defined(PD_PLATFORM_WINDOWS) || defined(_WIN32)
    #if defined(PD_EXPORT_DLL)
        #define PD_CALL     __declspec(dllexport) extern
    #elif defined(PD_IMPORT_DLL)
        #define PD_CALL     __declspec(dllimport) extern
    #else
        #define PD_CALL
        #error "Define either PD_EXPORT_DLL or PD_IMPORT_DLL"
    #endif
#else
    #define PD_CALL
    #error "This platform is not supported!"
#endif

/// @}

#endif /* DEWPSI_BITS_ATTRIBUTES_H */
