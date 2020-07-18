#ifndef DEWPSI_MATH_H
#define DEWPSI_MATH_H

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /// Returns a value within a range.
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
