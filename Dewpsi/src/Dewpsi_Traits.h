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

    class __Undefined {};

    /** Is void.
    *   Trait class that identifies whether @c T is void.
    *   @par Source
    *   http://www.cplusplus.com/reference/type_traits/is_void/
    */
    template<typename T>
    struct IsVoid : public ::std::is_void<T> {};

    /// A metafunction that always returns void.
    /// Used to detect valid types. Copied from the stl.
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

    /** Alias of std::integral_constant (see source)
    *   @par Source
    *   http://www.cplusplus.com/reference/type_traits/integral_constant/
    */
    template<typename Type, Type Constant>
    using IntegralConstant = ::std::integral_constant<Type, Constant>;

    /// Instantiation of IntegralConstant with a constant of type @c bool, equal to @c true
    typedef IntegralConstant<bool, true> TrueType;

    /// Instantiation of IntegralConstant with a constant of type @c bool, equal to @c false
    typedef IntegralConstant<bool, false> FalseType;

    // Type properties

    /** Is signed integral type.
    *   Trait class that identifies whether @c T is a signed type.
    *   It inherits from either @c true_type or @c false_type depending on
    *   whether @c T is a signed type or not.
    */
    template<typename T>
    struct IsSigned : public ::std::is_signed<T> {};

    /** Is same type.
    *   Trait class that identifies whether @c T is the same type as @a U, including
    *   whether they have the same const or volatile qualifications, if any.
    *   @par Source
    *   http://www.cplusplus.com/reference/type_traits/is_same/
    */
    template<typename T, typename U>
    struct IsSame : public ::std::is_same<T, U> {};

    /** Is empty.
    *   Trait class that identifies whether @c T is an empty class or struct (but not union).
    *   @par Source
    *   http://www.cplusplus.com/reference/type_traits/is_empty/
    */
    template<typename T>
    struct IsEmpty : public ::std::is_empty<T> {};

    // Type Modifiers

    /** Make unsigned.
    *   Obtains the unsigned type corresponding to @c T.
    *   @par Source
    *   http://www.cplusplus.com/reference/type_traits/make_unsigned/
    */
    template<typename T>
    struct MakeUnsigned : public ::std::make_unsigned<T> {};

    // Misc

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

    /** Use @c Op if it exists, otherwise resort to @c Default.
    *   @tparam Default,Op Types
    *   @tparam Args       Arguments to @c Op
    *
    *   @par Members
    *   Name    | Comment
    *   ------- | -------
    *   type    | Type determined by the condition
    *   value_t | @a TrueType if @c Op exists or @a FalseType if it doesn't
    */
    template<typename Default, typename AlwaysVoid,
        template<typename...> class Op, typename... Args>
    struct __Detector {
        using type = Default;
        using value_t = FalseType;
    }; // Specialization for false

    // Specialization for true
    template<typename Default, template<typename...> class Op, typename... Args>
    struct __Detector<Default, __void_t<Op<Args...>>, Op, Args...> {
        using type = Op<Args...>;
        using value_t = TrueType;
    };

    template<typename Default, template<typename...> class Op, typename... Args>
    using __DetectOr = __Detector<Default, void, Op, Args...>;

    template<typename Default, template<typename...> class Op, typename... Args>
    using __DetectedOrType = typename __DetectOr<Default, Op, Args...>::type;

    /// @}
}

#endif /* DEWPSI_TRAITS_H */
