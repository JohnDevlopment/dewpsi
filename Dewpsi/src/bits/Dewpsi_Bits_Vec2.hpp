#ifndef DEWPSI_BITS_VEC2_HPP
#define DEWPSI_BITS_VEC2_HPP

#include <Dewpsi_Core.h>

#ifdef _DOXYGEN_DO_INCLUDE
namespace Dewpsi {
#endif /* _DOXYGEN_DO_INCLUDE */

/** @class      Dewpsi::TVector2D
*   @brief      A 2D vector (x, y).
*
*   @tparam     Type        An integral or floating-point type
*   @section    sec_ctors   Constructor
*   @include    vector2_ctor.cpp
*   Each constructor does the following:
*   -# Constructs a vector that's initialized to zero.
*   -# Constructs a copy of @a rhs.
*   -# Aquires the contents of @a rhs.
*   -# Constructs a vector initialized to @a x and @a y.
*   -# Constructs a vector initialized to @a val.
*   -# Copies one or two values in @a il to the vector, in order.
*
*   @par Relational Operators
*   The ==, !=, <, >, <=, and >= operators are supported.
*   @include vector2_relops.cpp
*   -# Checks for the equality of two vectors.
*   -# Checks for the inequality of two vectors.
*   -# Checks if the vector is less than @a rhs.
*   -# Checks if the vector is less than or equal to @a rhs.
*   -# Checks if the vector is greater than @a rhs.
*   -# Checks if the vector is greater than or equal to @a rhs.
*
*   @par Arithmetic Operators
*   @include vector2_mathops.cpp
*   The math operators +, -, *, and / are supported for arithmetic between a vector
*   and a vector and between a vector and a value. The compound assignment operators
*   +=, -=, *=, and /= are supported for between a vector and another vector.
*
*   @ingroup    vectors
*/
template<typename Type>
struct TVector2D {
    /// An alias of @c Type.
    typedef Type value_type;

    TVector2D() : x(0), y(0) {}
    TVector2D(const value_type& _x, const value_type& _y) : x(_x), y(_y) {}
    TVector2D(const value_type& _val) : x(_val), y(_val) {}
    TVector2D(const TVector2D& rhs) : x(rhs.x), y(rhs.y) {}
    TVector2D(TVector2D&& rhs) : x(rhs.x), y(rhs.y) {}
    TVector2D(std::initializer_list<Type> il) : x(0), y(0)
    {
        PDuint i = 0;

        const value_type* const end = il.end();
        for (const value_type* itr = il.begin(); itr != end; ++itr)
        {
            if (i >= 2) break;
            reinterpret_cast<value_type*>(&x)[i++] = *itr;
        }
    }

    // Default destructor.
    ~TVector2D() = default;


    TVector2D operator+(const TVector2D& rhs) const
    {
        TVector2D res = {x + rhs.x, y + rhs.y};
        return res;
    }
    TVector2D operator-(const TVector2D& rhs) const
    {
        TVector2D res = {x - rhs.x, y - rhs.y};
        return res;
    }
    TVector2D operator*(const TVector2D& rhs) const
    {
        TVector2D res = {x * rhs.x, y * rhs.y};
        return res;
    }

    TVector2D operator/(const TVector2D& rhs) const
    {
        TVector2D res;
        res.x = (res.x && rhs.x) ? (res.x / rhs.x) : 0;
        res.y = (res.y && rhs.y) ? (res.y / rhs.y) : 0;

        return res;
    }

    TVector2D operator+(const value_type& rhs) const
    {
        return {x + rhs, y + rhs};
    }

    TVector2D operator-(const value_type& rhs) const
    {
        return {x - rhs, y - rhs};
    }
    TVector2D operator*(const value_type& rhs) const
    {
        return {x * rhs, y * rhs};
    }

    TVector2D operator/(const value_type& rhs) const
    {
        TVector2D res;
        if (rhs)
        {
            res.x = x ? (x / rhs) : 0;
            res.y = y ? (y / rhs) : 0;
        }

        return res;
    }

    // Compound assignment

    TVector2D& operator+=(const TVector2D& rhs)
    {
        x += rhs.x;
        y += rhs.y;

    return *this;
    }
    TVector2D& operator-=(const TVector2D& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

    return *this;
    }
    TVector2D& operator*=(const TVector2D& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;

    return *this;
    }
    TVector2D& operator/=(const TVector2D& rhs)
    {
        x = (x && rhs.x) ? (x / rhs.x) : 0;
        y = (y && rhs.y) ? (y / rhs.y) : 0;

    return *this;
    }

    TVector2D& operator+=(const value_type& rhs)
    {
        x += rhs;
        y += rhs;

    return *this;
    }
    TVector2D& operator-=(const value_type& rhs)
    {
        x -= rhs;
        y -= rhs;

    return *this;
    }
    TVector2D& operator*=(const value_type& rhs)
    {
        x *= rhs;
        y *= rhs;

    return *this;
    }
    TVector2D& operator/=(const value_type& rhs)
    {
        x = (x && rhs) ? (x / rhs) : 0;
        y = (y && rhs) ? (y / rhs) : 0;

    return *this;
    }



    // Relational operators

    bool operator==(const TVector2D& rhs) const {return x == rhs.x && y == rhs.y;}
    bool operator!=(const TVector2D& rhs) const {return x != rhs.x && y != rhs.y;}
    bool operator<(const TVector2D& rhs) const {return x < rhs.x && y < rhs.y;}
    bool operator<=(const TVector2D& rhs) const {return x <= rhs.x && y <= rhs.y;}
    bool operator>(const TVector2D& rhs) const {return x > rhs.x && y > rhs.y;}
    bool operator>=(const TVector2D& rhs) const {return x >= rhs.x && y >= rhs.y;}

    // Assignment operators

    TVector2D& operator=(const TVector2D& rhs)
    {
        Copy(rhs);
        return *this;
    }

    TVector2D& operator=(TVector2D&& rhs)
    {
        Copy(rhs);
        return *this;
    }

    /// Return the length of the vector.
    float Length() const;

    /// Return a normal-length vector.
    TVector2D<float> Normalized() const;

    Type x; ///< X component
    Type y; ///< Y component

    /// A helper to the copy constructor.
    void Copy(const TVector2D& src);

    /** Divides the components of @c %TVector2D by @a val.
    *   Internally, the members @a x and @a y are casted to floats,
    *   divided by @a val, and then cast back to @c Type.
    */
    void Divide(float val);
};

template<typename _Tp>
float TVector2D<_Tp>::Length() const
{
    return (float) std::sqrt(x * x + y * y);
}

template<typename _Tp>
TVector2D<float> TVector2D<_Tp>::Normalized() const
{
    TVector2D<float> result = {(float) x, (float) y};
    result.Divide(result.Length());

    return result;
}

template<typename _Tp>
void TVector2D<_Tp>::Copy(const TVector2D<_Tp>& src)
{
    x = src.x;
    y = src.y;
}

template<typename _Tp>
void TVector2D<_Tp>::Divide(float val)
{
    x = (x && val) ? (x / val) : 0;
    y = (y && val) ? (y / val) : 0;
}

/** A specialization of TVector2D that utilizes floats
*   @ingroup vectors
*/
typedef TVector2D<float> Vector2D;

#ifdef _DOXYGEN_DO_INCLUDE
}
#endif /* _DOXYGEN_DO_INCLUDE */

#endif /* DEWPSI_BITS_VEC2_HPP */
