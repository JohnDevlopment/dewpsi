#ifndef DEWPSI_VECTOR_H
#define DEWPSI_VECTOR_H

#include <Dewpsi_Core.h>
#include <initializer_list>
#include <cmath>


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
