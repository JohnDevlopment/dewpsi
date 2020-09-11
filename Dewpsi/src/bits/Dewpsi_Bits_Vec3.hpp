#ifndef DEWPSI_BITS_VEC3_HPP
#define DEWPSI_BITS_VEC3_HPP

#include <Dewpsi_Core.h>

#ifdef _DOXYGEN_DO_INCLUDE
namespace Dewpsi {
#endif /* _DOXYGEN_DO_INCLUDE */

/** @class      Dewpsi::TVector3D
*   @brief      A 3D vector (x, y, z)
*   @tparam     Type An integral or floating-point type
*   @par        Constructor
*   @include    vector3_ctor.cpp
*   Each constructor does the following:
*   -# Constructs a vector that's initialized to zero.
*   -# Constructs a copy of @a rhs.
*   -# Aquires the contents of @a rhs.
*   -# Constructs a vector initialized to @a x and @a y.
*   -# Constructs a vector initialized to @a val.
*   -# Copies one or three values in @a il to the vector, in order.
*
*   @par Relational Operators
*   The ==, !=, <, >, <=, and >= operators are supported.
*   @include vector3_relops.cpp
*   -# Checks for the equality of three vectors.
*   -# Checks for the inequality of three vectors.
*   -# Checks if the vector is less than @a rhs.
*   -# Checks if the vector is less than or equal to @a rhs.
*   -# Checks if the vector is greater than @a rhs.
*   -# Checks if the vector is greater than or equal to @a rhs.
*
*   @par Arithmetic Operators
*   @include vector3_mathops.cpp
*   The math operators +, -, *, and / are supported for arithmetic between a vector
*   and a vector and between a vector and a value. The compound assignment operators
*   +=, -=, *=, and /= are supported for between a vector and another vector.
*
*   @ingroup    vectors
*/
template<typename Type>
struct TVector3D {
    /// Type of the vector components
    typedef Type value_type;

    TVector3D() : x(0), y(0), z(0) {}
    TVector3D(const TVector3D& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}
    TVector3D(TVector3D&& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}
    TVector3D(const value_type& _x, const value_type& _y, const value_type& _z)
    : x(_x), y(_y), z(_z) {}
    TVector3D(const value_type& _val) : x(_val), y(_val), z(_val) {}
    TVector3D(std::initializer_list<Type> il) : x(0), y(0), z(0)
    {
        PDuint i = 0;

        const value_type* const end = il.end();
        for (const value_type* itr = il.begin(); itr != end; ++itr)
        {
            if (i >= 3) break;
            reinterpret_cast<value_type*>(&x)[i++] = *itr;
        }
    }

    /// Default destructor.
    ~TVector3D() = default;

    TVector3D operator+(const TVector3D& rhs) const
    {
        TVector3D res = {x + rhs.x, y + rhs.y, z + rhs.z};
        return res;
    }
    TVector3D operator-(const TVector3D& rhs) const
    {
        TVector3D res = {x - rhs.x, y - rhs.y, z - rhs.z};
        return res;
    }
    TVector3D operator*(const TVector3D& rhs) const
    {
        TVector3D res = {x * rhs.x, y * rhs.y, z * rhs.z};
        return res;
    }
    TVector3D operator/(const TVector3D& rhs) const
    {
        TVector3D res;
        res.x = (res.x && rhs.x) ? (res.x / rhs.x) : 0;
        res.y = (res.y && rhs.y) ? (res.y / rhs.y) : 0;
        res.z = (res.z && rhs.z) ? (res.z / rhs.z) : 0;

        return res;
    }
    TVector3D operator+(const value_type& rhs) const
    {
        return {x + rhs, y + rhs, z + rhs};
    }
    TVector3D operator-(const value_type& rhs) const
    {
        return {x - rhs, y - rhs, z - rhs};
    }
    TVector3D operator*(const value_type& rhs) const
    {
        return {x * rhs, y * rhs, z * rhs};
    }

    TVector3D operator/(const value_type& rhs) const
    {
        TVector3D res;
        if (rhs)
        {
            res.x = x ? (x / rhs) : 0;
            res.y = y ? (y / rhs) : 0;
            res.z = z ? (z / rhs) : 0;
        }

        return res;
    }

    TVector3D& operator+=(const TVector3D& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

    return *this;
    }
    TVector3D& operator-=(const TVector3D& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

    return *this;
    }
    TVector3D& operator*=(const TVector3D& rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;

    return *this;
    }
    TVector3D& operator/=(const TVector3D& rhs) {
        x = (x && rhs.x) ? (x / rhs.x) : 0;
        y = (y && rhs.y) ? (y / rhs.y) : 0;
        z = (z && rhs.z) ? (z / rhs.z) : 0;

    return *this;
    }

    // Relational operators

    bool operator==(const TVector3D& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    bool operator!=(const TVector3D& rhs) const
    {
        return x != rhs.x && y != rhs.y;
    }
    bool operator<(const TVector3D& rhs) const
    {
        return x < rhs.x && y < rhs.y && z < rhs.z;
    }
    bool operator<=(const TVector3D& rhs) const
    {
        return x <= rhs.x && y <= rhs.y && z <= rhs.z;
    }
    bool operator>(const TVector3D& rhs) const
    {
        return x > rhs.x && y > rhs.y && z > rhs.z;
    }
    bool operator>=(const TVector3D& rhs) const
    {
        return x >= rhs.x && y >= rhs.y && z >= rhs.z;
    }

    // Assignment

    TVector3D& operator=(const TVector3D& rhs)
    {
        Copy(rhs);
        return *this;
    }

    TVector3D& operator=(TVector3D&& rhs)
    {
        Copy(rhs);
        return *this;
    }

    /// Return the length of this vector.
    float Length() const;

    /// Return a normal-length vector.
    TVector3D<float> Normalized() const;

    Type x; ///< X component
    Type y; ///< Y component
    Type z; ///< Z component

    void Copy(const TVector3D& src);
    void Divide(float val);
};

template<typename _Tp>
float TVector3D<_Tp>::Length() const
{
    return (float) std::sqrt(x * x + y * y + z * z);
}

template<typename _Tp>
TVector3D<float> TVector3D<_Tp>::Normalized() const
{
    TVector3D<float> result = {(float) x, (float) y, (float) z};

    result.Divide(result.Length());

    return result;
}

template<typename _Tp>
void TVector3D<_Tp>::Copy(const TVector3D<_Tp>& src)
{
    x = src.x;
    y = src.y;
    z = src.z;
}

template<typename _Tp>
void TVector3D<_Tp>::Divide(float val)
{
    x = (x && val) ? (x / val) : 0;
    y = (y && val) ? (y / val) : 0;
    z = (z && val) ? (z / val) : 0;
}

/** A specialization of TVector3D that utilizes floats
*   @ingroup vectors
*/
typedef TVector3D<float> Vector3D;

/** Constructs a 3D vector with X, Y, and Z components
*   the underlying type of which is @a T.
*
*   @param  x   The X component
*   @param  y   The Y component
*   @param  z   The Z component
*   @return     TVector3D of type @a T
*/
template<typename T>
TVector3D<T> MakeVector3D(T x, T y, T z)
{
    TVector3D<T> vec = {x, y, z};
    return vec;
}

#ifdef _DOXYGEN_DO_INCLUDE
}
#endif /* _DOXYGEN_DO_INCLUDE */

#endif /* DEWPSI_BITS_VEC3_HPP */
