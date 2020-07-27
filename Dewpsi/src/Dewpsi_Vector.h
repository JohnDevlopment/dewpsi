#ifndef DEWPSI_VECTOR_H
#define DEWPSI_VECTOR_H

/**
*   @file       Dewpsi_Vector.h
*   @brief      @doxfb
*   Contains definitions of vectors.
*
*   @defgroup vectors Vectors
*   @ingroup math
*/

#include <Dewpsi_Core.h>
#include <iostream>

namespace Dewpsi {
    /**
    *   @addtogroup vectors
    *   @{
    */
    
    /// A two-component vector.
    struct Vector2 {
        float x; ///< X component
        float y; ///< Y component
        
        /// Constructs an empty vector.
        Vector2() : x(0), y(0)
        {  }
        
        /// Constructs and initializes a vector with an x and y component.
        Vector2(float sx, float sy) : x(sx), y(sy)
        {  }
        
        /// Constructs a vector that is a copy of @a src.
        Vector2(const Vector2& src) : x(src.x), y(src.y)
        {  }
        
        /// Constructs a vector that is a copy of @a src.
        Vector2(Vector2&& src) : x(src.x), y(src.y)
        {  }
        
        /** Adds two vectors.
        *   @par Example
        *   @dontinclude vectors.cpp
        *   @snippet vectors.cpp Add Vectors
        */
        Vector2 operator+(const Vector2& rhs)
        {
            Vector2 vec = *this;
            vec.x += rhs.x;
            vec.y += rhs.y;
            return vec;
        }
        
        /** Subtracts two vectors.
        *   @par Example
        *   @dontinclude vectors.cpp
        *   @snippet vectors.cpp Subtract Vectors
        */
        Vector2 operator-(const Vector2& rhs)
        {
            Vector2 vec = *this;
            vec.x -= rhs.x;
            vec.y -= rhs.y;
            return vec;
        }
        
        /** Multiplies two vectors.
        *   @par Example
        *   @dontinclude vectors.cpp
        *   @snippet vectors.cpp Multiply Vectors
        */
        Vector2 operator*(const Vector2& rhs)
        {
            Vector2 vec = *this;
            vec.x *= rhs.x;
            vec.y *= rhs.y;
            return vec;
        }
        
        /** Divides two vectors.
        *   @par Example
        *   @dontinclude vectors.cpp
        *   @snippet vectors.cpp Divide Vectors
        */
        Vector2 operator/(const Vector2& rhs)
        {
            Vector2 vec = *this;
            vec.x /= rhs.x;
            vec.y /= rhs.y;
            return vec;
        }
    };
    
    /// @}
}

std::ostream& operator<<(std::ostream& os, const Dewpsi::Vector2& vec2)
{
    os << '(' << vec2.x << ", " << vec2.y << ')';
}

#endif /* DEWPSI_VECTOR_H */
