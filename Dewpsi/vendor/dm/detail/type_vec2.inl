#ifndef TYPE_VEC2_INL__DEWPSI_INCLUDED
#define TYPE_VEC2_INL__DEWPSI_INCLUDED

#ifndef TYPE_VEC2_HPP__DEWPSI_INCLUDED
#error "Do not include this file directly! Include type_vec2.hpp"
#endif

#include <cstring>

namespace dm {

    // -- Constructors --

    template<typename T>
    Vec<2, T>::Vec()
    {
        ::std::memset(this, 0, sizeof(*this));
    }

    template<typename T>
    Vec<2, T>::Vec(Vec<2, T>::value_type _value) : x(_value), y(_value)
    {}

    template<typename T>
    Vec<2, T>::Vec(Vec<2, T>::value_type _x, Vec<2, T>::value_type _y)
    : x(_x), y(_y) {}

    template<typename T>
    Vec<2, T>::Vec(const Vec<2, T>& src) : x(src.x), y(src.y) {}

    // -- Accessors --

    template<typename T>
    typename Vec<2, T>::value_type& Vec<2, T>::operator[](Vec<2, T>::length_type i)
    {
        assert(i < 2);
        return (&x)[i];
    }

    template<typename T>
    const typename Vec<2, T>::value_type& Vec<2, T>::operator[](Vec<2, T>::length_type i) const
    {
        assert(i < 2);
        return (&x)[i];
    }

    // -- Relational Operators --

    template<typename T>
    bool Vec<2, T>::operator==(const Vec& b) const
    {
        return (x == b.x) && (y == b.y);
    }

    template<typename T>
    bool Vec<2, T>::operator!=(const Vec& b) const
    {
        return (x != b.x) && (y != b.y);
    }

    template<typename T>
    bool Vec<2, T>::operator<=(const Vec& b) const
    {
        return (x <= b.x) && (y <= b.y);
    }

    template<typename T>
    bool Vec<2, T>::operator>=(const Vec& b) const
    {
        return (x >= b.x) && (y >= b.y);
    }

    template<typename T>
    bool Vec<2, T>::operator<(const Vec& b) const
    {
        return (x < b.x) && (y < b.y);
    }

    template<typename T>
    bool Vec<2, T>::operator>(const Vec& b) const
    {
        return (x > b.x) && (y > b.y);
    }

    // -- Unary Operators --

    template<typename T>
    Vec<2, T>& Vec<2, T>::operator=(const Vec<2, T>& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    template<typename T>
    Vec<2, T>& Vec<2, T>::operator=(Vec<2, T>&& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    template<typename T>
    Vec<2, T>& Vec<2, T>::operator+=(const Vec<2, T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    template<typename T>
    Vec<2, T>& Vec<2, T>::operator+=(T rhs)
    {
        x += rhs;
        y += rhs;
        return *this;
    }

    template<typename T>
    Vec<2, T>& Vec<2, T>::operator-=(const Vec<2, T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    template<typename T>
    Vec<2, T>& Vec<2, T>::operator-=(T rhs)
    {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    template<typename T>
    Vec<2, T>& Vec<2, T>::operator*=(const Vec<2, T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
    template<typename T>
    Vec<2, T>& Vec<2, T>::operator*=(T rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    template<typename T>
    Vec<2, T>& Vec<2, T>::operator/=(const Vec<2, T>& rhs)
    {
        *this = DivideNonZeroV(*this, rhs);
        return *this;
    }
    template<typename T>
    Vec<2, T>& Vec<2, T>::operator/=(T rhs)
    {
        *this = DivideNonZeroV(*this, Vec<2, T>(rhs));
        return *this;
    }

    template<typename T>
    Vec<2, T>& Vec<2, T>::operator%=(const Vec<2, T>& rhs)
    {
        return *this = *this % rhs;
    }
    template<typename T>
    Vec<2, T>& Vec<2, T>::operator%=(T rhs)
    {
        return *this = *this % Vec<2, T>(rhs);
    }

    // -- Vector functions --

    // Calculate length
    template<typename T>
    float Vec<2, T>::Length() const
    {
        const float fX = (float) x;
        const float fY = (float) y;
        return std::sqrt(fX * fX + fY * fY);
    }

    template<>
    float Vec<2, float>::Length() const {return std::sqrt(x * x + y * y);}

    // Retrieve a string with the value of the vector.
    template<typename T>
    std::string Vec<2, T>::GetString() const
    {
        std::stringstream ss;
        ss << '(' << x << ", " << y << ')';
        return ss.str();
    }

    // -- Non-member, non-operator functions --

    // Normalize the vector
    template<typename T>
    Vec<2, T> normalize(const Vec<2, T>& v)
    {
        return v / static_cast<T>(v.Length());
    }

    // Cast vector to a different type
    template<typename To, typename From>
    Vec<2, To> vector_cast(const Vec<2, From>& vector)
    {
        static_assert(std::is_convertible<To, From>::value, "'From' is not convertible to 'To'");
        return Vec<2, To>(
            static_cast<To>(vector.x),
            static_cast<To>(vector.y)
        );
    }

    // Division
    template<typename T, typename U = T>
    Vec<2, T> DivideNonZeroV(const Vec<2, T>& lhs, const Vec<2, U>& rhs)
    {
        static_assert(std::is_convertible<T, U>::value, "U is not convertible to T");
        return Vec<2, T>(
            (lhs.x && rhs.x) ? (lhs.x / rhs.x) : 0,
            (lhs.y && rhs.y) ? (lhs.y / rhs.y) : 0
        );
    }

    template<typename T, typename U = T>
    Vec<2, T> DivideNonZeroV(const Vec<2, T>& lhs, U rhs)
    {
        static_assert(std::is_convertible<T, U>::value, "U is not convertible to T");
        return Vec<2, T>(
            (lhs.x && rhs) ? (lhs.x / rhs) : 0,
            (lhs.y && rhs) ? (lhs.y / rhs) : 0
        );
    }

    // Modulus
    template<typename T, typename U = T>
    Vec<2, T> ModulusNonZeroV(const Vec<2, T>& lhs, const Vec<2, U>& rhs)
    {
        static_assert(std::is_convertible<T, U>::value, "U is not convertible to T");
        return Vec<2, T>(
            (lhs.x && rhs.x) ? (lhs.x % rhs.x) : 0,
            (lhs.y && rhs.y) ? (lhs.y % rhs.y) : 0
        );
    }

    template<typename T, typename U = T>
    Vec<2, T> ModulusNonZeroV(const Vec<2, T>& lhs, U rhs)
    {
        static_assert(std::is_convertible<T, U>::value, "U is not convertible to T");
        return Vec<2, T>(
            (lhs.x && rhs) ? (lhs.x % rhs) : 0,
            (lhs.y && rhs) ? (lhs.y % rhs) : 0
        );
    }

    // -- Binary operators --

    template<typename T>
    Vec<2, T> operator+(const Vec<2, T>& lhs, const Vec<2, T>& rhs)
    {
        Vec<2, T> res;
        res[0] = lhs[0] + rhs[0];
        res[1] = lhs[1] + rhs[1];
        return res;
    }

    template<typename T>
    Vec<2, T> operator+(const Vec<2, T>& lhs, T rhs)
    {
        Vec<2, T> res;
        res[0] = lhs[0] + rhs;
        res[1] = lhs[1] + rhs;
        return res;
    }

    template<typename T>
    Vec<2, T> operator+(T lhs, const Vec<2, T>& rhs)
    {
        Vec<2, T> res;
        res[0] = lhs + rhs[0];
        res[1] = lhs + rhs[1];
        return res;
    }


    template<typename T>
    Vec<2, T> operator-(const Vec<2, T>& lhs, const Vec<2, T>& rhs)
    {
        Vec<2, T> res;
        res[0] = lhs[0] - rhs[0];
        res[1] = lhs[1] - rhs[1];
        return res;
    }

    template<typename T>
    Vec<2, T> operator-(const Vec<2, T>& lhs, T rhs)
    {
        Vec<2, T> res;
        res[0] = lhs[0] - rhs;
        res[1] = lhs[1] - rhs;
        return res;
    }

    template<typename T>
    Vec<2, T> operator-(T lhs, const Vec<2, T>& rhs)
    {
        Vec<2, T> res;
        res[0] = lhs - rhs[0];
        res[1] = lhs - rhs[1];
        return res;
    }


    template<typename T>
    Vec<2, T> operator*(const Vec<2, T>& lhs, const Vec<2, T>& rhs)
    {
        Vec<2, T> res;
        res[0] = lhs[0] * rhs[0];
        res[1] = lhs[1] * rhs[1];
        return res;
    }

    template<typename T>
    Vec<2, T> operator*(const Vec<2, T>& lhs, T rhs)
    {
        Vec<2, T> res;
        res[0] = lhs[0] * rhs;
        res[1] = lhs[1] * rhs;
        return res;
    }

    template<typename T>
    Vec<2, T> operator*(T lhs, const Vec<2, T>& rhs)
    {
        Vec<2, T> res;
        res[0] = lhs * rhs[0];
        res[1] = lhs * rhs[1];
        return res;
    }


    // Division
    template<typename T>
    Vec<2, T> operator/(const Vec<2, T>& lhs, const Vec<2, T>& rhs)
    {
        return DivideNonZeroV(lhs, rhs);
    }

    template<typename T>
    Vec<2, T> operator/(const Vec<2, T>& lhs, T rhs)
    {
        return DivideNonZeroV(lhs, rhs);
    }

    template<typename T>
    Vec<2, T> operator/(T lhs, const Vec<2, T>& rhs)
    {
        return DivideNonZeroV(rhs, lhs);
    }


    // Modulus
    template<typename T>
    Vec<2, T> operator%(const Vec<2, T>& lhs, const Vec<2, T>& rhs)
    {
        return ModulusNonZeroV(lhs, rhs);
    }

    template<typename T>
    Vec<2, T> operator%(const Vec<2, T>& lhs, T rhs)
    {
        return ModulusNonZeroV(lhs, rhs);
    }

    template<typename T>
    Vec<2, T> operator%(T lhs, const Vec<2, T>& rhs)
    {
        return ModulusNonZeroV(rhs, lhs);
    }

    // Single-precision floating point modulus
    template<>
    Vec<2, float> operator%(const Vec<2, float>& lhs, const Vec<2, float>& rhs)
    {
        return lhs;
    }

    template<>
    Vec<2, float> operator%(const Vec<2, float>& lhs, float rhs)
    {
        return lhs;
    }

    template<>
    Vec<2, float> operator%(float lhs, const Vec<2, float>& rhs)
    {
        return rhs;
    }

    // Double-precision floating point modulus
    template<>
    Vec<2, double> operator%(const Vec<2, double>& lhs, const Vec<2, double>& rhs)
    {
        return lhs;
    }

    template<>
    Vec<2, double> operator%(const Vec<2, double>& lhs, double rhs)
    {
        return lhs;
    }

    template<>
    Vec<2, double> operator%(double lhs, const Vec<2, double>& rhs)
    {
        return rhs;
    }

} // namespace dm

#endif /* TYPE_VEC2_INL__DEWPSI_INCLUDED */
