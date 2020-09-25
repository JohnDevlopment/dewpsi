#ifndef DEWPSI_BITS_ITERATORTRAITS_H
#define DEWPSI_BITS_ITERATORTRAITS_H

/** @file     Dewpsi_Bits_IteratorTraits.h
*   @ref      core_iterators_traits
*   @brief    @doxfb
*   @defgroup core_iterators_traits Iterator Traits
*   @ingroup  core_iterators
*/

#include <Dewpsi_Traits.h>

namespace Dewpsi {
    /* Helper class for IteratorTraits. This is the generic version. */
    template<typename _Iterator, typename = __void_t<>>
    struct __IteratorTraits {};

    /* This version is used for iterator classes. The criteria is that the class defines
     IteratorCateogry, ValueType, DifferenceType, Pointer, and Reference as types. */
    template<typename _Iterator>
    struct __IteratorTraits<_Iterator, __void_t<typename _Iterator::IteratorCateogry,
                                                typename _Iterator::ValueType,
                                                typename _Iterator::DifferenceType,
                                                typename _Iterator::Pointer,
                                                typename _Iterator::Reference>> {
        typedef typename _Iterator::IteratorCateogry IteratorCateogry;
        typedef typename _Iterator::ValueType ValueType;
        typedef typename _Iterator::DifferenceType DifferenceType;
        typedef typename _Iterator::Pointer Pointer;
        typedef typename _Iterator::Reference Reference;
    };

    /** Iterator trait class that defines nested typedefs.
    *   The generic version simply forwards from the iterator argument.
    *   But there are partial specializations for pointers and const pointers.
    */
    template<typename _Iterator>
    struct IteratorTraits : public __IteratorTraits<_Iterator> {};

    template<typename _Type>
    struct IteratorTraits<_Type*> {
        typedef RandomAccessIteratorTag IteratorCategory;
        typedef _Type                   ValueType;
        typedef ptrdiff_t               DifferenceType;
        typedef _Type*                  Pointer;
        typedef _Type&                  Reference;
    };

    template<typename _Type>
    struct IteratorTraits<const _Type*> {
        typedef RandomAccessIteratorTag IteratorCategory;
        typedef _Type                   ValueType;
        typedef ptrdiff_t               DifferenceType;
        typedef const _Type*            Pointer;
        typedef const _Type&            Reference;
    };
}

#endif /* DEWPSI_BITS_ITERATORTRAITS_H */
