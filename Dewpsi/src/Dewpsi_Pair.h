#ifndef DEWPSI_PAIR_H
#define DEWPSI_PAIR_H

/// @file Dewpsi_Pair.h
/// @ref core

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /// A pair.
    template<class T1, class T2>
    struct Pair {
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;
    };
}

#endif /* DEWPSI_PAIR_H */
