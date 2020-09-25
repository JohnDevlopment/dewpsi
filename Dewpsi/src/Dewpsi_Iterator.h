#ifndef DEWPSI_ITERATOR_H
#define DEWPSI_ITERATOR_H

/** @file Dewpsi_Iterator.h
*   @ref core_iterators
*   @brief Iterator definition.
*   @defgroup core_iterators Iterators
*   @ingroup core
*   Contains reverse iterators and base iterator types.
*/

#include <Dewpsi_Traits.h>
#include <Dewpsi_Util.h>

#include "bits/Dewpsi_Bits_IteratorBaseTypes.h"
#include "bits/Dewpsi_Bits_IteratorTraits.h"

namespace Dewpsi {
    /** A reverse iterator.
    *   This is an adaptor for the underlying iterator being used. It is used like any other
    *   iterator, except it iterates through data in reverse direction. The fundamental
    *   relationship between a reverse iterator and its corresponding iterator @c i is
    *   represented as such:
    *   @code
    *       &*(reverse(i)) = &*(i - 1)
    *   @endcode
    *   @ingroup core_iterators
    *
    *   @par Operators
    *   @doxtype{ReverseIterator} supports postfix and prefix increment/decrement operators, as well
    *   as binary addition and subtraction operators. Like other iterators, this one can also be
    *   dereferenced with the astkerik (*) and arrow (->) operators.
    */
    template<typename _Iterator>
    class ReverseIterator : public Iterator<typename IteratorTraits<_Iterator>::IteratorCategory,
                                            typename IteratorTraits<_Iterator>::ValueType,
                                            typename IteratorTraits<_Iterator>::DifferenceType,
                                            typename IteratorTraits<_Iterator>::Pointer,
                                            typename IteratorTraits<_Iterator>::Reference>
    {
    protected:
        _Iterator m_Current;

        typedef IteratorTraits<_Iterator> TraitsType;
    public:
        typedef _Iterator IteratorType;
        typedef typename TraitsType::DifferenceType DifferenceType;
        typedef typename TraitsType::Pointer Pointer;
        typedef typename TraitsType::Reference Reference;

        constexpr ReverseIterator() noexcept = default;

        /// Construct an iterator that moves backwards.
        constexpr ReverseIterator(IteratorType _x) noexcept : m_Current(_x) {}

        /// Copy constructor.
        explicit constexpr ReverseIterator(const ReverseIterator& _src) noexcept
            : m_Current(_src.m_Current) {}

        /// Allows for a reverse iterator to be constructed from a convertible different type.
        template<typename _Iter>
        constexpr ReverseIterator(const ReverseIterator<_Iter>& _src)
            : m_Current(_src.Base()) {}

        /// Returns the value of the iterator.
        constexpr Reference operator*() const
        {
            _Iterator temp = m_Current;
            return *--temp;
        }

        /// Access a member of the value pointed to by the iterator.
        Pointer operator->() {return PD_ADDRESSOF(operator*());}

        /// Access a member of the value pointed to by the iterator.
        const Pointer operator->() const {return PD_ADDRESSOF(operator*());}

        /** Increment operator.
        *   Prefix and postfix are supported. With @c iter representing this class:
        *   @code
        *       ++iter; // prefix increment
        *       iter++; // postfix increment
        *   @endcode
        */
        ReverseIterator operator++()
        {
            --m_Current;
            return *this;
        }
        ReverseIterator operator++(int)
        {
            ReverseIterator temp = *this;
            --m_Current;
            return temp;
        }

        /** Decrement operator.
        *   Prefix and postfix are supported. With @c iter representing this class:
        *   @code
        *       --iter; // prefix decrement
        *       iter--; // postfix decrement
        *   @endcode
        */
        ReverseIterator operator--()
        {
            ++m_Current;
            return *this;
        }
        ReverseIterator operator--(int)
        {
            ReverseIterator temp = *this;
            ++m_Current;
            return temp;
        }

        /// Addition operator.
        ReverseIterator operator+(DifferenceType rhs) const
        {
            return ReverseIterator(m_Current - rhs);
        }

        /// Subtraction operator.
        ReverseIterator operator-(DifferenceType rhs) const
        {
            return ReverseIterator(m_Current + rhs);
        }

        /// Equality operator.
        bool operator==(ReverseIterator rhs) const noexcept {return m_Current == rhs.m_Current;}

        /// Inequality operator.
        bool operator!=(ReverseIterator rhs) const noexcept {return m_Current != rhs.m_Current;}

        /// Returns the base of the iterator.
        IteratorType Base() const {return m_Current;}
    };
}


#endif /* DEWPSI_ITERATOR_H */
