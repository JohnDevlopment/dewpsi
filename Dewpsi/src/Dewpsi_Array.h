#ifndef DEWPSI_ARRAY_H
#define DEWPSI_ARRAY_H

/// @file Dewpsi_Array.h
/// @ref core

//#include <Dewpsi_Core.h>
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
        typedef Type value_type;      ///< Type of each element
        typedef Type* pointer_type;   ///< Pointer of @c value_type
        typedef Type& reference_type; ///< Reference of @c value_type

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

        /// Obtain a reference to one of the elements (does not check for bounds).
        reference_type operator[](PDsizei i)
        {
            assert(i < N);
            return values[i];
        }

        /// Obtain an immutable reference to one of the elements (does not check for bounds).
        const reference_type operator[](PDsizei i) const
        {
            assert(i < N);
            return values[i];
        }

        /// Return the size of the array.
        constexpr PDsizei Size() const {return N;}

        Type values[N];
    };
}

#endif /* DEWPSI_ARRAY_H */
