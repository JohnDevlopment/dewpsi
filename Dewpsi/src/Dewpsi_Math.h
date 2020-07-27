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
}

#endif /* DEWPSI_MATH_H */
