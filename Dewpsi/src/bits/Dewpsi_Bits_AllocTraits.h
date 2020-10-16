#ifndef DEWPSI_BITS_ALLOCTRAITS_H
#define DEWPSI_BITS_ALLOCTRAITS_H

/** @file Dewpsi_Bits_AllocTraits.h
*   @brief An internal header. Do not attempt to use it directly. @doxheader{Dewpsi_Memory.h}
*/

#include <Dewpsi_Types.h>
#include <Dewpsi_Traits.h>

#include "Dewpsi_Bits_PtrTraits.h"

namespace Dewpsi {
    struct AllocatorTraitsBase {
    protected:
        template<typename _Tp>
        using __pointer = typename _Tp::__Pointer;

        template<typename _Tp>
        using __const_pointer = typename _Tp::__ConstPointer;

        template<typename _Tp>
        using __void_pointer = typename _Tp::__VoidPointer;

        template<typename _Tp>
        using __const_void_pointer = typename _Tp::__ConstVoidPointer;

        template<typename _Tp>
        using __pocca = typename _Tp::__PropagateOnContainerCopyAssignment;

        template<typename _Tp>
        using __pocma = typename _Tp::__PropagateOnContainerMoveAssignment;

        template<typename _Tp>
        using __pocs = typename _Tp::__PropagateOnContainerSwap;

        template<typename _Tp>
        using __equal = typename _Tp::__IsAlwaysEqual;
    };

    /** %Allocator traits class.
    *   Provides a standardized way to access the various properties of allocators.
    *   @ingroup core_traits
    */
    template<typename Alloc>
    struct AllocatorTraits : AllocatorTraitsBase {
        /// Allocator type.
        typedef Alloc __AllocatorType;

        /// Allocated type.
        typedef typename Alloc::__ValueType __ValueType;

        /// Pointer to allocated type.
        using __Pointer = __DetectedOrType<__ValueType*, __pointer, Alloc>;

    private:
        // Select Func<Alloc> or PointerTraits<__Pointer>::__Rebind<_Tp>
        template<template<typename> class Func, typename _Tp, typename = void>
        struct __Ptr {
            using type = typename PointerTraits<__Pointer>::template __Rebind<_Tp>;
        };

        template<template<typename> class Func, typename _Tp>
        struct __Ptr<Func, _Tp, __void_t<Func<Alloc>>> {
            using type = Func<Alloc>;
        };

        // Select A2::__DifferenceType or PointerTraits<Ptr>::__DifferenceType
        template<typename A2, typename Ptr, typename = void>
        struct __Diff {
            using type = typename PointerTraits<Ptr>::__DifferenceType;
        };

        template<typename A2, typename Ptr>
        struct __Diff<A2, Ptr, __void_t<typename A2::__DifferenceType>> {
            using type = typename A2::__DifferenceType;
        };

        // Select A2::__SizeType or MakeUnsigned<Diff>::type
        template<typename A2, typename Diff, typename = void>
        struct __Size : MakeUnsigned<Diff> {};

        template<typename A2, typename Diff>
        struct __Size<A2, Diff, __void_t<typename A2::__SizeType>> {
            typedef typename A2::__SizeType type;
        };

    public:
        /// Const pointer to allocated type
        using __ConstPointer = typename __Ptr<__const_pointer, const __ValueType>::type;

        /// Pointer to void
        using __VoidPointer = typename __Ptr<__void_pointer, void>::type;

        /// Const pointer to void
        using __ConstVoidPointer = typename __Ptr<__const_void_pointer, const void>::type;

        /// Type that represents the difference between two pointers.
        using __DifferenceType = typename __Diff<Alloc, __Pointer>::type;

        /// Type used to represent sizes.
        using __SizeType = typename __Size<Alloc, __DifferenceType>::type;

        /** How the allocator is propogated on copy assignment.
        *   @c Alloc::__PropagateOnContainerCopyAssignment if that type exists or
        *   @c FalseType otherwise.
        */
        using __PropagateOnContainerCopyAssignment = __DetectedOrType<FalseType, __pocca, Alloc>;

        /** How the allocator is propogated on move assignment.
        *   @c Alloc::__PropagateOnContainerMoveAssignment if that type exists or
        *   @c FalseType otherwise.
        */
        using __PropagateOnContainerMoveAssignment = __DetectedOrType<FalseType, __pocma, Alloc>;

        /** How the allocator is propogated on swap.
        *   @c Alloc::__PropagateOnContainerSwap if that type exists or @c FalseType otherwise.
        */
        using __PropagateOnContainerSwap = __DetectedOrType<FalseType, __pocs, Alloc>;

        /** Whether all instances of the allocator type are equal.
        *   @c Alloc::__IsAlwaysEqual if that type exists or @c FalseType otherwise.
        */
        using __IsAlwaysEqual = __DetectedOrType<FalseType, __equal, Alloc>;
    };
}

#endif /* DEWPSI_BITS_ALLOCTRAITS_H */
