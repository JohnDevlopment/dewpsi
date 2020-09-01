#ifndef DEWPSI_BITS_VEC2_HPP
#define DEWPSI_BITS_VEC2_HPP

#include <Dewpsi_Core.h>

#ifdef _DOXYGEN_DO_INCLUDE
namespace Dewpsi {
#endif /* _DOXYGEN_DO_INCLUDE */

/** @class Dewpsi::TVector2D
*   @brief A 2D vector (x, y)
*   @tparam Type An integral or floating-point type
*   @ingroup vectors
*/
template<typename Type>
struct TVector2D {
    /// Constructs an empty vector.
    TVector2D() : x(0), y(0)
    {  }

    /** Initializes a vector with @a x, @a y, and @a z components.
    *
    *   @param  _x  The X component
    *   @param  _y  The Y component
    */
    TVector2D(Type _x, Type _y) : x(_x), y(_y)
    {  }

    /// Destroys the object.
    ~TVector2D() {}

    /// Copy constructor
    TVector2D(const TVector2D& rhs) : x(rhs.x), y(rhs.y) {}

    /// Move constructor
    TVector2D(TVector2D&& rhs) : x(rhs.x), y(rhs.y) {}

    /** Initialize vector with a list of values.
    *   @snippet vectors.cpp Initialize Vector
    */
    TVector2D(std::initializer_list<Type> il) : x(0), y(0)
    {
        PDuint i = 0;

        const Type* const end = il.end();
        for (const Type* itr = il.begin(); itr != end; ++itr)
        {
            if (i >= 2) break;
            reinterpret_cast<Type*>(&x)[i++] = *itr;
        }
    }

    /** Add two vectors together.
    *
    *   @snippet vectors.cpp Add Vectors
    */
    TVector2D operator+(const TVector2D& rhs) const
    {
        TVector2D res = {x, y};
        res.x += rhs.x;
        res.y += rhs.y;
        return res;
    }

    /** Subtract two vectors.
    *
    *   @snippet vectors.cpp Subtract Vectors
    */
    TVector2D operator-(const TVector2D& rhs) const
    {
        TVector2D res = {x, y};
        res.x -= rhs.x;
        res.y -= rhs.y;
        return res;
    }

    /** Multiply two vectors.
    *
    *   @snippet vectors.cpp Multiply Vectors
    */
    TVector2D operator*(const TVector2D& rhs) const
    {
        TVector2D res = {x, y};
        res.x *= rhs.x;
        res.y *= rhs.y;
        return res;
    }

    /** Divide two vectors.
    *
    *   @snippet vectors.cpp Divide Vectors
    */
    TVector2D operator/(const TVector2D& rhs) const
    {
        TVector2D res = {x, y};

        res.x = (res.x && rhs.x) ? (res.x / rhs.x) : 0;
        res.y = (res.y && rhs.y) ? (res.y / rhs.y) : 0;

        return res;
    }

    /// Compond addition
    TVector2D& operator+=(const TVector2D& rhs) {
        x = x + rhs.x;
        y = y + rhs.y;

    return *this;
    }

    /// Compond subtraction
    TVector2D& operator-=(const TVector2D& rhs) {
        x = x - rhs.x;
        y = y - rhs.y;

    return *this;
    }

    /// Compond multiplication
    TVector2D& operator*=(const TVector2D& rhs) {
        x = x * rhs.x;
        y = y * rhs.y;

    return *this;
    }

    /// Compond division
    TVector2D& operator/=(const TVector2D& rhs) {
        x = (x && rhs.x) ? (x / rhs.x) : 0;
        y = (y && rhs.y) ? (y / rhs.y) : 0;

    return *this;
    }

    /// Returns true if two vectors are equal.
    bool operator==(const TVector2D& rhs) const {
        return (x == rhs.x) && (y == rhs.y);
    }

    /// Returns true if two vectors are not equal.
    bool operator!=(const TVector2D& rhs) const {
        return (x != rhs.x) && (y != rhs.y);
    }

    /** Returns true if the vector is less than @a rhs.
    *   @snippet vectors.cpp Bool Less
    */
    bool operator<(const TVector2D& rhs) const {
        return (x < rhs.x) && (y < rhs.y);
    }

    /** Returns true if the vector is less than or equal to @a rhs.
    *   @snippet vectors.cpp Bool LessEqual
    */
    bool operator<=(const TVector2D& rhs) const {
        return x <= rhs.x && y <= rhs.y;
    }

    /** Returns true if the vector is greater than @a rhs.
    *   @snippet vectors.cpp Bool Great
    */
    bool operator>(const TVector2D& rhs) const {
        return x > rhs.x && y > rhs.y;
    }

    /** Returns true if the vector is greater than or equal to @a rhs.
    *   @snippet vectors.cpp Bool GreatEqual
    */
    bool operator>=(const TVector2D& rhs) const {
        return x >= rhs.x && y >= rhs.y;
    }

    /// Assignment operator
    TVector2D& operator=(const TVector2D& rhs)
    {
        Copy(rhs);
        return *this;
    }

    /// Assignment operator
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

/** Constructs a 2D vector with X and Y components of type @c T.
*
*   @param  x   The X component
*   @param  y   The Y component
*   @return     TVector2D of type @a T
*   @ingroup    vectors
*/
template<typename T>
TVector2D<T> MakeVector2D(T x, T y)
{
    TVector2D<T> vec = {x, y};
    return vec;
}

#ifdef _DOXYGEN_DO_INCLUDE
}
#endif /* _DOXYGEN_DO_INCLUDE */

#endif /* DEWPSI_BITS_VEC2_HPP */
