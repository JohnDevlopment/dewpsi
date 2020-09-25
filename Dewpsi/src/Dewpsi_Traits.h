#ifndef DEWPSI_TRAITS_H
#define DEWPSI_TRAITS_H

/** @file Dewpsi_Traits.h
*   @ref core_traits
*   @defgroup core_traits Type Traits
*   @ingroup core
*   Trait classes specifically tailored to %Dewpsi types.
*/

#include <type_traits>

namespace Dewpsi {
    /// @addtogroup core_traits
    /// @{

    /// A metafunction that always returns void. Used to detect valid types. Copied from the stl.
    template<typename... Args>
    using __void_t = void;

#if __cplusplus >= 201703L || !defined(__STRICT_ANSI__)
    #ifndef __cpp_lib_void_t
        #define __cpp_lib_void_t 201411
    #endif
    /// A metafunction that always returns void. Used to detect valid types. Copied from the stl.
    template<typename... Args>
    using void_t = void;
#endif

    /// @}
}

#endif /* DEWPSI_TRAITS_H */
