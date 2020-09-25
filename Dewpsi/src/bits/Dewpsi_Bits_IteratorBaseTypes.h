#ifndef DEWPSI_BITS_ITERATORBASETYPES_H
#define DEWPSI_BITS_ITERATORBASETYPES_H

/** @file     Dewpsi_Bits_IteratorBaseTypes.h
*   @ref      core_iterators
*   @brief    @doxfb
*/

#include <Dewpsi_Traits.h>

namespace Dewpsi {
    /// Import of std::ptrdiff_t
    using ::std::ptrdiff_t;

    /** @defgroup core_iterators_tags Iterator Tags
    *   @ingroup core_iterators
    *   @{
    */

    /// Output iterator
    struct OutputIteratorTag {};

    /// Input iterator
    struct InputIteratorTag {};

    /// Forward iterator
    struct ForwardIteratorTag : public InputIteratorTag {};

    /// A bi-directional iterator that supports a superset of forward iterator operations
    struct BidirectionalIteratorTag : public ForwardIteratorTag {};

    /// A random-access iterator that supports a superset of bi-directional iterator operations
    struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};

    /// @}

    /// Basic iterator. All it does is set typedefs for the client.
    /// @ingroup core_iterators
    template<typename _Category, typename _Value, typename _Distance = ptrdiff_t,
             typename _Pointer = _Value*, typename _Reference = _Value&>
    struct Iterator {
        /// One of the @link core_iterators_tags iterator tag types@endlink
        typedef _Category IteratorCategory;
        /// The type "pointed to" by the iterator
        typedef _Value ValueType;
        /// Represents the distance between iterators
        typedef _Distance DifferenceType;
        /// Pointer to the value
        typedef _Pointer Pointer;
        /// Reference to the value
        typedef _Reference Reference;
    };
}

#endif /* DEWPSI_BITS_ITERATORBASETYPES_H */
