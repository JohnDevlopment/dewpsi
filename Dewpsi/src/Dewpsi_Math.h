#ifndef DEWPSI_MATH_H
#define DEWPSI_MATH_H

/**
*   @file       Dewpsi_Math.h
*   @brief      @doxfb
*   Contains math functions.
*
*   @defgroup math Math
*   @ingroup core
*/

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /** Returns a value within a range.
    *   
    *   @param  min The smallest value in the range
    *   @param  max The largest value in the range
    *   @param  val The value being clamped
    *   @return     If @a val is less than @a min, min is returned;
    *               if greater than @a max, max is returned; else,
    *               @a val is returned
    *   @ingroup    math
    */
    template<typename T>
    T Clamp(T min, T max, T val)
    {
        if (val < min)
            return min;
        else if (val > max)
            return max;
        
        return val;
    }
    
    /** Returns the smaller of two values.
    *   The type of both parameters is any type that can support
    *   the less-than operator.
    *   
    *   @param  a   A value
    *   @param  b   A value
    *   @return     @a a if it is less than @a b, otherwise @a b
    *   @ingroup    math
    */
    template<typename T>
    T min(T a, T b)
    {
        if (a < b) return a;
        return b;
    }
    
    /** Returns the larger of two values.
    *   The type of both parameters is any type that can support
    *   the greater-than operator.
    *
    *   @param  a   A value
    *   @param  b   A value
    *   @return     @a a if it is greater than @a b, otherwise @a b
    *   @ingroup    math
    */
    template<typename T>
    T max(T a, T b)
    {
        if (a > b) return a;
        return b;
    }
}

#endif /* DEWPSI_MATH_H */
