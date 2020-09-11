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
#include <Dewpsi_Vector.h>
#include <Dewpsi_Traits.h>
#include <Dewpsi_Debug.h>

namespace Dewpsi {
    /** Obtain the absolute value of @a val.
    *   @param  val A value of an integral or floating-point type.
    *   @return     The absolute value of @a val. If @a T is an unsigned
    *               type, this just returns @a val.
    *   @tparam T   A type
    */
    template<typename T>
    typename std::enable_if<std::is_signed<T>::value, T>::type
    abs(T val) {
        std::cout << "generic abs()" << std::endl;
        return (val < 0 ? -val : val);
    }
    template<typename T>
    typename std::enable_if<!std::is_signed<T>::value, T>::type
    abs(T val)
    {
        std::cout << "unsigned_type abs()" << std::endl;
        return val;
    }

    /** Returns a value within a range.
    *
    *   @param  min The smallest value in the range
    *   @param  max The largest value in the range
    *   @param  val The value being clamped
    *   @tparam T   Any type that supports the less-than and greater-than operators
    *   @return     If @a val is less than @a min, min is returned;
    *               if greater than @a max, max is returned; else,
    *               @a val is returned
    *   @ingroup    math
    */
    template<typename T>
    T clamp(T min, T max, T val)
    {
        if (val < min)
            return min;
        else if (val > max)
            return max;

        return val;
    }

    /** Returns the smaller of two values.
    *   @param  a   A value
    *   @param  b   A value
    *   @tparam T   Any type that supports the less-than operator
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
    *   @param  a   A value
    *   @param  b   A value
    *   @tparam T   Any type that supports the greater-than operator
    *   @return     @a a if it is greater than @a b, otherwise @a b
    *   @ingroup    math
    */
    template<typename T>
    T max(T a, T b)
    {
        if (a > b) return a;
        return b;
    }

    /** Calculates the "most common divisor" among two numbers.
    *   @param  a,b Two numbers that are inequal to each other
    *   @return     The most common divisor of @a and @a b, that is,
    *               the highest number that the two can be divided by.
    *   @tparam T   A numerical type
    */
    template<typename T>
    T mcd(T a, T b)
    {
        if (a == b)
            return a;
        else if (a == 1 || b == 1)
            return 1;

        T result = min(a, b);

        for (; result > 1; --result)
        {
            if (! (a % result) && ! (b % result))
                break;
        }

        return result;
    }

    /** Calculates the ratio between two numbers.
    *   If the two numbers are equal, the number is returned.
    *   This function does not compile if @c V cannot be assigned
    *   a value of @c T.
    *   @param  a,b Numbers
    *   @tparam T   A numeric type of the parameters
    *   @tparam V   The underlying type of the vector
    *   @return     A 2D vector representing the ration between @a a and @a b
    */
    template<typename T, typename V = T>
    TVector2D<V> ratio(T a, T b)
    {
        constexpr int iMaxLoops = 100;

        static_assert(std::is_convertible<T, V>::value, "Type T cannot be converted to type U");

        T divisor = mcd(a, b);
        assert(divisor != 1);
        TVector2D<V> result(a, b);

        while (! (result.x % divisor) && ! (result.y % divisor))
        {
            result /= TVector2D<V>(divisor);
        }

        return result;
    }

    template<typename T>
    inline TVector2D<T> ratio(const TVector2D<T>& v)
    {
        return ratio(v.x, v.y);
    }
}

#endif /* DEWPSI_MATH_H */
