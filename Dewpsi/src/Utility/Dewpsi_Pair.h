#ifndef DEWPSI_PAIR_H
#define DEWPSI_PAIR_H

/// @file Dewpsi_Pair.h
/// @ref core

#include <Dewpsi_Core.h>
#include <initializer_list>

namespace Dewpsi {
    /** A pair of values of any type.
    *   @tparam T1,T2 Any type
    *   @ingroup core
    */
    template<class T1, class T2>
    struct Pair {
        typedef T1 first_type; ///< Type of the member @a first
        typedef T2 second_type; ///< Type of the member @a second

        first_type first; ///< The first member of the pair of type @c T1
        second_type second; ///< The second member of the pair of type @c T2

        /// Default constructor.
        Pair() = default;

        /// Default destructor.
        ~Pair() = default;

        /// Initialize with @a _first and @a _second.
        Pair(T1 _first, T2 _second) : first(_first), second(_second) {}

        /// Copy constructor.
        Pair(const Pair& src) : first(src.first), second(src.second) {}

        /// Move constructor.
        Pair(Pair&& src) : first(src.first), second(src.second)
        {
            src.first  = (T1) 0;
            src.second = (T2) 0;
        }
    };
}

#endif /* DEWPSI_PAIR_H */
