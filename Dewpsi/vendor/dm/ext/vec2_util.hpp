#ifndef VEC2_UTIL_HPP__DEWPSI_INCLUDED
#define VEC2_UTIL_HPP__DEWPSI_INCLUDED

#include "../detail/type_vec2.hpp"
#include "../DewpsiMath_Util.hpp"

namespace dm {
    //template<typename T>
    //struct Vec<2, T>;

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
    Vec<2, V> ratio(T a, T b)
    {
        static_assert(std::is_convertible<T, V>::value, "Type T cannot be converted to type U");

        if (a == b)
            return Vec<2, V>(a);

        T divisor = mcd(a, b);
        assert(divisor != 1);
        Vec<2, V> result(a, b);

        while (! (result.x % divisor) && ! (result.y % divisor))
            result /= Vec<2, V>(divisor);

        return result;
    }

    /// A version of ratio that accepts a two-component vector.
    template<typename T>
    inline Vec<2, T> ratio(const Vec<2, T>& v)
    {
        return ratio(v.x, v.y);
    }
}

#endif /* VEC2_UTIL_HPP__DEWPSI_INCLUDED */
