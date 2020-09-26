#ifndef DEWPSI_ARRAY_H
#define DEWPSI_ARRAY_H

/// @file Dewpsi_Array.h
/// @ref core

#include <Dewpsi_Iterator.h>
#include <Dewpsi_Math.h>
#include <cassert>
#include <initializer_list>

namespace Dewpsi {
    /** An array with static storage.
    *   @tparam Type Type of the array
    *   @tparam N    Number of elements
    *   @ingroup     core
    */
    template<typename Type, PDsizei N>
    struct Array {
        typedef Type __ValueType;             ///< Type of each element
        typedef Type* __Pointer;              ///< Pointer of @c value_type
        typedef const Type* __ConstPointer;   ///< Const pointer of @c value_type
        typedef Type& __Reference;            ///< Reference of @c value_type
        typedef const Type& __ConstReference; ///< Const reference of @c value_type
        typedef Type* __Iterator;             ///< Iterator of @c value_type
        typedef const Type* __ConstIterator;  ///< Const iterator of @c value_type

        /// Reverse iterator
        typedef Dewpsi::ReverseIterator<__Iterator> __ReverseIterator;

        /// Const reverse iterator
        typedef Dewpsi::ReverseIterator<__ConstIterator> __ConstReverseIterator;

        /** Constructor.
        *   There are multiple constructors with different parameters:
        *   -# Default constructor
        *   -# Copies values from one array to another, respecting bounds
        *   -# Moves values from one array to another, respecting bounds
        *   -# Initialize array from a list of values
        *
        *   @par Examples
        *   @code{.cpp}
                Array<int, 5> first;
                Array<int, 5> second = first;
                Array<int, 5> third = std::move(second);
                Array<int, 5> fourth = {1, 2, 3, 4, 5};
        *   @endcode
        */
        Array() = default;
        Array(const Array& src)
        {
            PDsizei size = Dewpsi::min(N, src.Size());
            for (PDsizei i = 0; i < size; ++i)
                values[i] = src.values[i];
        }
        Array(Array&& src)
        {
            PDsizei size = Dewpsi::min(N, src.Size());
            for (PDsizei i = 0; i < size; ++i)
                values[i] = src.values[i];
        }

        /// Initialize %Array with a list of values.
        Array(const std::initializer_list<Type>& il)
        {
            PDsizei x = 0;
            for (auto e : il)
            {
                if (x == N) break;
                values[x++] = e;
            }
        }

        ~Array() = default;

        /// Copies one %Array to another.
        Array& operator=(const Array& src) = delete;

        void SetData(const std::initializer_list<Type>& il)
        {
            PDsizei x = 0;
            for (auto e : il)
            {
                if (x == N) break;
                values[x++] = e;
            }
        }

        /// Obtain a reference to one of the elements (does not check for bounds).
        __Reference operator[](PDsizei i)
        {
            assert(i < size);
            return values[i];
        }

        /// Obtain an immutable reference to one of the elements (does not check for bounds).
        const __Reference operator[](PDsizei i) const
        {
            assert(i < size);
            return values[i];
        }

        /// Returns the size of the array in bytes.
        constexpr PDsizei Size() const {return sizeof(Type) * size;}

        /// Returns the length of the array.
        constexpr PDsizei Length() const {return size;}

        /// Return an interator to the beginning of the array.
        __Iterator begin() {return __Iterator(values);}
        __ConstIterator begin() const {return __ConstIterator(values);}

        /// Return an interator to the just past the end of the array.
        __Iterator end() {return __Iterator(static_cast<__Pointer>(values) + size);}
        __ConstIterator end() const {return __ConstIterator(static_cast<__ConstPointer>(values) + size);}

        /// Returns a reverse iterator to the end of the array.
        __ReverseIterator rbegin() noexcept {return __ReverseIterator(this->end());}
        __ConstReverseIterator rbegin() const noexcept {return __ConstReverseIterator(this->end());}

        /// Returns a reverse iterator to the beginning of the array.
        __ReverseIterator rend() noexcept {return __ReverseIterator(this->begin());}
        __ConstReverseIterator rend() const noexcept {return __ConstReverseIterator(this->begin());}

        /// Returns a raw pointer to the array (alias for @doxfunc{begin()}).
        __Pointer Data() noexcept {return static_cast<__Pointer>(&values[0]);}
        __ConstPointer Data() const noexcept {return static_cast<__Pointer>(&values[0]);}

    private:
        __ValueType values[N];
        const PDsizei size = N;
    };
}

#endif /* DEWPSI_ARRAY_H */
