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

    /** Trait class that identifies whether @c T is a signed type.
    *   It inherits from either @c true_type or @c false_type depending on
    *   whether @c T is a signed type or not.
    */
    template<typename T>
    struct IsSigned : public ::std::is_signed<T> {};

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

    /** Obtains @c T or @c F depending on whether @c Cond is either true or false.
    *   If @c Cond evaluates to @c true, then @doxtype{Conditional::type} is defined
    *   as an alias of @c T.\n
    *   If @c Cond evaluates to @c false, then @doxtype{Conditional::type} is defined as an alias
    *   of @c F.
    *   @code{.cpp}
    *       Dewpsi::Conditional<true, int, float>::type isInt; // type = int
    *       Dewpsi::Conditional<false, int, float>::type isFloat; // type = float
    *   @endcode
    *   @tparam Cond A compile-time expression of type @c bool
    *   @tparam T,F  Types
    */
    template<bool Cond, typename T, typename F> using Conditional = ::std::conditional<Cond, T, F>;

#if 0
    template<bool Cond, typename T, typename F>
    struct Conditional {
        typedef T type;
    };

    // Partial specialization for false
    template<typename T, typename F>
    struct Conditional<false, T, F> {
        typedef F type;
    };

    template<typename...>
    struct Or;

    /// Boolean OR meta class.
    template<>
    struct Or<> : public false_type {};

    template<typename B1>
    struct Or<B1> : public B1 {};

    template<typename B1, typename B2>
    struct Or<B1, B2> : public Conditional<B1::value, B1, B2>::type {};

    template<typename B1, typename B2, typename B3, typename... Bn>
    struct Or<B1, B2, B3, Bn...> : public Conditional<B1::value, B1, Or<B2, B3, Bn...>>::type {};
#endif

    /// @}
}

#endif /* DEWPSI_TRAITS_H */
