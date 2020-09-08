#ifndef DEWPSI_VECTOR_H
#define DEWPSI_VECTOR_H

#include <Dewpsi_Core.h>
#include <initializer_list>
#include <cmath>
#include <Dewpsi_Traits.h>

/**
*   @file       Dewpsi_Vector.h
*   @brief      @doxfb
*   Prototypes for vectors.
*
*   @defgroup vectors Vectors
*   @ingroup math
*/

namespace Dewpsi {
    /// @cond NEVER
    #include <bits/Dewpsi_Bits_Vec3.hpp>
    #include <bits/Dewpsi_Bits_Vec2.hpp>
    /// @endcond

    template<typename To, typename From>
    TVector2D<To> VectorCast(const TVector2D<From>& vct)
    {
        return {static_cast<To>(vct.x), static_cast<To>(vct.y)};
    }

    /** Creates a 2D vector.
    *   @param  x   X component
    *   @param  y   Y component
    *   @return     A @doxtype{TVector2D} with the X and Y components
    *   @ingroup    vectors
    */
    template<typename T>
    TVector2D<T> Create2DVector(T x, T y)
    {
        return TVector2D<T>(x, y);
    }

    /** Constructs a 2D vector.
    *   @param  xy  Used to initialize the X and Y components
    *   @return     A @doxtype{TVector2D} with the X and Y components
    *   @ingroup    vectors
    */
    template<typename T>
    TVector2D<T> Create2DVector(T xy)
    {
        return TVector2D<T>(xy, xy);
    }

    /// This function is disabled.
    template<typename T>
    TVector2D<T> Create2DVector() = delete;
}

/// Extraction operator between @c std::ostream and TVector3D.
template<typename _valuetype>
inline std::ostream& operator<<(std::ostream& os, const Dewpsi::TVector3D<_valuetype>& vec)
{
    os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
}

/// Extraction operator between @c std::ostream and TVector2D.
template<typename _valuetype>
inline std::ostream& operator<<(std::ostream& os, const Dewpsi::TVector2D<_valuetype>& vec)
{
    os << '(' << vec.x << ", " << vec.y << ')';
}

#endif /* DEWPSI_VECTOR_H */
