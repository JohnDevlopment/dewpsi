#ifndef DEWPSI_BITS_PTRTRAITS_H
#define DEWPSI_BITS_PTRTRAITS_H

/** @file Dewpsi_Bits_PtrTraits.h
*   @brief An internal header. Do not attempt to use it directly. @doxheader{Dewpsi_Memory.h}
*/

#include <Dewpsi_Traits.h>

namespace Dewpsi {
    template<typename T>
    using __MakeNotVoid = typename Conditional<IsVoid<T>::value, __Undefined, T>::type;

    // Given Template<T, ...> and U return Template<U, ...>, otherwise invalid.
    template<typename T, typename U>
    struct __ReplaceFirstArg {};

    template<template<typename, typename...> class TClass, typename U,
        typename T, typename... Types>
    struct __ReplaceFirstArg<TClass<T, Types...>, U> {
        using type = TClass<U, Types...>;
    };

    // Given Template<T, ...> return T, otherwise invalid
    template<typename T>
    struct __GetFirstArg {
        using type = __Undefined;
    };

    template<template<typename, typename...> class TClass, typename T, typename... Types>
    struct __GetFirstArg<TClass<T, Types...>> {
        using type = T;
    };

    template<typename T>
    using __GetFirstArg_t = typename __GetFirstArg<T>::type;

    /// Uniform interface to all pointer-like types.
    /// @ingroup core_traits
    template<typename Ptr>
    struct PointerTraits {
    private:
        template<typename _Tp>
        using __element_type = typename _Tp::__ElementType;

        template<typename _Tp>
        using __difference_type = typename _Tp::__DifferenceType;

        // Rebind _Tp to _Up for subclass.
        template<typename _Tp, typename _Up, typename = void>
        struct __rebind : __ReplaceFirstArg<_Tp, _Up> {};

        template<typename _Tp, typename _Up>
        struct __rebind<_Tp, _Up, __void_t<typename _Tp::template __Rebind<_Up>>>
        {
            using type = typename _Tp::template __Rebind<_Up>;
        };

    public:
        /// Pointer type.
        using __Pointer = Ptr;

        /// Type pointed to.
        using __ElementType = __DetectedOrType<__GetFirstArg_t<Ptr>, __element_type, Ptr>;

        /// Type used to represent differences between two pointers.
        using __DifferenceType = __DetectedOrType<PDptrdiff, __difference_type, Ptr>;

        /** Rebind pointer to a different type.
        *   For an example on how to use this, let's remap the pointer type to point to @c block.
        *   @code{.cpp}
        *       using block_ptr_t = Dewpsi::PointerTraits<Ptr>::__Rebind<block>;
        *       block_ptr_t m_Ptr;
        *   @endcode
        */
        template<typename _Up>
        using __Rebind = typename __rebind<Ptr, _Up>::type;

        static Ptr PointerTo(__MakeNotVoid<__ElementType>& e)
        {
            return Ptr::PointerTo(e);
        }

        static_assert(! IsSame<__ElementType, __Undefined>::value,
            "pointer type defines element type or is like SomePointer<T, ...>");
    };

    /// Partial specialization for built-in pointers.
    /// @ingroup core_traits
    template<typename T>
    struct PointerTraits<T*> {
        /// Pointer type.
        typedef T* __Pointer;

        // Type pointed to.
        typedef T __ElementType;

        /// Type used to represent differences between two pointers.
        typedef PDptrdiff __DifferenceType;

        /** Rebind pointer to a different type.
        *   For an example on how to use this, let's remap the pointer type to point to @c block.
        *   @code{.cpp}
        *       using block_ptr_t = Dewpsi::PointerTraits<Ptr>::__Rebind<block>;
        *       block_ptr_t m_Ptr;
        *   @endcode
        */
        template<typename _Up>
        using __Rebind = _Up*;

        static __Pointer PointerTo(__MakeNotVoid<__ElementType>& e)
        {
            return AddressOf(e);
        }

        static_assert(! IsSame<__ElementType, __Undefined>::value,
            "pointer type defines element type or is like SomePointer<T, ...>");
    };
}

#endif /* DEWPSI_BITS_PTRTRAITS_H */
